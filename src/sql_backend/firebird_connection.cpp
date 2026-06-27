#include "sql_backend/firebird_connection.h"

#include "sql_backend/sql_common.h"

#include "openads/ace.h"

#if !defined(OPENADS_WITH_FIREBIRD)

// Only compiled when the native Firebird backend is enabled (see
// src/CMakeLists.txt). Guard defensively so a stray build without the flag
// is an empty translation unit, not undefined symbols.
namespace openads::sql_backend {}

#else

#include <ibase.h>

#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <mutex>
#include <set>
#include <string>
#include <vector>

namespace openads::sql_backend {

namespace {

constexpr int kSqlDialect = SQL_DIALECT_V6;

// ---- error helpers -------------------------------------------------------

util::Error fb_error(const char* context, const ISC_STATUS* status) {
    std::string detail;
    char        buf[512];
    const ISC_STATUS* p = status;
    while (fb_interpret(buf, sizeof(buf), &p)) {
        if (!detail.empty()) detail += "; ";
        detail += buf;
    }
    std::string msg = context ? context : "firebird";
    if (!detail.empty()) {
        msg += ": ";
        msg += detail;
    }
    return util::Error{5001, 0, msg, ""};
}

bool status_failed(const ISC_STATUS* st) {
    return st[0] == 1 && st[1] != 0;
}

// ---- identifier / literal helpers ---------------------------------------

std::string to_upper(const std::string& s) {
    std::string o = s;
    for (char& c : o) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return o;
}

// Firebird folds unquoted identifiers to upper case; legacy / Comix tables
// are stored upper case. Quote the folded name so reserved words and the
// catalog name agree.
std::string quote_ident(const std::string& name) {
    return '"' + name + '"';
}

std::string escape_literal(const std::string& value) {
    std::string out = "'";
    for (char c : value) {
        if (c == '\'') out += "''";
        else           out += c;
    }
    out += '\'';
    return out;
}

bool is_numeric_literal(const std::string& s) {
    if (s.empty()) return false;
    bool any_digit = false;
    for (char c : s) {
        if (c >= '0' && c <= '9') { any_digit = true; continue; }
        if (c == '+' || c == '-' || c == '.' || c == 'e' || c == 'E') continue;
        return false;
    }
    return any_digit;
}

std::size_t field_index_ci(const FirebirdTable& tbl, const std::string& name) {
    const std::string want = to_upper(name);
    for (std::size_t i = 0; i < tbl.fields.size(); ++i) {
        if (to_upper(tbl.fields[i].name) == want) return i;
    }
    return static_cast<std::size_t>(-1);
}

// Emit a SQL literal for `value` against `column`: bare for numeric columns
// (when the value is a clean number), single-quoted otherwise.
std::string format_literal(const FirebirdTable& tbl, const std::string& column,
                           const std::string& value) {
    const std::size_t idx = field_index_ci(tbl, column);
    const bool numeric = idx != static_cast<std::size_t>(-1) &&
        (tbl.fields[idx].type == ADS_INTEGER ||
         tbl.fields[idx].type == ADS_DOUBLE);
    if (numeric && is_numeric_literal(value)) return value;
    return escape_literal(value);
}

std::string index_column_sql(const std::string& column, IndexExprKind kind) {
    const std::string qcol = quote_ident(column);
    return kind == IndexExprKind::UpperColumn ? "UPPER(" + qcol + ")" : qcol;
}

std::string pk_select_list(const FirebirdTable& tbl) {
    std::string out;
    for (std::size_t i = 0; i < tbl.pk_columns.size(); ++i) {
        if (i > 0) out += ", ";
        out += quote_ident(tbl.pk_columns[i]);
    }
    return out;
}

std::string pk_where_clause(const FirebirdTable& tbl,
                            const FirebirdTable::PkRow& pk) {
    std::string out;
    for (std::size_t i = 0; i < tbl.pk_columns.size(); ++i) {
        if (i > 0) out += " AND ";
        out += quote_ident(tbl.pk_columns[i]) + " = " +
               format_literal(tbl, tbl.pk_columns[i], pk.values[i]);
    }
    return out;
}

// ---- ADS type mapping ----------------------------------------------------

// Firebird RDB$FIELD_TYPE codes.
constexpr int kFbShort     = 7;
constexpr int kFbLong      = 8;
constexpr int kFbFloat     = 10;
constexpr int kFbDate      = 12;   // legacy DATE (== TIMESTAMP in dialect 1)
constexpr int kFbTime      = 13;
constexpr int kFbText      = 14;   // CHAR
constexpr int kFbInt64     = 16;
constexpr int kFbInt128    = 26;
constexpr int kFbDouble    = 27;
constexpr int kFbTimestamp = 35;
constexpr int kFbVarying   = 37;   // VARCHAR
constexpr int kFbBlob      = 261;
constexpr int kFbBoolean   = 23;

FirebirdTable::FieldDesc map_firebird_column(const std::string& name,
                                             int field_type,
                                             int /*sub_type*/,
                                             int char_length,
                                             int field_scale,
                                             bool nullable) {
    FirebirdTable::FieldDesc fd;
    fd.name     = name;
    fd.nullable = nullable;

    switch (field_type) {
        case kFbShort:
        case kFbLong:
        case kFbInt64:
        case kFbInt128:
            if (field_scale < 0) {
                // NUMERIC / DECIMAL — scaled exact numeric.
                fd.type     = ADS_DOUBLE;
                fd.length   = 8;
                fd.decimals = static_cast<std::uint16_t>(-field_scale);
            } else {
                fd.type     = ADS_INTEGER;
                fd.length   = 4;
                fd.decimals = 0;
            }
            break;
        case kFbFloat:
        case kFbDouble:
            fd.type     = ADS_DOUBLE;
            fd.length   = 8;
            fd.decimals = 6;
            break;
        case kFbBoolean:
            fd.type     = ADS_LOGICAL;
            fd.length   = 1;
            fd.decimals = 0;
            break;
        case kFbDate:
        case kFbTime:
        case kFbTimestamp:
            fd.type     = ADS_DATE;
            fd.length   = 8;
            fd.decimals = 0;
            break;
        case kFbBlob:
            fd.type     = ADS_MEMO;
            fd.length   = 10;
            fd.decimals = 0;
            break;
        case kFbText:
        case kFbVarying:
        default:
            fd.type     = ADS_STRING;
            fd.length   = char_length > 0
                              ? static_cast<std::uint32_t>(char_length)
                              : 64;
            fd.decimals = 0;
            break;
    }
    return fd;
}

// ---- DSQL value formatting ----------------------------------------------

// Render a scaled exact-numeric integer (NUMERIC/DECIMAL) as decimal text.
std::string scaled_to_string(long long raw, int scale) {
    if (scale >= 0) return std::to_string(raw);
    const int digits = -scale;
    const bool neg = raw < 0;
    unsigned long long mag = neg ? static_cast<unsigned long long>(-(raw + 1)) + 1ULL
                                 : static_cast<unsigned long long>(raw);
    std::string s = std::to_string(mag);
    if (static_cast<int>(s.size()) <= digits) {
        s.insert(0, digits - s.size() + 1, '0');
    }
    s.insert(s.size() - digits, ".");
    if (neg) s.insert(0, "-");
    return s;
}

std::string fmt_double(double d) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.10g", d);
    return buf;
}

std::string format_xsqlvar(const XSQLVAR& v, bool& is_null) {
    is_null = (v.sqltype & 1) && v.sqlind && (*v.sqlind < 0);
    if (is_null) return std::string();

    const int dtype = v.sqltype & ~1;
    switch (dtype) {
        case SQL_TEXT: {
            // CHAR: fixed width, right-trim trailing spaces.
            int n = v.sqllen;
            while (n > 0 && v.sqldata[n - 1] == ' ') --n;
            return std::string(v.sqldata, static_cast<std::size_t>(n));
        }
        case SQL_VARYING: {
            short len = 0;
            std::memcpy(&len, v.sqldata, sizeof(short));
            return std::string(v.sqldata + 2, static_cast<std::size_t>(len));
        }
        case SQL_SHORT: {
            short n = 0;
            std::memcpy(&n, v.sqldata, sizeof(n));
            return scaled_to_string(n, v.sqlscale);
        }
        case SQL_LONG: {
            ISC_LONG n = 0;
            std::memcpy(&n, v.sqldata, sizeof(n));
            return scaled_to_string(n, v.sqlscale);
        }
        case SQL_INT64: {
            ISC_INT64 n = 0;
            std::memcpy(&n, v.sqldata, sizeof(n));
            return scaled_to_string(static_cast<long long>(n), v.sqlscale);
        }
        case SQL_FLOAT: {
            float f = 0;
            std::memcpy(&f, v.sqldata, sizeof(f));
            return fmt_double(static_cast<double>(f));
        }
        case SQL_DOUBLE: {
            double d = 0;
            std::memcpy(&d, v.sqldata, sizeof(d));
            return fmt_double(d);
        }
        case SQL_TYPE_DATE: {
            ISC_DATE raw = 0;
            std::memcpy(&raw, v.sqldata, sizeof(raw));
            struct tm t {};
            isc_decode_sql_date(&raw, &t);
            char b[16];
            std::snprintf(b, sizeof(b), "%04d-%02d-%02d",
                          t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
            return b;
        }
        case SQL_TYPE_TIME: {
            ISC_TIME raw = 0;
            std::memcpy(&raw, v.sqldata, sizeof(raw));
            struct tm t {};
            isc_decode_sql_time(&raw, &t);
            char b[16];
            std::snprintf(b, sizeof(b), "%02d:%02d:%02d",
                          t.tm_hour, t.tm_min, t.tm_sec);
            return b;
        }
        case SQL_TIMESTAMP: {
            ISC_TIMESTAMP raw {};
            std::memcpy(&raw, v.sqldata, sizeof(raw));
            struct tm t {};
            isc_decode_timestamp(&raw, &t);
            char b[32];
            std::snprintf(b, sizeof(b), "%04d-%02d-%02d %02d:%02d:%02d",
                          t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                          t.tm_hour, t.tm_min, t.tm_sec);
            return b;
        }
        case SQL_BOOLEAN: {
            signed char b = 0;
            std::memcpy(&b, v.sqldata, sizeof(b));
            return b ? "true" : "false";
        }
        default:
            // BLOB / unsupported: surface an empty placeholder rather than
            // raw bytes (the read path never returns binary garbage).
            return std::string();
    }
}

// ---- DSQL query execution -----------------------------------------------

struct SqlRows {
    std::vector<std::string>              col_names;
    std::vector<std::vector<std::string>> rows;
    std::vector<std::vector<bool>>        nulls;
};

XSQLDA* alloc_xsqlda(int n) {
    XSQLDA* p = static_cast<XSQLDA*>(std::calloc(1, XSQLDA_LENGTH(n)));
    p->version = SQLDA_VERSION1;
    p->sqln    = static_cast<short>(n);
    return p;
}

void free_xsqlda_buffers(XSQLDA* sqlda) {
    if (!sqlda) return;
    for (int i = 0; i < sqlda->sqld; ++i) {
        std::free(sqlda->sqlvar[i].sqldata);
        std::free(sqlda->sqlvar[i].sqlind);
        sqlda->sqlvar[i].sqldata = nullptr;
        sqlda->sqlvar[i].sqlind  = nullptr;
    }
}

// Run a SELECT and materialise all rows as strings (nulls flagged).
util::Result<SqlRows> exec_query(isc_db_handle* db, isc_tr_handle* tr,
                                 const std::string& sql) {
    ISC_STATUS_ARRAY st;
    isc_stmt_handle  stmt = 0;
    if (isc_dsql_allocate_statement(st, db, &stmt); status_failed(st)) {
        return fb_error("firebird alloc stmt", st);
    }

    XSQLDA* out = alloc_xsqlda(20);
    auto teardown = [&]() {
        free_xsqlda_buffers(out);
        std::free(out);
        if (stmt) {
            ISC_STATUS_ARRAY s2;
            isc_dsql_free_statement(s2, &stmt, DSQL_drop);
        }
    };

    isc_dsql_prepare(st, tr, &stmt, 0, sql.c_str(), kSqlDialect, out);
    if (status_failed(st)) { auto e = fb_error("firebird prepare", st); teardown(); return e; }

    if (out->sqld > out->sqln) {
        const int n = out->sqld;
        std::free(out);
        out = alloc_xsqlda(n);
        isc_dsql_describe(st, &stmt, 1, out);
        if (status_failed(st)) { auto e = fb_error("firebird describe", st); teardown(); return e; }
    }

    // Allocate per-column buffers; force the null indicator on every column.
    for (int i = 0; i < out->sqld; ++i) {
        XSQLVAR& v = out->sqlvar[i];
        const int dtype = v.sqltype & ~1;
        int len = v.sqllen;
        if (dtype == SQL_VARYING) len += 2;
        v.sqldata = static_cast<char*>(std::malloc(static_cast<std::size_t>(len) + 1));
        v.sqltype |= 1;
        v.sqlind = static_cast<short*>(std::malloc(sizeof(short)));
    }

    isc_dsql_execute(st, tr, &stmt, kSqlDialect, nullptr);
    if (status_failed(st)) { auto e = fb_error("firebird execute", st); teardown(); return e; }

    SqlRows res;
    res.col_names.reserve(static_cast<std::size_t>(out->sqld));
    for (int i = 0; i < out->sqld; ++i) {
        const XSQLVAR& v = out->sqlvar[i];
        const int nl = v.aliasname_length > 0 ? v.aliasname_length : v.sqlname_length;
        res.col_names.emplace_back(v.aliasname, static_cast<std::size_t>(nl));
    }

    while (true) {
        const ISC_STATUS fetch = isc_dsql_fetch(st, &stmt, kSqlDialect, out);
        if (fetch == 100L) break;          // end of cursor
        if (fetch != 0) { auto e = fb_error("firebird fetch", st); teardown(); return e; }
        std::vector<std::string> row;
        std::vector<bool>        rn;
        row.reserve(static_cast<std::size_t>(out->sqld));
        rn.reserve(static_cast<std::size_t>(out->sqld));
        for (int i = 0; i < out->sqld; ++i) {
            bool is_null = false;
            row.push_back(format_xsqlvar(out->sqlvar[i], is_null));
            rn.push_back(is_null);
        }
        res.rows.push_back(std::move(row));
        res.nulls.push_back(std::move(rn));
    }

    teardown();
    return res;
}

// Run a statement that yields no result set (INSERT / UPDATE / DELETE / DDL).
util::Result<void> exec_dml(isc_db_handle* db, isc_tr_handle* tr,
                            const std::string& sql) {
    ISC_STATUS_ARRAY st;
    isc_dsql_execute_immediate(st, db, tr, 0, sql.c_str(), kSqlDialect, nullptr);
    if (status_failed(st)) return fb_error("firebird dml", st);
    return util::Result<void>{};
}

} // namespace

// ---- Impl ----------------------------------------------------------------

struct FirebirdConnection::Impl {
    isc_db_handle         db = 0;
    isc_tr_handle         tr = 0;
    mutable std::mutex    mu;
    bool                  lock_table_ready = false;
    std::set<std::string> held_locks;       // OPENADS$LOCKS keys this conn holds
};

FirebirdConnection::FirebirdConnection() = default;
FirebirdConnection::~FirebirdConnection() { disconnect(); }

FirebirdConnection::FirebirdConnection(FirebirdConnection&& other) noexcept
    : impl_(std::move(other.impl_)) {}

FirebirdConnection& FirebirdConnection::operator=(FirebirdConnection&& other) noexcept {
    if (this != &other) {
        disconnect();
        impl_ = std::move(other.impl_);
    }
    return *this;
}

namespace {

// Build a DPB (database parameter buffer) for isc_attach_database.
std::vector<char> build_dpb(const FirebirdUri& uri) {
    std::vector<char> dpb;
    dpb.push_back(static_cast<char>(isc_dpb_version1));
    auto add = [&](char tag, const std::string& val) {
        if (val.empty()) return;
        dpb.push_back(tag);
        dpb.push_back(static_cast<char>(val.size()));
        dpb.insert(dpb.end(), val.begin(), val.end());
    };
    const std::string user = uri.user.empty() ? std::string("SYSDBA") : uri.user;
    add(static_cast<char>(isc_dpb_user_name), user);
    add(static_cast<char>(isc_dpb_password), uri.password);
    add(static_cast<char>(isc_dpb_lc_ctype),
        uri.charset.empty() ? std::string("UTF8") : uri.charset);
    add(static_cast<char>(isc_dpb_sql_role_name), uri.role);
    return dpb;
}

// Read-committed read-write transaction: writes commit_retaining and the
// re-counts after them see the new data.
util::Result<void> start_tx(isc_db_handle* db, isc_tr_handle* tr) {
    static const char tpb[] = {
        isc_tpb_version3,
        isc_tpb_write,
        isc_tpb_read_committed,
        isc_tpb_rec_version,
        isc_tpb_wait,
    };
    ISC_STATUS_ARRAY st;
    isc_start_transaction(st, tr, 1, db,
                          static_cast<short>(sizeof(tpb)),
                          const_cast<char*>(tpb));
    if (status_failed(st)) return fb_error("firebird start tx", st);
    return util::Result<void>{};
}

// ---- rLock()/fLock() emulation via a lock table -------------------------
//
// Firebird has no advisory-lock primitive (no GET_LOCK / pg_advisory_lock), so
// mutual exclusion across attachments is built on a tiny lock table whose only
// column is a unique key: INSERT acquires, DELETE releases. Each op runs in its
// own short transaction so the row is committed (and therefore visible to other
// attachments) immediately, and the long-lived data transaction is never
// touched. The key is an FNV-1a hash of the logical lock name, bounded to 16
// hex chars so it always fits the index and needs no escaping.

constexpr int kFbDuplicateKeySqlCode = -803;  // duplicate value in unique index
constexpr int kFbTableExistsSqlCode  = -607;  // metadata: object already exists

std::string fb_hash_key(const std::string& s) {
    std::uint64_t h = 1469598103934665603ULL;       // FNV-1a 64-bit
    for (unsigned char c : s) { h ^= c; h *= 1099511628211ULL; }
    static const char* hex = "0123456789abcdef";
    char buf[17];
    for (int i = 15; i >= 0; --i) { buf[i] = hex[h & 0xF]; h >>= 4; }
    return std::string(buf, 16);
}

std::string fb_record_lock_key(const FirebirdTable& tbl, std::size_t pos) {
    std::string k = "R\x1f" + tbl.name;
    if (pos < tbl.pk_snapshot.size()) {
        for (const std::string& v : tbl.pk_snapshot[pos].values) k += "\x1f" + v;
    }
    return fb_hash_key(k);
}

std::string fb_table_lock_key(const FirebirdTable& tbl) {
    return fb_hash_key("T\x1f" + tbl.name);
}

// Create OPENADS$LOCKS if absent. Idempotent and race-safe: a concurrent
// attachment that already created it surfaces as -607, which we accept.
util::Result<void> fb_ensure_lock_table(isc_db_handle* db) {
    isc_tr_handle tr = 0;
    if (auto t = start_tx(db, &tr); !t) return t.error();
    ISC_STATUS_ARRAY st;
    isc_dsql_execute_immediate(
        st, db, &tr, 0,
        "CREATE TABLE OPENADS$LOCKS ("
        "LOCK_KEY VARCHAR(16) CHARACTER SET OCTETS NOT NULL PRIMARY KEY)",
        kSqlDialect, nullptr);
    if (status_failed(st)) {
        const ISC_LONG code = isc_sqlcode(st);
        ISC_STATUS_ARRAY s2;
        isc_rollback_transaction(s2, &tr);
        if (code == kFbTableExistsSqlCode) return util::Result<void>{};
        return fb_error("firebird create lock table", st);
    }
    isc_commit_transaction(st, &tr);
    if (status_failed(st)) return fb_error("firebird commit lock table", st);
    return util::Result<void>{};
}

// Returns true when the key was acquired, false when another attachment holds
// it (duplicate-key), or an error for anything else. `key` is hex, so it embeds
// in the statement literal with no escaping.
util::Result<bool> fb_lock_insert(isc_db_handle* db, const std::string& key) {
    isc_tr_handle tr = 0;
    if (auto t = start_tx(db, &tr); !t) return t.error();
    const std::string sql =
        "INSERT INTO OPENADS$LOCKS (LOCK_KEY) VALUES ('" + key + "')";
    ISC_STATUS_ARRAY st;
    isc_dsql_execute_immediate(st, db, &tr, 0, sql.c_str(), kSqlDialect, nullptr);
    if (status_failed(st)) {
        const ISC_LONG code = isc_sqlcode(st);
        ISC_STATUS_ARRAY s2;
        isc_rollback_transaction(s2, &tr);
        if (code == kFbDuplicateKeySqlCode) return false;  // already locked
        return fb_error("firebird lock insert", st);
    }
    isc_commit_transaction(st, &tr);
    if (status_failed(st)) return fb_error("firebird lock commit", st);
    return true;
}

util::Result<void> fb_lock_delete(isc_db_handle* db, const std::string& key) {
    isc_tr_handle tr = 0;
    if (auto t = start_tx(db, &tr); !t) return t.error();
    const std::string sql =
        "DELETE FROM OPENADS$LOCKS WHERE LOCK_KEY = '" + key + "'";
    ISC_STATUS_ARRAY st;
    isc_dsql_execute_immediate(st, db, &tr, 0, sql.c_str(), kSqlDialect, nullptr);
    if (status_failed(st)) {
        auto e = fb_error("firebird unlock delete", st);
        ISC_STATUS_ARRAY s2;
        isc_rollback_transaction(s2, &tr);
        return e;
    }
    isc_commit_transaction(st, &tr);
    if (status_failed(st)) return fb_error("firebird unlock commit", st);
    return util::Result<void>{};
}

} // namespace

util::Result<FirebirdConnection> FirebirdConnection::open(const FirebirdUri& uri) {
    FirebirdConnection conn;
    conn.impl_ = std::make_unique<Impl>();

    const std::string attach = uri.attach_string();
    std::vector<char> dpb = build_dpb(uri);

    ISC_STATUS_ARRAY st;
    isc_attach_database(st, static_cast<short>(attach.size()),
                        const_cast<char*>(attach.c_str()),
                        &conn.impl_->db,
                        static_cast<short>(dpb.size()), dpb.data());
    if (status_failed(st)) return fb_error("firebird attach", st);

    if (auto t = start_tx(&conn.impl_->db, &conn.impl_->tr); !t) {
        ISC_STATUS_ARRAY s2;
        isc_detach_database(s2, &conn.impl_->db);
        return t.error();
    }
    return std::move(conn);
}

void FirebirdConnection::disconnect() noexcept {
    if (!impl_) return;
    std::lock_guard<std::mutex> lk(impl_->mu);
    // Release any locks this connection still holds so they do not orphan in
    // OPENADS$LOCKS (best-effort: a hard crash cannot run this, advisory-lock
    // backends auto-release on session end where Firebird cannot).
    if (impl_->db) {
        for (const std::string& key : impl_->held_locks) {
            fb_lock_delete(&impl_->db, key);
        }
        impl_->held_locks.clear();
    }
    ISC_STATUS_ARRAY st;
    if (impl_->tr) {
        isc_commit_transaction(st, &impl_->tr);
        impl_->tr = 0;
    }
    if (impl_->db) {
        isc_detach_database(st, &impl_->db);
        impl_->db = 0;
    }
    impl_.reset();
}

bool FirebirdConnection::valid() const noexcept {
    return impl_ && impl_->db != 0;
}

namespace {

util::Result<std::vector<std::string>>
discover_pk(isc_db_handle* db, isc_tr_handle* tr, const std::string& sql_table) {
    const std::string sql =
        "SELECT TRIM(s.RDB$FIELD_NAME) "
        "FROM RDB$RELATION_CONSTRAINTS rc "
        "JOIN RDB$INDEX_SEGMENTS s ON s.RDB$INDEX_NAME = rc.RDB$INDEX_NAME "
        "WHERE rc.RDB$RELATION_NAME = " + escape_literal(sql_table) +
        " AND rc.RDB$CONSTRAINT_TYPE = 'PRIMARY KEY' "
        "ORDER BY s.RDB$FIELD_POSITION";
    auto r = exec_query(db, tr, sql);
    if (!r) return r.error();
    std::vector<std::string> cols;
    for (auto& row : r.value().rows) {
        if (!row.empty() && !row[0].empty()) cols.push_back(row[0]);
    }
    if (cols.empty()) {
        return util::Error{5001, 0,
            "table has no primary key (Firebird backend navigates by a "
            "stable key)", sql_table};
    }
    return cols;
}

util::Result<void> describe_columns(isc_db_handle* db, isc_tr_handle* tr,
                                    FirebirdTable* tbl) {
    const std::string sql =
        "SELECT TRIM(rf.RDB$FIELD_NAME), f.RDB$FIELD_TYPE, "
        "f.RDB$FIELD_SUB_TYPE, f.RDB$CHARACTER_LENGTH, f.RDB$FIELD_SCALE, "
        "COALESCE(rf.RDB$NULL_FLAG, 0) "
        "FROM RDB$RELATION_FIELDS rf "
        "JOIN RDB$FIELDS f ON rf.RDB$FIELD_SOURCE = f.RDB$FIELD_NAME "
        "WHERE rf.RDB$RELATION_NAME = " + escape_literal(tbl->sql_table) +
        " ORDER BY rf.RDB$FIELD_POSITION";
    auto r = exec_query(db, tr, sql);
    if (!r) return r.error();
    const auto& rows = r.value().rows;
    if (rows.empty()) {
        return util::Error{5001, 0, "table not found or has no columns",
                           tbl->name};
    }
    std::vector<FirebirdTable::FieldDesc> out;
    out.reserve(rows.size());
    for (const auto& row : rows) {
        auto cell = [&](std::size_t i) -> std::string {
            return i < row.size() ? row[i] : std::string();
        };
        const std::string cname  = cell(0);
        const int ftype = std::atoi(cell(1).c_str());
        const int subt  = std::atoi(cell(2).c_str());
        const int clen  = std::atoi(cell(3).c_str());
        const int scale = std::atoi(cell(4).c_str());
        const int nflag = std::atoi(cell(5).c_str());  // 1 => NOT NULL
        out.push_back(map_firebird_column(cname, ftype, subt, clen, scale,
                                          nflag != 1));
    }
    tbl->fields        = std::move(out);
    tbl->fields_cached = true;
    return util::Result<void>{};
}

util::Result<void> load_pk_snapshot(isc_db_handle* db, isc_tr_handle* tr,
                                    FirebirdTable* tbl) {
    const std::string list = pk_select_list(*tbl);
    const std::string sql =
        "SELECT " + list + " FROM " + quote_ident(tbl->sql_table) +
        " ORDER BY " + list;
    auto r = exec_query(db, tr, sql);
    if (!r) return r.error();
    tbl->pk_snapshot.clear();
    tbl->pk_snapshot.reserve(r.value().rows.size());
    for (auto& row : r.value().rows) {
        FirebirdTable::PkRow pk;
        pk.values = std::move(row);
        tbl->pk_snapshot.push_back(std::move(pk));
    }
    return util::Result<void>{};
}

util::Result<void> load_result_row(FirebirdTable* tbl, std::size_t idx) {
    if (idx >= tbl->result_rows.size()) {
        tbl->positioned    = false;
        tbl->row_valid     = false;
        tbl->pos           = tbl->result_rows.size();
        tbl->current_recno = 0;
        return util::Result<void>{};
    }
    tbl->pos           = idx;
    tbl->current_row   = tbl->result_rows[idx];
    tbl->current_nulls = tbl->result_nulls[idx];
    tbl->current_recno = static_cast<std::uint32_t>(idx + 1);
    tbl->positioned    = true;
    tbl->row_valid     = true;
    return util::Result<void>{};
}

util::Result<void> load_current_row(isc_db_handle* db, isc_tr_handle* tr,
                                    FirebirdTable* tbl, std::size_t idx) {
    if (tbl->is_result) return load_result_row(tbl, idx);
    if (idx >= tbl->pk_snapshot.size()) {
        tbl->positioned = false;
        tbl->row_valid  = false;
        return util::Result<void>{};
    }
    const std::string sql =
        "SELECT * FROM " + quote_ident(tbl->sql_table) + " WHERE " +
        pk_where_clause(*tbl, tbl->pk_snapshot[idx]);
    auto r = exec_query(db, tr, sql);
    if (!r) return r.error();
    auto& rows = r.value().rows;
    if (rows.empty()) {
        tbl->positioned = false;
        tbl->row_valid  = false;
        return util::Result<void>{};
    }
    tbl->current_row   = std::move(rows[0]);
    tbl->current_nulls = std::move(r.value().nulls[0]);
    tbl->pos           = idx;
    tbl->current_recno = static_cast<std::uint32_t>(idx + 1);
    tbl->positioned    = true;
    tbl->row_valid     = true;
    tbl->row_dirty     = false;
    tbl->pending_append = false;
    return util::Result<void>{};
}

util::Result<void> ensure_staging(FirebirdTable* tbl) {
    if (!tbl->fields_cached) {
        return util::Error{5001, 0, "schema not cached", ""};
    }
    if (tbl->staging_row.size() != tbl->fields.size()) {
        tbl->staging_row.assign(tbl->fields.size(), "");
        tbl->staging_nulls.assign(tbl->fields.size(), true);
    }
    return util::Result<void>{};
}

util::Result<void> sync_staging_from_current(FirebirdTable* tbl) {
    if (auto r = ensure_staging(tbl); !r) return r.error();
    if (tbl->row_valid) {
        tbl->staging_row   = tbl->current_row;
        tbl->staging_nulls = tbl->current_nulls;
    } else {
        for (std::size_t i = 0; i < tbl->fields.size(); ++i) {
            tbl->staging_row[i].clear();
            tbl->staging_nulls[i] = true;
        }
    }
    tbl->row_dirty = false;
    return util::Result<void>{};
}

} // namespace

util::Result<std::unique_ptr<FirebirdTable>>
FirebirdConnection::open_table(const std::string& table_name) {
    if (!impl_) return util::Error{5001, 0, "firebird connection not open", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid()) return util::Error{5001, 0, "firebird connection not open", ""};
    if (!is_safe_identifier(table_name)) {
        return util::Error{5001, 0, "invalid table name", table_name};
    }
    auto tbl       = std::make_unique<FirebirdTable>();
    tbl->conn      = this;
    tbl->name      = table_name;
    tbl->sql_table = to_upper(table_name);

    auto pk = discover_pk(&impl_->db, &impl_->tr, tbl->sql_table);
    if (!pk) return pk.error();
    tbl->pk_columns = std::move(pk).value();

    if (auto d = describe_columns(&impl_->db, &impl_->tr, tbl.get()); !d) {
        return d.error();
    }
    if (auto s = load_pk_snapshot(&impl_->db, &impl_->tr, tbl.get()); !s) {
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

util::Result<void> FirebirdConnection::goto_top(FirebirdTable* tbl) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird goto_top", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird goto_top", ""};
    }
    if (tbl->cached_rec_count == 0) {
        tbl->positioned = false; tbl->row_valid = false;
        tbl->current_recno = 0; tbl->pos = 0;
        return util::Result<void>{};
    }
    return load_current_row(&impl_->db, &impl_->tr, tbl, 0);
}

util::Result<void> FirebirdConnection::goto_bottom(FirebirdTable* tbl) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird goto_bottom", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird goto_bottom", ""};
    }
    if (tbl->cached_rec_count == 0) {
        tbl->positioned = false; tbl->row_valid = false;
        tbl->current_recno = 0; tbl->pos = 0;
        return util::Result<void>{};
    }
    return load_current_row(&impl_->db, &impl_->tr, tbl,
                            tbl->cached_rec_count - 1);
}

util::Result<void> FirebirdConnection::skip(FirebirdTable* tbl, std::int32_t step) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird skip", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird skip", ""};
    }
    if (step == 0) return util::Result<void>{};
    if (tbl->cached_rec_count == 0) {
        tbl->positioned = false; tbl->row_valid = false; tbl->pos = 0;
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
        tbl->positioned = false; tbl->row_valid = false; tbl->pos = 0;
        return util::Error{5026, 0, "bof", ""};
    }
    if (static_cast<std::uint32_t>(next) >= tbl->cached_rec_count) {
        tbl->positioned = false; tbl->row_valid = false;
        tbl->pos = tbl->cached_rec_count;
        return util::Result<void>{};
    }
    return load_current_row(&impl_->db, &impl_->tr, tbl,
                            static_cast<std::size_t>(next));
}

util::Result<bool> FirebirdConnection::at_eof(FirebirdTable* tbl) const {
    if (!impl_) return util::Error{5001, 0, "invalid firebird at_eof", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird at_eof", ""};
    }
    if (tbl->cached_rec_count == 0) return true;
    if (!tbl->positioned && tbl->pos >= tbl->cached_rec_count) return true;
    return false;
}

util::Result<bool> FirebirdConnection::at_bof(FirebirdTable* tbl) const {
    if (!impl_) return util::Error{5001, 0, "invalid firebird at_bof", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird at_bof", ""};
    }
    if (tbl->cached_rec_count == 0) return true;
    return !tbl->positioned && tbl->pos == 0;
}

util::Result<std::uint32_t> FirebirdConnection::record_count(FirebirdTable* tbl) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird record_count", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird record_count", ""};
    }
    if (tbl->rec_count_cached) return tbl->cached_rec_count;
    tbl->cached_rec_count = static_cast<std::uint32_t>(tbl->pk_snapshot.size());
    tbl->rec_count_cached = true;
    return tbl->cached_rec_count;
}

util::Result<std::vector<FirebirdTable::FieldDesc>>
FirebirdConnection::describe_table(FirebirdTable* tbl) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird describe_table", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird describe_table", ""};
    }
    if (tbl->fields_cached) return tbl->fields;
    if (auto d = describe_columns(&impl_->db, &impl_->tr, tbl); !d) return d.error();
    return tbl->fields;
}

util::Result<void> FirebirdConnection::read_field(
    FirebirdTable* tbl, const std::string& field_name,
    std::string& buf, bool& is_null) const {
    if (!impl_) return util::Error{5001, 0, "invalid firebird read_field", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird read_field", ""};
    }
    if (!tbl->fields_cached) return util::Error{5001, 0, "schema not cached", ""};
    if (!tbl->row_valid && !tbl->pending_append) {
        return util::Error{5026, 0, "no current record", ""};
    }
    const std::size_t idx = field_index_ci(*tbl, field_name);
    if (idx == static_cast<std::size_t>(-1)) {
        return util::Error{5063, 0, "column not found", field_name};
    }
    const bool use_staging = tbl->pending_append || tbl->row_dirty;
    if (use_staging) {
        if (idx >= tbl->staging_row.size()) {
            return util::Error{5001, 0, "staging mismatch", ""};
        }
        is_null = tbl->staging_nulls[idx];
        buf     = tbl->staging_row[idx];
        return util::Result<void>{};
    }
    if (idx >= tbl->current_row.size()) {
        return util::Error{5001, 0, "row cache mismatch", ""};
    }
    is_null = tbl->current_nulls[idx];
    buf     = tbl->current_row[idx];
    return util::Result<void>{};
}

util::Result<bool> FirebirdConnection::seek_index(
    FirebirdTable* tbl, const std::string& column, IndexExprKind kind,
    const std::string& key, bool soft, bool last_key) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird seek", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird seek", ""};
    }
    if (!is_safe_identifier(column)) {
        return util::Error{5001, 0, "invalid seek column", column};
    }
    if (!tbl->fields_cached) {
        if (auto d = describe_columns(&impl_->db, &impl_->tr, tbl); !d) return d.error();
    }
    const std::size_t col_idx = field_index_ci(*tbl, column);
    if (col_idx == static_cast<std::size_t>(-1)) {
        return util::Error{5063, 0, "seek column not found", column};
    }
    const std::string& sql_col = tbl->fields[col_idx].name;

    const std::string pkcols = pk_select_list(*tbl);
    const std::string esc     = (kind == IndexExprKind::UpperColumn)
                                    ? escape_literal(key)
                                    : format_literal(*tbl, sql_col, key);
    const std::string qexpr   = index_column_sql(sql_col, kind);
    const std::string from    = " FROM " + quote_ident(tbl->sql_table);

    // ROWS 1 caps the result to the first match (Firebird's LIMIT).
    std::string sql;
    if (last_key) {
        sql = soft
            ? "SELECT " + pkcols + from + " WHERE " + qexpr + " <= " + esc +
              " ORDER BY " + qexpr + " DESC ROWS 1"
            : "SELECT " + pkcols + from + " WHERE " + qexpr + " = " + esc +
              " ORDER BY " + qexpr + " DESC ROWS 1";
    } else {
        sql = soft
            ? "SELECT " + pkcols + from + " WHERE " + qexpr + " >= " + esc +
              " ORDER BY " + qexpr + " ASC ROWS 1"
            : "SELECT " + pkcols + from + " WHERE " + qexpr + " = " + esc +
              " ROWS 1";
    }

    auto r = exec_query(&impl_->db, &impl_->tr, sql);
    if (!r) return r.error();

    bool found = false;
    if (!r.value().rows.empty()) {
        FirebirdTable::PkRow pk;
        pk.values = r.value().rows[0];
        std::size_t pos = static_cast<std::size_t>(-1);
        for (std::size_t i = 0; i < tbl->pk_snapshot.size(); ++i) {
            if (tbl->pk_snapshot[i] == pk) { pos = i; break; }
        }
        if (pos != static_cast<std::size_t>(-1)) {
            if (auto lr = load_current_row(&impl_->db, &impl_->tr, tbl, pos); !lr) {
                return lr.error();
            }
            found = tbl->positioned && tbl->row_valid;
        } else {
            tbl->positioned = false; tbl->row_valid = false; found = false;
        }
    } else {
        tbl->positioned = false; tbl->row_valid = false; found = false;
    }
    tbl->last_seek_found = found;
    return found;
}

util::Result<void> FirebirdConnection::append_blank(FirebirdTable* tbl) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird append", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird append", ""};
    }
    if (!tbl->fields_cached) {
        if (auto d = describe_columns(&impl_->db, &impl_->tr, tbl); !d) return d.error();
    }
    if (auto s = ensure_staging(tbl); !s) return s.error();
    for (std::size_t i = 0; i < tbl->fields.size(); ++i) {
        tbl->staging_row[i].clear();
        tbl->staging_nulls[i] = true;
    }
    tbl->pending_append   = true;
    tbl->row_dirty        = true;
    tbl->row_valid        = true;
    tbl->positioned       = true;
    tbl->current_recno    = 0;
    return util::Result<void>{};
}

util::Result<void> FirebirdConnection::set_field(
    FirebirdTable* tbl, const std::string& field_name,
    const std::string& value) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird set_field", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird set_field", ""};
    }
    if (!tbl->row_valid && !tbl->pending_append) {
        return util::Error{5026, 0, "no current record", ""};
    }
    if (!tbl->fields_cached) return util::Error{5001, 0, "schema not cached", ""};
    const std::size_t idx = field_index_ci(*tbl, field_name);
    if (idx == static_cast<std::size_t>(-1)) {
        return util::Error{5063, 0, "column not found", field_name};
    }
    if (auto s = ensure_staging(tbl); !s) return s.error();
    if (!tbl->row_dirty && !tbl->pending_append) {
        if (auto sync = sync_staging_from_current(tbl); !sync) return sync.error();
    }
    tbl->staging_row[idx]   = value;
    tbl->staging_nulls[idx] = false;
    tbl->row_dirty          = true;
    return util::Result<void>{};
}

util::Result<void> FirebirdConnection::flush_record(FirebirdTable* tbl) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird flush", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird flush", ""};
    }
    if (!tbl->row_dirty && !tbl->pending_append) return util::Result<void>{};
    if (!tbl->fields_cached) return util::Error{5001, 0, "schema not cached", ""};
    if (auto s = ensure_staging(tbl); !s) return s.error();

    auto literal_for = [&](std::size_t i) -> std::string {
        if (tbl->staging_nulls[i]) return "NULL";
        return format_literal(*tbl, tbl->fields[i].name, tbl->staging_row[i]);
    };

    if (tbl->pending_append) {
        std::string cols, vals;
        bool any = false;
        for (std::size_t i = 0; i < tbl->fields.size(); ++i) {
            if (tbl->staging_nulls[i]) continue;  // let the DB default it
            if (any) { cols += ", "; vals += ", "; }
            cols += quote_ident(tbl->fields[i].name);
            vals += literal_for(i);
            any = true;
        }
        if (!any) return util::Error{5001, 0, "insert has no columns", tbl->name};
        const std::string sql =
            "INSERT INTO " + quote_ident(tbl->sql_table) +
            " (" + cols + ") VALUES (" + vals + ")";
        if (auto e = exec_dml(&impl_->db, &impl_->tr, sql); !e) return e.error();

        // Capture this row's PK from staging, then persist + re-snapshot.
        FirebirdTable::PkRow pk;
        pk.values.resize(tbl->pk_columns.size());
        bool pk_ready = true;
        for (std::size_t i = 0; i < tbl->pk_columns.size(); ++i) {
            const std::size_t fi = field_index_ci(*tbl, tbl->pk_columns[i]);
            if (fi == static_cast<std::size_t>(-1) || tbl->staging_nulls[fi]) {
                pk_ready = false; break;
            }
            pk.values[i] = tbl->staging_row[fi];
        }
        tbl->pending_append   = false;
        tbl->row_dirty        = false;
        tbl->rec_count_cached = false;
        ISC_STATUS_ARRAY st;
        isc_commit_retaining(st, &impl_->tr);
        if (status_failed(st)) return fb_error("firebird commit", st);
        if (auto s = load_pk_snapshot(&impl_->db, &impl_->tr, tbl); !s) return s.error();
        tbl->cached_rec_count = static_cast<std::uint32_t>(tbl->pk_snapshot.size());
        tbl->rec_count_cached = true;
        if (pk_ready) {
            for (std::size_t i = 0; i < tbl->pk_snapshot.size(); ++i) {
                if (tbl->pk_snapshot[i].values == pk.values) {
                    return load_current_row(&impl_->db, &impl_->tr, tbl, i);
                }
            }
        }
        return util::Result<void>{};
    }

    // UPDATE the positioned row by its PK.
    if (tbl->pos >= tbl->pk_snapshot.size()) {
        return util::Error{5026, 0, "no current record", ""};
    }
    const FirebirdTable::PkRow current_pk = tbl->pk_snapshot[tbl->pos];
    std::string set_clause;
    bool any = false;
    for (std::size_t i = 0; i < tbl->fields.size(); ++i) {
        bool is_pk = false;
        for (const auto& pkc : tbl->pk_columns) {
            if (to_upper(pkc) == to_upper(tbl->fields[i].name)) { is_pk = true; break; }
        }
        if (is_pk) continue;
        if (any) set_clause += ", ";
        set_clause += quote_ident(tbl->fields[i].name) + " = " + literal_for(i);
        any = true;
    }
    if (!any) { tbl->row_dirty = false; return util::Result<void>{}; }
    const std::string sql =
        "UPDATE " + quote_ident(tbl->sql_table) + " SET " + set_clause +
        " WHERE " + pk_where_clause(*tbl, current_pk);
    if (auto e = exec_dml(&impl_->db, &impl_->tr, sql); !e) return e.error();
    tbl->row_dirty = false;
    ISC_STATUS_ARRAY st;
    isc_commit_retaining(st, &impl_->tr);
    if (status_failed(st)) return fb_error("firebird commit", st);
    return load_current_row(&impl_->db, &impl_->tr, tbl, tbl->pos);
}

util::Result<void> FirebirdConnection::delete_record(FirebirdTable* tbl) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird delete", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird delete", ""};
    }
    if (tbl->pending_append || tbl->pos >= tbl->pk_snapshot.size() ||
        !tbl->positioned) {
        return util::Error{5026, 0, "no current record", ""};
    }
    const std::string sql =
        "DELETE FROM " + quote_ident(tbl->sql_table) + " WHERE " +
        pk_where_clause(*tbl, tbl->pk_snapshot[tbl->pos]);
    if (auto e = exec_dml(&impl_->db, &impl_->tr, sql); !e) return e.error();
    ISC_STATUS_ARRAY st;
    isc_commit_retaining(st, &impl_->tr);
    if (status_failed(st)) return fb_error("firebird commit", st);

    tbl->positioned    = false;
    tbl->row_valid     = false;
    tbl->row_dirty     = false;
    tbl->pending_append = false;
    if (auto s = load_pk_snapshot(&impl_->db, &impl_->tr, tbl); !s) return s.error();
    tbl->cached_rec_count = static_cast<std::uint32_t>(tbl->pk_snapshot.size());
    tbl->rec_count_cached = true;
    tbl->pos              = tbl->cached_rec_count;
    tbl->current_recno    = 0;
    return util::Result<void>{};
}

util::Result<void> FirebirdConnection::lock_record(FirebirdTable* tbl,
                                                   std::uint32_t recno) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird lock", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird lock", ""};
    }
    const std::size_t pos =
        (recno == 0) ? tbl->pos : static_cast<std::size_t>(recno - 1);
    if (pos >= tbl->pk_snapshot.size()) {
        return util::Error{5026, 0, "no current record", ""};
    }
    if (!impl_->lock_table_ready) {
        if (auto e = fb_ensure_lock_table(&impl_->db); !e) return e.error();
        impl_->lock_table_ready = true;
    }
    const std::string key = fb_record_lock_key(*tbl, pos);
    auto r = fb_lock_insert(&impl_->db, key);
    if (!r) return r.error();
    if (!r.value()) return util::Error{5035, 0, "record locked", ""};
    impl_->held_locks.insert(key);
    return util::Result<void>{};
}

util::Result<void> FirebirdConnection::unlock_record(FirebirdTable* tbl,
                                                     std::uint32_t recno) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird unlock", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird unlock", ""};
    }
    const std::size_t pos =
        (recno == 0) ? tbl->pos : static_cast<std::size_t>(recno - 1);
    if (pos >= tbl->pk_snapshot.size()) return util::Result<void>{};
    const std::string key = fb_record_lock_key(*tbl, pos);
    auto r = fb_lock_delete(&impl_->db, key);
    if (!r) return r.error();
    impl_->held_locks.erase(key);
    return util::Result<void>{};
}

util::Result<void> FirebirdConnection::lock_table(FirebirdTable* tbl) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird lock", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird lock", ""};
    }
    if (!impl_->lock_table_ready) {
        if (auto e = fb_ensure_lock_table(&impl_->db); !e) return e.error();
        impl_->lock_table_ready = true;
    }
    const std::string key = fb_table_lock_key(*tbl);
    auto r = fb_lock_insert(&impl_->db, key);
    if (!r) return r.error();
    if (!r.value()) return util::Error{5035, 0, "table locked", ""};
    impl_->held_locks.insert(key);
    return util::Result<void>{};
}

util::Result<void> FirebirdConnection::unlock_table(FirebirdTable* tbl) {
    if (!impl_) return util::Error{5001, 0, "invalid firebird unlock", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid() || tbl == nullptr) {
        return util::Error{5001, 0, "invalid firebird unlock", ""};
    }
    const std::string key = fb_table_lock_key(*tbl);
    auto r = fb_lock_delete(&impl_->db, key);
    if (!r) return r.error();
    impl_->held_locks.erase(key);
    return util::Result<void>{};
}

util::Result<std::unique_ptr<FirebirdTable>>
FirebirdConnection::run_sql(const std::string& sql) {
    if (!impl_) return util::Error{5001, 0, "firebird connection not open", ""};
    std::lock_guard<std::mutex> lk(impl_->mu);
    if (!valid()) return util::Error{5001, 0, "firebird connection not open", ""};

    // exec_query prepares + executes any statement; a SELECT yields columns,
    // DML/DDL yields none (and has already run by the time we get here).
    auto r = exec_query(&impl_->db, &impl_->tr, sql);
    if (!r) return r.error();
    if (r.value().col_names.empty()) {
        // DML / DDL: persist it and report no cursor.
        ISC_STATUS_ARRAY st;
        isc_commit_retaining(st, &impl_->tr);
        return std::unique_ptr<FirebirdTable>{};
    }

    auto tbl       = std::make_unique<FirebirdTable>();
    tbl->conn      = this;
    tbl->name      = "(result)";
    tbl->is_result = true;
    tbl->fields.reserve(r.value().col_names.size());
    for (auto& cn : r.value().col_names) {
        FirebirdTable::FieldDesc fd;
        fd.name = cn;
        fd.type = ADS_STRING;  // passthrough cursors expose text cells
        fd.length = 0;
        tbl->fields.push_back(fd);
    }
    tbl->fields_cached    = true;
    tbl->result_rows      = std::move(r.value().rows);
    tbl->result_nulls     = std::move(r.value().nulls);
    tbl->cached_rec_count = static_cast<std::uint32_t>(tbl->result_rows.size());
    tbl->rec_count_cached = true;
    tbl->positioned       = false;
    tbl->pos              = 0;
    tbl->row_valid        = false;
    tbl->current_recno    = 0;
    return tbl;
}

} // namespace openads::sql_backend

#endif // OPENADS_WITH_FIREBIRD
