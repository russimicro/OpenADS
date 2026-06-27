#include "sql_backend/odbc_connection.h"

#include "sql_backend/odbc_backend.h"
#include "sql_backend/sql_common.h"

#include "openads/ace.h"
#include "openads/error.h"

#if !defined(OPENADS_WITH_ODBC)

// This translation unit is only compiled when the ODBC backend is
// enabled (see src/CMakeLists.txt, target_sources under OPENADS_WITH_ODBC).
// Guard defensively so a stray build without the flag is an empty unit
// rather than a pile of undefined-symbol errors.
namespace openads::sql_backend {}

#else

#if defined(_WIN32)
#  include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdlib>

namespace openads::sql_backend {

namespace {

inline SQLCHAR* sqlstr(const std::string& s) {
    return const_cast<SQLCHAR*>(
        reinterpret_cast<const SQLCHAR*>(s.c_str()));
}

std::string odbc_diag(SQLSMALLINT handle_type, SQLHANDLE handle) {
    std::string out;
    SQLCHAR     state[6];
    SQLINTEGER  native = 0;
    SQLCHAR     msg[1024];
    SQLSMALLINT msg_len = 0;
    for (SQLSMALLINT i = 1; i <= 8; ++i) {
        SQLRETURN r = SQLGetDiagRec(handle_type, handle, i, state, &native,
                                    msg, static_cast<SQLSMALLINT>(sizeof(msg)),
                                    &msg_len);
        if (r == SQL_NO_DATA || !SQL_SUCCEEDED(r)) break;
        if (!out.empty()) out += "; ";
        out += reinterpret_cast<const char*>(state);
        out += ": ";
        out += reinterpret_cast<const char*>(msg);
    }
    return out;
}

// Read every row of an already-executed statement, each cell coerced to
// a UTF-8/ANSI string via SQL_C_CHAR (long values stitched across
// SQLGetData calls). NULL cells are flagged.
util::Result<void> read_all_rows(
    SQLHSTMT st,
    std::vector<std::vector<std::string>>& rows,
    std::vector<std::vector<bool>>& nulls) {
    SQLSMALLINT cols = 0;
    if (!SQL_SUCCEEDED(SQLNumResultCols(st, &cols))) {
        return odbc_error("odbc num cols", odbc_diag(SQL_HANDLE_STMT, st));
    }
    // INSERT / UPDATE / DELETE produce no result set; there is nothing to
    // fetch and SQLFetch on a cursorless statement would error. Return the
    // empty row set so the write path can reuse run_query for DML.
    if (cols == 0) return util::Result<void>{};
    while (true) {
        SQLRETURN r = SQLFetch(st);
        if (r == SQL_NO_DATA) break;
        if (!SQL_SUCCEEDED(r)) {
            return odbc_error("odbc fetch", odbc_diag(SQL_HANDLE_STMT, st));
        }
        std::vector<std::string> row;
        std::vector<bool>        rn;
        row.reserve(cols);
        rn.reserve(cols);
        for (SQLSMALLINT c = 1; c <= cols; ++c) {
            std::string val;
            bool        is_null = false;
            char        buf[4096];
            SQLLEN      ind = 0;
            while (true) {
                SQLRETURN gr = SQLGetData(st, static_cast<SQLUSMALLINT>(c),
                                          SQL_C_CHAR, buf,
                                          static_cast<SQLLEN>(sizeof(buf)),
                                          &ind);
                if (gr == SQL_NO_DATA) break;
                if (!SQL_SUCCEEDED(gr)) {
                    return odbc_error("odbc getdata",
                                      odbc_diag(SQL_HANDLE_STMT, st));
                }
                if (ind == SQL_NULL_DATA) {
                    is_null = true;
                    break;
                }
                std::size_t chunk;
                if (ind == SQL_NO_TOTAL ||
                    ind >= static_cast<SQLLEN>(sizeof(buf))) {
                    chunk = sizeof(buf) - 1;  // truncated; NUL eats one byte
                } else if (ind < 0) {
                    chunk = 0;                // stray driver indicator; ignore
                } else {
                    chunk = static_cast<std::size_t>(ind);
                }
                val.append(buf, chunk);
                if (gr == SQL_SUCCESS) break;  // all data consumed
                // SQL_SUCCESS_WITH_INFO: more remains, keep reading
            }
            row.push_back(std::move(val));
            rn.push_back(is_null);
        }
        rows.push_back(std::move(row));
        nulls.push_back(std::move(rn));
    }
    return util::Result<void>{};
}

// ---------------------------------------------------------------------------
// BoundParam: a typed parameter for parameterised SQL queries.
// sql_type / col_size / decimals carry the ODBC column metadata so the
// driver can coerce the value correctly (e.g. SQL_INTEGER vs SQL_VARCHAR).
// is_null=true emits SQL_NULL_DATA regardless of value.
// ---------------------------------------------------------------------------
struct BoundParam {
    std::string value;
    bool        is_null  = false;
    SQLSMALLINT sql_type = SQL_VARCHAR;
    SQLULEN     col_size = 0;
    SQLSMALLINT decimals = 0;
};

// True for ADS column types whose empty-string value is meaningful (a blank
// string is NOT NULL). Non-textual types (numeric, date, …) that arrive
// empty from xBase must be sent as SQL NULL — the NULL-on-write rule.
bool is_textual(std::uint16_t ads_type) {
    return ads_type == ADS_STRING;
}

// Build a BoundParam for `value` in `column` of `tbl`, applying the
// NULL-on-write rule. Tasks 3 and 4 reuse this helper.
BoundParam param_for(const OdbcTable& tbl, const std::string& column,
                     const std::string& value) {
    BoundParam p;
    const std::size_t idx = field_index_ci(tbl, column);
    if (idx != static_cast<std::size_t>(-1)) {
        const auto& f = tbl.fields[idx];
        if (f.sql_type != 0) p.sql_type = static_cast<SQLSMALLINT>(f.sql_type);
        p.col_size = f.column_size;
        p.decimals = static_cast<SQLSMALLINT>(f.decimals);
        if (value.empty() && !is_textual(f.type)) p.is_null = true;
    }
    p.value = value;
    return p;
}

// ---------------------------------------------------------------------------
// run_query — parameterised overload (read/navigation path).
// Binds params via SQLBindParameter (SQL_C_CHAR); the indicator array keeps
// alive for the duration of the call since it is a local variable.
// ---------------------------------------------------------------------------
util::Result<void> run_query(
    SQLHDBC dbc, const std::string& sql,
    const std::vector<BoundParam>& params,
    std::vector<std::vector<std::string>>& rows,
    std::vector<std::vector<bool>>& nulls,
    SQLULEN max_rows = 0) {
    SQLHSTMT st = SQL_NULL_HSTMT;
    if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &st))) {
        return odbc_error("odbc alloc stmt", odbc_diag(SQL_HANDLE_DBC, dbc));
    }
    if (max_rows > 0) {
        SQLSetStmtAttr(st, SQL_ATTR_MAX_ROWS,
                       reinterpret_cast<SQLPOINTER>(max_rows), 0);
    }
    std::vector<SQLLEN> ind(params.size());
    for (std::size_t i = 0; i < params.size(); ++i) {
        const BoundParam& p = params[i];
        ind[i] = p.is_null ? SQL_NULL_DATA
                           : static_cast<SQLLEN>(p.value.size());
        SQLRETURN br = SQLBindParameter(
            st, static_cast<SQLUSMALLINT>(i + 1), SQL_PARAM_INPUT,
            SQL_C_CHAR, p.sql_type,
            p.col_size > 0 ? p.col_size : (p.value.empty() ? 1 : p.value.size()),
            p.decimals,
            const_cast<char*>(p.value.c_str()),
            static_cast<SQLLEN>(p.value.size()), &ind[i]);
        if (!SQL_SUCCEEDED(br)) {
            auto e = odbc_error("odbc bind param",
                                odbc_diag(SQL_HANDLE_STMT, st));
            SQLFreeHandle(SQL_HANDLE_STMT, st);
            return e;
        }
    }
    SQLRETURN r = SQLExecDirect(st, sqlstr(sql), SQL_NTS);
    if (!SQL_SUCCEEDED(r) && r != SQL_NO_DATA) {
        auto e = odbc_error("odbc exec", odbc_diag(SQL_HANDLE_STMT, st));
        SQLFreeHandle(SQL_HANDLE_STMT, st);
        return e;
    }
    auto rr = read_all_rows(st, rows, nulls);
    SQLFreeHandle(SQL_HANDLE_STMT, st);
    return rr;
}

// No-parameter convenience — for metadata/snapshot reads with no values to
// bind (load_pk_snapshot, seek_index, flush/delete via the write path).
util::Result<void> run_query(
    SQLHDBC dbc, const std::string& sql,
    std::vector<std::vector<std::string>>& rows,
    std::vector<std::vector<bool>>& nulls,
    SQLULEN max_rows = 0) {
    static const std::vector<BoundParam> kNoParams;
    return run_query(dbc, sql, kNoParams, rows, nulls, max_rows);
}

std::string quote_ident(const std::string& q, const std::string& name) {
    if (q.empty()) return name;
    return q + name + q;
}


std::string index_column_sql(const std::string& q, const std::string& column,
                             IndexExprKind kind) {
    const std::string qcol = quote_ident(q, column);
    return kind == IndexExprKind::UpperColumn ? "UPPER(" + qcol + ")" : qcol;
}

std::string pk_select_list(const std::string& q, const OdbcTable& tbl) {
    std::string out;
    for (std::size_t i = 0; i < tbl.pk_columns.size(); ++i) {
        if (i > 0) out += ", ";
        out += quote_ident(q, tbl.pk_columns[i]);
    }
    return out;
}

// Parameterised WHERE clause — used by the READ/navigation and WRITE paths.
// Emits `col = ? AND ...` and appends one BoundParam per PK column to `out`.
// Tasks 3 and 4 will migrate UPDATE/DELETE callers to this overload too.
std::string pk_where_clause(const std::string& q, const OdbcTable& tbl,
                            const OdbcTable::PkRow& pk,
                            std::vector<BoundParam>& out) {
    std::string s;
    for (std::size_t i = 0; i < tbl.pk_columns.size(); ++i) {
        if (i > 0) s += " AND ";
        s += quote_ident(q, tbl.pk_columns[i]) + " = ?";
        out.push_back(param_for(tbl, tbl.pk_columns[i], pk.values[i]));
    }
    return s;
}

std::vector<std::string> order_keyed(std::vector<std::pair<int, std::string>> k) {
    std::stable_sort(k.begin(), k.end(),
                     [](const auto& a, const auto& b) {
                         return a.first < b.first;
                     });
    std::vector<std::string> out;
    out.reserve(k.size());
    for (auto& kv : k) out.push_back(kv.second);
    return out;
}

// Try SQLPrimaryKeys, falling back to the first UNIQUE index reported by
// SQLStatistics. Many drivers (the Access/Jet ODBC driver among them) do
// not implement SQLPrimaryKeys but do report a unique index, which is an
// equally valid stable row key for snapshot navigation.
util::Result<std::vector<std::string>>
discover_pk(SQLHDBC dbc, const std::string& name) {
    // 1) SQLPrimaryKeys — columns (1-based): 4 COLUMN_NAME, 5 KEY_SEQ.
    {
        SQLHSTMT st = SQL_NULL_HSTMT;
        if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &st))) {
            return odbc_error("odbc alloc stmt", odbc_diag(SQL_HANDLE_DBC, dbc));
        }
        SQLRETURN r = SQLPrimaryKeys(st, nullptr, 0, nullptr, 0,
                                     sqlstr(name), SQL_NTS);
        if (SQL_SUCCEEDED(r)) {
            std::vector<std::vector<std::string>> rows;
            std::vector<std::vector<bool>>        nulls;
            auto rr = read_all_rows(st, rows, nulls);
            SQLFreeHandle(SQL_HANDLE_STMT, st);
            if (!rr) return rr.error();
            // Pin to the first schema seen (col 2 = TABLE_SCHEM): with a
            // null schema arg, drivers that support schemas can return PK
            // columns for same-named tables across schemas; mixing them
            // would corrupt the key.
            std::string chosen_schema;
            bool        has_schema = false;
            std::vector<std::pair<int, std::string>> keyed;
            for (auto& row : rows) {
                std::string schem = row.size() >= 2 ? row[1] : std::string();
                std::string col   = row.size() >= 4 ? row[3] : std::string();
                int seq = row.size() >= 5 ? std::atoi(row[4].c_str()) : 0;
                if (!has_schema) { chosen_schema = schem; has_schema = true; }
                if (schem != chosen_schema) continue;
                if (!col.empty()) keyed.emplace_back(seq, col);
            }
            if (!keyed.empty()) return order_keyed(std::move(keyed));
        } else {
            // Unsupported (IM001) or failed — fall through to statistics.
            SQLFreeHandle(SQL_HANDLE_STMT, st);
        }
    }

    // 2) SQLStatistics unique index — columns (1-based): 4 NON_UNIQUE
    // (0 = unique), 6 INDEX_NAME, 8 ORDINAL_POSITION, 9 COLUMN_NAME.
    {
        SQLHSTMT st = SQL_NULL_HSTMT;
        if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &st))) {
            return odbc_error("odbc alloc stmt", odbc_diag(SQL_HANDLE_DBC, dbc));
        }
        SQLRETURN r = SQLStatistics(st, nullptr, 0, nullptr, 0,
                                    sqlstr(name), SQL_NTS,
                                    SQL_INDEX_UNIQUE, SQL_QUICK);
        if (SQL_SUCCEEDED(r)) {
            std::vector<std::vector<std::string>> rows;
            std::vector<std::vector<bool>>        nulls;
            auto rr = read_all_rows(st, rows, nulls);
            SQLFreeHandle(SQL_HANDLE_STMT, st);
            if (!rr) return rr.error();
            std::string chosen;
            std::string chosen_schema;
            bool        has_schema = false;
            std::vector<std::pair<int, std::string>> keyed;
            for (auto& row : rows) {
                const std::string schem      = row.size() >= 2 ? row[1] : "";
                const std::string non_unique = row.size() >= 4 ? row[3] : "";
                const std::string idx_name   = row.size() >= 6 ? row[5] : "";
                const std::string ord_s      = row.size() >= 8 ? row[7] : "";
                const std::string col_name   = row.size() >= 9 ? row[8] : "";
                if (col_name.empty()) continue;     // table-statistic row
                if (non_unique != "0") continue;    // only unique indexes
                if (!has_schema) { chosen_schema = schem; has_schema = true; }
                if (schem != chosen_schema) continue;
                if (chosen.empty()) chosen = idx_name;
                if (idx_name != chosen) continue;
                keyed.emplace_back(std::atoi(ord_s.c_str()), col_name);
            }
            if (!keyed.empty()) return order_keyed(std::move(keyed));
        } else {
            SQLFreeHandle(SQL_HANDLE_STMT, st);
        }
    }

    return util::Error{5001, 0,
        "table has no primary key or unique index (ODBC backend v1 "
        "navigates by a stable key)", name};
}

util::Result<void> describe_columns(SQLHDBC dbc, OdbcTable* tbl) {
    if (!is_safe_identifier(tbl->name)) {
        return util::Error{5001, 0, "invalid table name", tbl->name};
    }
    SQLHSTMT st = SQL_NULL_HSTMT;
    if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &st))) {
        return odbc_error("odbc alloc stmt", odbc_diag(SQL_HANDLE_DBC, dbc));
    }
    SQLRETURN r = SQLColumns(st, nullptr, 0, nullptr, 0,
                             sqlstr(tbl->name), SQL_NTS, nullptr, 0);
    if (!SQL_SUCCEEDED(r)) {
        auto e = odbc_error("SQLColumns", odbc_diag(SQL_HANDLE_STMT, st));
        SQLFreeHandle(SQL_HANDLE_STMT, st);
        return e;
    }
    std::vector<std::vector<std::string>> rows;
    std::vector<std::vector<bool>>        nulls;
    auto rr = read_all_rows(st, rows, nulls);
    SQLFreeHandle(SQL_HANDLE_STMT, st);
    if (!rr) return rr.error();
    if (rows.empty()) {
        return util::Error{5001, 0, "table not found or has no columns",
                           tbl->name};
    }
    // SQLColumns columns (1-based): 4 COLUMN_NAME, 5 DATA_TYPE,
    // 7 COLUMN_SIZE, 9 DECIMAL_DIGITS, 11 NULLABLE. Rows arrive in
    // ORDINAL_POSITION order per the ODBC spec.
    std::string chosen_schema;
    bool        has_schema = false;
    std::vector<OdbcTable::FieldDesc> out;
    out.reserve(rows.size());
    for (auto& row : rows) {
        auto cell = [&](std::size_t one_based) -> std::string {
            std::size_t i = one_based - 1;
            return i < row.size() ? row[i] : std::string();
        };
        // Pin to the first schema seen (col 2 = TABLE_SCHEM) so a null
        // schema arg cannot mix columns of same-named tables in different
        // schemas into one field list.
        const std::string schem = cell(2);
        if (!has_schema) {
            chosen_schema     = schem;
            tbl->sql_table    = cell(3);
            has_schema        = true;
        }
        if (schem != chosen_schema) continue;
        const std::string cname = cell(4);
        const int dtype = std::atoi(cell(5).c_str());
        const int csize = std::atoi(cell(7).c_str());
        const int ddig  = std::atoi(cell(9).c_str());
        const int ncode = std::atoi(cell(11).c_str());  // 0=NO_NULLS,1=NULLABLE
        out.push_back(map_odbc_column(cname, dtype, ncode != 0, csize, ddig));
    }
    tbl->fields        = std::move(out);
    tbl->fields_cached = true;
    return util::Result<void>{};
}

util::Result<void> load_pk_snapshot(SQLHDBC dbc, const std::string& q,
                                    OdbcTable* tbl) {
    const std::string list = pk_select_list(q, *tbl);
    const std::string sql =
        "SELECT " + list + " FROM " + quote_ident(q, tbl->sql_table) +
        " ORDER BY " + list;
    std::vector<std::vector<std::string>> rows;
    std::vector<std::vector<bool>>        nulls;
    auto r = run_query(dbc, sql, rows, nulls);
    if (!r) return r.error();
    tbl->pk_snapshot.clear();
    tbl->pk_snapshot.reserve(rows.size());
    for (auto& row : rows) {
        OdbcTable::PkRow pk;
        pk.values = std::move(row);
        tbl->pk_snapshot.push_back(std::move(pk));
    }
    return util::Result<void>{};
}

util::Result<void> load_current_row(SQLHDBC dbc, const std::string& q,
                                    OdbcTable* tbl, std::size_t idx) {
    if (idx >= tbl->pk_snapshot.size()) {
        tbl->positioned = false;
        tbl->row_valid  = false;
        return util::Result<void>{};
    }
    std::vector<BoundParam> params;
    const std::string sql =
        "SELECT * FROM " + quote_ident(q, tbl->sql_table) + " WHERE " +
        pk_where_clause(q, *tbl, tbl->pk_snapshot[idx], params);
    std::vector<std::vector<std::string>> rows;
    std::vector<std::vector<bool>>        nulls;
    auto r = run_query(dbc, sql, params, rows, nulls, /*max_rows=*/1);
    if (!r) return r.error();
    if (rows.empty()) {
        tbl->positioned = false;
        tbl->row_valid  = false;
        return util::Result<void>{};
    }
    tbl->current_row   = std::move(rows[0]);
    tbl->current_nulls = std::move(nulls[0]);
    tbl->pos           = idx;
    tbl->current_recno = static_cast<std::uint32_t>(idx + 1);
    tbl->positioned    = true;
    tbl->row_valid     = true;
    return util::Result<void>{};
}

} // namespace

struct OdbcConnection::Impl {
    SQLHENV     env = SQL_NULL_HENV;
    SQLHDBC     dbc = SQL_NULL_HDBC;
    std::string quote;
    std::string dbms_name;   // SQL_DBMS_NAME, e.g. "Microsoft SQL Server"
};

OdbcConnection::OdbcConnection() = default;
OdbcConnection::~OdbcConnection() { disconnect(); }

OdbcConnection::OdbcConnection(OdbcConnection&& other) noexcept
    : impl_(std::move(other.impl_)) {}

OdbcConnection& OdbcConnection::operator=(OdbcConnection&& other) noexcept {
    if (this != &other) {
        disconnect();
        impl_ = std::move(other.impl_);
    }
    return *this;
}

util::Result<OdbcConnection> OdbcConnection::open(const OdbcUri& uri) {
    OdbcConnection conn;
    conn.impl_ = std::make_unique<Impl>();

    if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE,
                                      &conn.impl_->env))) {
        return util::Error{5001, 0, "odbc: SQLAllocHandle(ENV) failed", ""};
    }
    if (!SQL_SUCCEEDED(SQLSetEnvAttr(conn.impl_->env, SQL_ATTR_ODBC_VERSION,
                                     reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3),
                                     0))) {
        return odbc_error("odbc: SQLSetEnvAttr(ODBC_VERSION)",
                          odbc_diag(SQL_HANDLE_ENV, conn.impl_->env));
    }
    if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, conn.impl_->env,
                                      &conn.impl_->dbc))) {
        return odbc_error("odbc: SQLAllocHandle(DBC)",
                          odbc_diag(SQL_HANDLE_ENV, conn.impl_->env));
    }

    SQLCHAR     outbuf[2048];
    SQLSMALLINT outlen = 0;
    SQLRETURN r = SQLDriverConnect(
        conn.impl_->dbc, nullptr, sqlstr(uri.connstr), SQL_NTS,
        outbuf, static_cast<SQLSMALLINT>(sizeof(outbuf)), &outlen,
        SQL_DRIVER_NOPROMPT);
    if (!SQL_SUCCEEDED(r)) {
        return odbc_error("odbc connect",
                          odbc_diag(SQL_HANDLE_DBC, conn.impl_->dbc));
    }

    SQLCHAR     q[16] = {0};
    SQLSMALLINT qlen  = 0;
    if (SQL_SUCCEEDED(SQLGetInfo(conn.impl_->dbc, SQL_IDENTIFIER_QUOTE_CHAR,
                                 q, static_cast<SQLSMALLINT>(sizeof(q)),
                                 &qlen))) {
        std::string qs(reinterpret_cast<const char*>(q));
        if (!qs.empty() && qs != " ") conn.impl_->quote = qs;
    }

    // DBMS name decides which lock primitive is available (only SQL Server has
    // sp_getapplock); cache it once here rather than probing per lock call.
    SQLCHAR     dn[256] = {0};
    SQLSMALLINT dnlen   = 0;
    if (SQL_SUCCEEDED(SQLGetInfo(conn.impl_->dbc, SQL_DBMS_NAME, dn,
                                 static_cast<SQLSMALLINT>(sizeof(dn)), &dnlen))) {
        conn.impl_->dbms_name.assign(reinterpret_cast<const char*>(dn));
    }
    return std::move(conn);
}

void OdbcConnection::disconnect() noexcept {
    if (impl_) {
        if (impl_->dbc != SQL_NULL_HDBC) {
            SQLDisconnect(impl_->dbc);
            SQLFreeHandle(SQL_HANDLE_DBC, impl_->dbc);
            impl_->dbc = SQL_NULL_HDBC;
        }
        if (impl_->env != SQL_NULL_HENV) {
            SQLFreeHandle(SQL_HANDLE_ENV, impl_->env);
            impl_->env = SQL_NULL_HENV;
        }
    }
    impl_.reset();
}

bool OdbcConnection::valid() const noexcept {
    return impl_ && impl_->dbc != SQL_NULL_HDBC;
}

util::Result<std::unique_ptr<OdbcTable>>
OdbcConnection::open_table(const std::string& table_name) {
    if (!valid()) {
        return util::Error{5001, 0, "odbc connection not open", ""};
    }
    if (!is_safe_identifier(table_name)) {
        return util::Error{5001, 0, "invalid table name", table_name};
    }
    auto tbl  = std::make_unique<OdbcTable>();
    tbl->conn = this;
    tbl->name      = table_name;
    tbl->sql_table = table_name;

    auto pk = discover_pk(impl_->dbc, table_name);
    if (!pk) return pk.error();
    tbl->pk_columns = std::move(pk).value();

    if (auto d = describe_columns(impl_->dbc, tbl.get()); !d) {
        return d.error();
    }
    if (auto s = load_pk_snapshot(impl_->dbc, impl_->quote, tbl.get()); !s) {
        return s.error();
    }
    tbl->cached_rec_count = static_cast<std::uint32_t>(tbl->pk_snapshot.size());
    tbl->rec_count_cached = true;
    tbl->positioned       = false;
    tbl->row_valid        = false;
    tbl->pos              = 0;
    tbl->current_recno    = 0;
    return tbl;
}

util::Result<void> OdbcConnection::goto_top(OdbcTable* tbl) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc goto_top", ""};
    }
    if (tbl->cached_rec_count == 0) {
        tbl->positioned    = false;
        tbl->row_valid     = false;
        tbl->current_recno = 0;
        tbl->pos           = 0;
        return util::Result<void>{};
    }
    return load_current_row(impl_->dbc, impl_->quote, tbl, 0);
}

util::Result<void> OdbcConnection::goto_bottom(OdbcTable* tbl) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc goto_bottom", ""};
    }
    if (tbl->cached_rec_count == 0) {
        tbl->positioned    = false;
        tbl->row_valid     = false;
        tbl->current_recno = 0;
        tbl->pos           = 0;
        return util::Result<void>{};
    }
    return load_current_row(impl_->dbc, impl_->quote, tbl,
                            tbl->cached_rec_count - 1);
}

util::Result<void> OdbcConnection::skip(OdbcTable* tbl, std::int32_t step) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc skip", ""};
    }
    if (step == 0) return util::Result<void>{};
    if (tbl->cached_rec_count == 0) {
        tbl->positioned = false;
        tbl->row_valid  = false;
        tbl->pos        = 0;
        return util::Result<void>{};
    }

    std::int64_t next = 0;
    if (!tbl->positioned) {
        if (tbl->pos == 0) {
            if (step > 0) next = step - 1;
            else return util::Error{5026, 0, "bof", ""};
        } else {
            if (step < 0) next = static_cast<std::int64_t>(tbl->pos) + step;
            else return util::Result<void>{};
        }
    } else {
        next = static_cast<std::int64_t>(tbl->pos) + step;
    }

    if (next < 0) {
        tbl->positioned = false;
        tbl->row_valid  = false;
        tbl->pos        = 0;
        return util::Error{5026, 0, "bof", ""};
    }
    if (static_cast<std::uint32_t>(next) >= tbl->cached_rec_count) {
        tbl->positioned = false;
        tbl->row_valid  = false;
        tbl->pos        = tbl->cached_rec_count;
        return util::Result<void>{};
    }
    return load_current_row(impl_->dbc, impl_->quote, tbl,
                            static_cast<std::size_t>(next));
}

util::Result<bool> OdbcConnection::at_eof(OdbcTable* tbl) const {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc at_eof", ""};
    }
    if (tbl->cached_rec_count == 0) return true;
    if (!tbl->positioned && tbl->pos >= tbl->cached_rec_count) return true;
    return false;
}

util::Result<bool> OdbcConnection::at_bof(OdbcTable* tbl) const {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc at_bof", ""};
    }
    if (tbl->cached_rec_count == 0) return true;
    return !tbl->positioned && tbl->pos == 0;
}

util::Result<std::uint32_t> OdbcConnection::record_count(OdbcTable* tbl) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc record_count", ""};
    }
    if (tbl->rec_count_cached) return tbl->cached_rec_count;
    tbl->cached_rec_count = static_cast<std::uint32_t>(tbl->pk_snapshot.size());
    tbl->rec_count_cached = true;
    return tbl->cached_rec_count;
}

util::Result<std::vector<OdbcTable::FieldDesc>>
OdbcConnection::describe_table(OdbcTable* tbl) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc describe_table", ""};
    }
    if (tbl->fields_cached) return tbl->fields;
    if (auto d = describe_columns(impl_->dbc, tbl); !d) return d.error();
    return tbl->fields;
}

util::Result<void> OdbcConnection::read_field(
    OdbcTable* tbl, const std::string& field_name,
    std::string& buf, bool& is_null) const {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc read_field", ""};
    }
    if (!tbl->fields_cached) {
        return util::Error{5001, 0, "schema not cached", ""};
    }
    if (!tbl->row_valid) {
        return util::Error{5026, 0, "no current record", ""};
    }
    const std::size_t idx = field_index_ci(*tbl, field_name);
    if (idx == static_cast<std::size_t>(-1)) {
        return util::Error{5063, 0, "column not found", field_name};
    }
    if (idx >= tbl->current_row.size()) {
        return util::Error{5001, 0, "row cache mismatch", ""};
    }
    is_null = tbl->current_nulls[idx];
    buf     = tbl->current_row[idx];
    return util::Result<void>{};
}

util::Result<bool> OdbcConnection::seek_index(
    OdbcTable* tbl, const std::string& column, IndexExprKind kind,
    const std::string& key, bool soft, bool last_key) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc seek", ""};
    }
    if (!is_safe_identifier(column)) {
        return util::Error{5001, 0, "invalid seek column", column};
    }
    if (!tbl->fields_cached) {
        if (auto d = describe_columns(impl_->dbc, tbl); !d) return d.error();
    }
    const std::size_t col_idx = field_index_ci(*tbl, column);
    if (col_idx == static_cast<std::size_t>(-1)) {
        return util::Error{5063, 0, "seek column not found", column};
    }
    const std::string& sql_col = tbl->fields[col_idx].name;

    const std::string& q      = impl_->quote;
    const std::string  pkcols = pk_select_list(q, *tbl);
    const std::string  qexpr  = index_column_sql(q, sql_col, kind);
    const std::string  from   = " FROM " + quote_ident(q, tbl->sql_table);

    std::vector<BoundParam> params;
    if (kind == IndexExprKind::UpperColumn) {
        BoundParam p; p.sql_type = SQL_VARCHAR; p.value = key;
        params.push_back(p);
    } else {
        params.push_back(param_for(*tbl, sql_col, key));
    }

    std::string sql;
    if (last_key) {
        sql = soft
            ? "SELECT " + pkcols + from + " WHERE " + qexpr + " <= ?"
              " ORDER BY " + qexpr + " DESC"
            : "SELECT " + pkcols + from + " WHERE " + qexpr + " = ?"
              " ORDER BY " + qexpr + " DESC";
    } else {
        sql = soft
            ? "SELECT " + pkcols + from + " WHERE " + qexpr + " >= ?"
              " ORDER BY " + qexpr + " ASC"
            : "SELECT " + pkcols + from + " WHERE " + qexpr + " = ?";
    }

    std::vector<std::vector<std::string>> rows;
    std::vector<std::vector<bool>>        nulls;
    auto r = run_query(impl_->dbc, sql, params, rows, nulls, /*max_rows=*/1);
    if (!r) return r.error();

    bool found = false;
    if (!rows.empty()) {
        OdbcTable::PkRow pk;
        pk.values = rows[0];
        std::size_t pos = static_cast<std::size_t>(-1);
        for (std::size_t i = 0; i < tbl->pk_snapshot.size(); ++i) {
            if (tbl->pk_snapshot[i] == pk) {
                pos = i;
                break;
            }
        }
        if (pos != static_cast<std::size_t>(-1)) {
            if (auto lr = load_current_row(impl_->dbc, q, tbl, pos); !lr) {
                return lr.error();
            }
            found = tbl->positioned && tbl->row_valid;
        } else {
            tbl->positioned = false;
            tbl->row_valid  = false;
            found           = false;
        }
    } else {
        tbl->positioned = false;
        tbl->row_valid  = false;
        found           = false;
    }
    tbl->last_seek_found = found;
    return found;
}

namespace {

bool ci_equal(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

// Build the PK tuple of the row just written, so flush_table can reposition
// the cursor on it after reloading the snapshot. For an append the PK comes
// from the staged values; for an edit it is the current PK with any staged
// PK-column override applied.
OdbcTable::PkRow target_pk(const OdbcTable& tbl, bool appending) {
    OdbcTable::PkRow pk;
    pk.values.reserve(tbl.pk_columns.size());
    for (std::size_t c = 0; c < tbl.pk_columns.size(); ++c) {
        std::string v = (!appending && tbl.pos < tbl.pk_snapshot.size())
                            ? tbl.pk_snapshot[tbl.pos].values[c]
                            : std::string();
        for (const auto& kv : tbl.staged) {
            if (ci_equal(kv.first, tbl.pk_columns[c])) { v = kv.second; break; }
        }
        pk.values.push_back(v);
    }
    return pk;
}

} // namespace

util::Result<void> OdbcConnection::append_blank(OdbcTable* tbl) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc append", ""};
    }
    if (!tbl->fields_cached) {
        if (auto d = describe_columns(impl_->dbc, tbl); !d) return d.error();
    }
    tbl->staged.clear();
    tbl->appending = true;
    return util::Result<void>{};
}

util::Result<void> OdbcConnection::set_field(OdbcTable* tbl,
                                             const std::string& name,
                                             const std::string& value) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc set_field", ""};
    }
    if (!tbl->fields_cached) {
        if (auto d = describe_columns(impl_->dbc, tbl); !d) return d.error();
    }
    const std::size_t idx = field_index_ci(*tbl, name);
    if (idx == static_cast<std::size_t>(-1)) {
        return util::Error{5063, 0, "column not found", name};
    }
    const std::string& sqlname = tbl->fields[idx].name;  // driver casing
    for (auto& kv : tbl->staged) {
        if (ci_equal(kv.first, sqlname)) { kv.second = value; return {}; }
    }
    tbl->staged.emplace_back(sqlname, value);
    return util::Result<void>{};
}

util::Result<void> OdbcConnection::flush_table(OdbcTable* tbl) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc flush", ""};
    }
    const std::string& q = impl_->quote;

    if (tbl->appending) {
        if (tbl->staged.empty()) {
            return util::Error{5001, 0, "append with no fields set",
                               tbl->name};
        }
        std::string cols, marks;
        std::vector<BoundParam> params;
        for (std::size_t i = 0; i < tbl->staged.size(); ++i) {
            if (i) { cols += ", "; marks += ", "; }
            cols  += quote_ident(q, tbl->staged[i].first);
            marks += "?";
            params.push_back(param_for(*tbl, tbl->staged[i].first,
                                       tbl->staged[i].second));
        }
        const std::string sql =
            "INSERT INTO " + quote_ident(q, tbl->sql_table) +
            " (" + cols + ") VALUES (" + marks + ")";
        std::vector<std::vector<std::string>> rows;
        std::vector<std::vector<bool>>        nulls;
        if (auto r = run_query(impl_->dbc, sql, params, rows, nulls); !r) {
            return r.error();
        }
    } else {
        if (tbl->staged.empty()) return util::Result<void>{};  // no-op edit
        if (!tbl->positioned || tbl->pos >= tbl->pk_snapshot.size()) {
            return util::Error{5026, 0, "no current record to update", ""};
        }
        std::string sets;
        std::vector<BoundParam> params;
        for (std::size_t i = 0; i < tbl->staged.size(); ++i) {
            if (i) sets += ", ";
            sets += quote_ident(q, tbl->staged[i].first) + " = ?";
            params.push_back(param_for(*tbl, tbl->staged[i].first,
                                       tbl->staged[i].second));
        }
        const std::string where =
            pk_where_clause(q, *tbl, tbl->pk_snapshot[tbl->pos], params);
        const std::string sql =
            "UPDATE " + quote_ident(q, tbl->sql_table) + " SET " + sets +
            " WHERE " + where;
        std::vector<std::vector<std::string>> rows;
        std::vector<std::vector<bool>>        nulls;
        if (auto r = run_query(impl_->dbc, sql, params, rows, nulls); !r) {
            return r.error();
        }
    }

    const OdbcTable::PkRow want = target_pk(*tbl, tbl->appending);
    tbl->staged.clear();
    tbl->appending = false;

    if (auto s = load_pk_snapshot(impl_->dbc, q, tbl); !s) return s.error();
    tbl->cached_rec_count = static_cast<std::uint32_t>(tbl->pk_snapshot.size());
    tbl->rec_count_cached = true;

    std::size_t pos = static_cast<std::size_t>(-1);
    for (std::size_t i = 0; i < tbl->pk_snapshot.size(); ++i) {
        if (tbl->pk_snapshot[i] == want) { pos = i; break; }
    }
    if (pos != static_cast<std::size_t>(-1)) {
        return load_current_row(impl_->dbc, q, tbl, pos);
    }
    tbl->positioned = false;
    tbl->row_valid  = false;
    return util::Result<void>{};
}

util::Result<void> OdbcConnection::delete_record(OdbcTable* tbl) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc delete", ""};
    }
    tbl->staged.clear();
    tbl->appending = false;
    if (!tbl->positioned || tbl->pos >= tbl->pk_snapshot.size()) {
        return util::Error{5026, 0, "no current record to delete", ""};
    }
    const std::string& q = impl_->quote;
    std::vector<BoundParam> params;
    const std::string where =
        pk_where_clause(q, *tbl, tbl->pk_snapshot[tbl->pos], params);
    const std::string sql =
        "DELETE FROM " + quote_ident(q, tbl->sql_table) + " WHERE " + where;
    std::vector<std::vector<std::string>> rows;
    std::vector<std::vector<bool>>        nulls;
    if (auto r = run_query(impl_->dbc, sql, params, rows, nulls); !r) {
        return r.error();
    }

    if (auto s = load_pk_snapshot(impl_->dbc, q, tbl); !s) return s.error();
    tbl->cached_rec_count = static_cast<std::uint32_t>(tbl->pk_snapshot.size());
    tbl->rec_count_cached = true;
    tbl->positioned = false;
    tbl->row_valid  = false;
    return util::Result<void>{};
}

// ---- rLock()/fLock() via SQL Server application locks --------------------
namespace {

bool is_sql_server(const std::string& dbms) {
    return dbms.find("Microsoft SQL Server") != std::string::npos;
}

std::string odbc_hash_key(const std::string& s) {
    std::uint64_t h = 1469598103934665603ULL;       // FNV-1a 64-bit
    for (unsigned char c : s) { h ^= c; h *= 1099511628211ULL; }
    static const char* hex = "0123456789abcdef";
    char buf[17];
    for (int i = 15; i >= 0; --i) { buf[i] = hex[h & 0xF]; h >>= 4; }
    return std::string(buf, 16);
}

std::string odbc_record_lock_key(const OdbcTable& tbl, std::size_t pos) {
    std::string k = "R\x1f" + tbl.name;
    if (pos < tbl.pk_snapshot.size()) {
        for (const std::string& v : tbl.pk_snapshot[pos].values) k += "\x1f" + v;
    }
    return odbc_hash_key(k);
}

std::string odbc_table_lock_key(const OdbcTable& tbl) {
    return odbc_hash_key("T\x1f" + tbl.name);
}

// Run sp_getapplock / sp_releaseapplock and return the integer the proc yields:
// >= 0 acquired/released, -1 not granted (timeout), other negatives are errors.
// `key` is hex, so it embeds in the N'...' literal with no escaping.
util::Result<int> odbc_applock(SQLHDBC dbc, const std::string& key, bool acquire) {
    const std::string sql =
        acquire
            ? "DECLARE @r int; EXEC @r = sp_getapplock @Resource=N'" + key +
              "', @LockMode='Exclusive', @LockOwner='Session', @LockTimeout=0; "
              "SELECT @r;"
            : "DECLARE @r int; EXEC @r = sp_releaseapplock @Resource=N'" + key +
              "', @LockOwner='Session'; SELECT @r;";
    std::vector<std::vector<std::string>> rows;
    std::vector<std::vector<bool>>        nulls;
    if (auto r = run_query(dbc, sql, rows, nulls); !r) return r.error();
    if (rows.empty() || rows[0].empty()) {
        return util::Error{5001, 0, "applock returned no value", ""};
    }
    return std::atoi(rows[0][0].c_str());
}

} // namespace

util::Result<void> OdbcConnection::lock_record(OdbcTable* tbl,
                                               std::uint32_t recno) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc lock", ""};
    }
    if (!is_sql_server(impl_->dbms_name)) {
        return util::Error{openads::AE_FUNCTION_NOT_AVAILABLE, 0,
                           "record lock not supported for this ODBC backend", ""};
    }
    const std::size_t pos =
        (recno == 0) ? tbl->pos : static_cast<std::size_t>(recno - 1);
    if (pos >= tbl->pk_snapshot.size()) {
        return util::Error{5026, 0, "no current record", ""};
    }
    auto r = odbc_applock(impl_->dbc, odbc_record_lock_key(*tbl, pos), true);
    if (!r) return r.error();
    if (r.value() < 0) return util::Error{5035, 0, "record locked", ""};
    return util::Result<void>{};
}

util::Result<void> OdbcConnection::unlock_record(OdbcTable* tbl,
                                                 std::uint32_t recno) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc unlock", ""};
    }
    if (!is_sql_server(impl_->dbms_name)) {
        return util::Error{openads::AE_FUNCTION_NOT_AVAILABLE, 0,
                           "record lock not supported for this ODBC backend", ""};
    }
    const std::size_t pos =
        (recno == 0) ? tbl->pos : static_cast<std::size_t>(recno - 1);
    if (pos >= tbl->pk_snapshot.size()) return util::Result<void>{};
    auto r = odbc_applock(impl_->dbc, odbc_record_lock_key(*tbl, pos), false);
    if (!r) return r.error();
    return util::Result<void>{};
}

util::Result<void> OdbcConnection::lock_table(OdbcTable* tbl) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc lock", ""};
    }
    if (!is_sql_server(impl_->dbms_name)) {
        return util::Error{openads::AE_FUNCTION_NOT_AVAILABLE, 0,
                           "table lock not supported for this ODBC backend", ""};
    }
    auto r = odbc_applock(impl_->dbc, odbc_table_lock_key(*tbl), true);
    if (!r) return r.error();
    if (r.value() < 0) return util::Error{5035, 0, "table locked", ""};
    return util::Result<void>{};
}

util::Result<void> OdbcConnection::unlock_table(OdbcTable* tbl) {
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid odbc unlock", ""};
    }
    if (!is_sql_server(impl_->dbms_name)) {
        return util::Error{openads::AE_FUNCTION_NOT_AVAILABLE, 0,
                           "table lock not supported for this ODBC backend", ""};
    }
    auto r = odbc_applock(impl_->dbc, odbc_table_lock_key(*tbl), false);
    if (!r) return r.error();
    return util::Result<void>{};
}

} // namespace openads::sql_backend

#endif // OPENADS_WITH_ODBC
