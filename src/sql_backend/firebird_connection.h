#pragma once

#include "sql_backend/firebird_table.h"
#include "sql_backend/firebird_uri.h"
#include "sql_backend/sql_common.h"
#include "util/result.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace openads::sql_backend {

// Native Firebird backend. Talks to a Firebird database — embedded `.fdb`
// in-process or a TCP server — through the libfbclient `isc_*` / DSQL API.
// Read navigation mirrors the ODBC backend (primary-key snapshot); write
// support (append / update / delete) mirrors the MariaDB backend (staging
// buffer flushed as INSERT / UPDATE / DELETE).
class FirebirdConnection {
public:
    FirebirdConnection();
    ~FirebirdConnection();

    FirebirdConnection(FirebirdConnection&&) noexcept;
    FirebirdConnection& operator=(FirebirdConnection&&) noexcept;

    FirebirdConnection(const FirebirdConnection&)            = delete;
    FirebirdConnection& operator=(const FirebirdConnection&) = delete;

    static util::Result<FirebirdConnection> open(const FirebirdUri& uri);

    void disconnect() noexcept;
    bool valid() const noexcept;

    util::Result<std::unique_ptr<FirebirdTable>>
        open_table(const std::string& table_name);

    util::Result<void> goto_top(FirebirdTable* tbl);
    util::Result<void> goto_bottom(FirebirdTable* tbl);
    util::Result<void> skip(FirebirdTable* tbl, std::int32_t step);

    util::Result<bool>          at_eof(FirebirdTable* tbl) const;
    util::Result<bool>          at_bof(FirebirdTable* tbl) const;
    util::Result<std::uint32_t> record_count(FirebirdTable* tbl);

    util::Result<std::vector<FirebirdTable::FieldDesc>>
        describe_table(FirebirdTable* tbl);

    util::Result<void> read_field(FirebirdTable* tbl,
                                  const std::string& field_name,
                                  std::string& buf,
                                  bool& is_null) const;

    util::Result<bool> seek_index(FirebirdTable* tbl,
                                  const std::string& column,
                                  IndexExprKind kind,
                                  const std::string& key,
                                  bool soft,
                                  bool last_key);

    util::Result<void> append_blank(FirebirdTable* tbl);
    util::Result<void> set_field(FirebirdTable* tbl,
                                 const std::string& field_name,
                                 const std::string& value);
    util::Result<void> flush_record(FirebirdTable* tbl);
    util::Result<void> delete_record(FirebirdTable* tbl);

    // rLock()/fLock() emulated with a lock table (OPENADS$LOCKS) since Firebird
    // has no advisory-lock primitive: INSERT a unique key to acquire (a PK
    // violation means another attachment holds it), DELETE to release. Each runs
    // in its own short transaction so it is visible cross-attachment at once and
    // never disturbs the data transaction. recno is the 1-based ACE record
    // number (0 = current). Held keys are released on disconnect.
    util::Result<void> lock_record(FirebirdTable* tbl, std::uint32_t recno);
    util::Result<void> unlock_record(FirebirdTable* tbl, std::uint32_t recno);
    util::Result<void> lock_table(FirebirdTable* tbl);
    util::Result<void> unlock_table(FirebirdTable* tbl);

    // AdsExecuteSQLDirect passthrough: run any statement. Returns a navigable
    // cursor when the statement produces a result set, or nullptr for DML/DDL.
    util::Result<std::unique_ptr<FirebirdTable>> run_sql(const std::string& sql);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace openads::sql_backend
