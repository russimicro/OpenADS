#pragma once

#include "sql_backend/backend_tx_manager.h"
#include "sql_backend/sqlite_table.h"
#include "sql_backend/uri.h"
#include "util/result.h"

#include <cstdint>
#include <memory>
#include <string>

namespace openads::sql_backend {

// Connection to an embedded SQLite database. Navigation methods take a
// SqliteTable* so ace_exports can dispatch like RemoteConnection/RemoteTable.
class SqliteConnection {
public:
    SqliteConnection();
    ~SqliteConnection();

    SqliteConnection(SqliteConnection&&) noexcept;
    SqliteConnection& operator=(SqliteConnection&&) noexcept;

    SqliteConnection(const SqliteConnection&)            = delete;
    SqliteConnection& operator=(const SqliteConnection&) = delete;

    static util::Result<SqliteConnection> open(const SqliteUri& uri);

    util::Result<bool> seek_index(SqliteTable* tbl,
                                  const std::string& column,
                                  const std::string& key,
                                  bool soft,
                                  bool last_key);

    void disconnect() noexcept;
    bool valid() const noexcept;

    util::Result<std::unique_ptr<SqliteTable>>
        open_table(const std::string& table_name);

    util::Result<void> goto_top(SqliteTable* tbl);
    util::Result<void> goto_bottom(SqliteTable* tbl);
    util::Result<void> skip(SqliteTable* tbl, std::int32_t step);

    // Tier-2 push-down: install (where non-empty) or clear (where empty) a SQL
    // WHERE fragment and reload the rowid list so navigation walks only the
    // matching rows. `where` must be a trusted, already-translated SQL boolean
    // expression (see engine::try_emit_sql_where) — it is spliced into the
    // SELECT verbatim. Resets the cursor to an unpositioned state.
    util::Result<void> set_filter(SqliteTable* tbl, const std::string& where);

    util::Result<bool>          at_eof(SqliteTable* tbl) const;
    util::Result<bool>          at_bof(SqliteTable* tbl) const;
    util::Result<std::uint32_t> record_count(SqliteTable* tbl);

    util::Result<std::vector<SqliteTable::FieldDesc>>
        describe_table(SqliteTable* tbl);

    // Fills buf with the ACE-padded field value; sets is_null when the
    // SQLite column is SQL NULL.
    util::Result<void> read_field(SqliteTable* tbl,
                                  const std::string& field_name,
                                  std::string& buf,
                                  bool& is_null) const;

    // AdsExecuteSQLDirect passthrough: prepare and run any statement. Returns a
    // materialized, navigable result cursor when the statement produces rows
    // (column count > 0), or a null pointer for a non-result statement
    // (INSERT/UPDATE/DELETE/DDL), which is executed to completion. SQLite itself
    // classifies the statement, so no SQL keyword parsing is needed.
    util::Result<std::unique_ptr<SqliteTable>> run_sql(const std::string& sql);

    const std::string& db_path() const noexcept { return db_path_; }

    // Execute a simple SQL statement (no result set). Used by the
    // transaction manager for BEGIN/COMMIT/ROLLBACK/SAVEPOINT.
    util::Result<void> exec_sql(const std::string& sql);

    // ── Tier 1: Transaction management (SQLRDD pattern) ─────────────
    BackendTxManager& tx_manager() noexcept { return tx_mgr_; }
    const BackendTxManager& tx_manager() const noexcept { return tx_mgr_; }

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
    std::string           db_path_;
    BackendTxManager      tx_mgr_;
};

} // namespace openads::sql_backend