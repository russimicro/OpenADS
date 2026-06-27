#pragma once

#include "sql_backend/odbc_table.h"
#include "sql_backend/odbc_uri.h"
#include "sql_backend/sql_common.h"
#include "util/result.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace openads::sql_backend {

// ODBC backend. Talks to any data source with an ODBC driver (SQL Server,
// Oracle, Firebird, PostgreSQL, MariaDB, DB2, Access, …) through the
// Win32 / unixODBC API. Read navigates a primary-key snapshot; write
// (append / update / delete) stages field values and flushes one
// INSERT / UPDATE / DELETE per record. v1 expects the caller to supply the
// primary key on append (no IDENTITY round-trip yet) and formats values as
// SQL literals (parameter binding is a later hardening slice).
class OdbcConnection {
public:
    OdbcConnection();
    ~OdbcConnection();

    OdbcConnection(OdbcConnection&&) noexcept;
    OdbcConnection& operator=(OdbcConnection&&) noexcept;

    OdbcConnection(const OdbcConnection&)            = delete;
    OdbcConnection& operator=(const OdbcConnection&) = delete;

    static util::Result<OdbcConnection> open(const OdbcUri& uri);

    void disconnect() noexcept;
    bool valid() const noexcept;

    util::Result<std::unique_ptr<OdbcTable>>
        open_table(const std::string& table_name);

    util::Result<void> goto_top(OdbcTable* tbl);
    util::Result<void> goto_bottom(OdbcTable* tbl);
    util::Result<void> skip(OdbcTable* tbl, std::int32_t step);

    util::Result<bool>          at_eof(OdbcTable* tbl) const;
    util::Result<bool>          at_bof(OdbcTable* tbl) const;
    util::Result<std::uint32_t> record_count(OdbcTable* tbl);

    util::Result<std::vector<OdbcTable::FieldDesc>>
        describe_table(OdbcTable* tbl);

    util::Result<void> read_field(OdbcTable* tbl,
                                  const std::string& field_name,
                                  std::string& buf,
                                  bool& is_null) const;

    util::Result<bool> seek_index(OdbcTable* tbl,
                                  const std::string& column,
                                  IndexExprKind kind,
                                  const std::string& key,
                                  bool soft,
                                  bool last_key);

    // --- navigational write (v1) ---
    // append_blank starts a fresh staged row; set_field stages a value by
    // column name (append OR positioned edit); flush_table emits the
    // INSERT (appending) or UPDATE (positioned) and repositions on the
    // written row; delete_record removes the current row.
    util::Result<void> append_blank(OdbcTable* tbl);
    util::Result<void> set_field(OdbcTable* tbl,
                                 const std::string& name,
                                 const std::string& value);
    util::Result<void> flush_table(OdbcTable* tbl);
    util::Result<void> delete_record(OdbcTable* tbl);

    // rLock()/fLock() emulated with SQL Server session-scoped application locks
    // (sp_getapplock / sp_releaseapplock @LockOwner='Session'): cross-connection,
    // held across statements, auto-released on session end. recno is the 1-based
    // ACE record number (0 = current). Only SQL Server exposes this primitive;
    // any other ODBC backend returns AE_FUNCTION_NOT_AVAILABLE.
    util::Result<void> lock_record(OdbcTable* tbl, std::uint32_t recno);
    util::Result<void> unlock_record(OdbcTable* tbl, std::uint32_t recno);
    util::Result<void> lock_table(OdbcTable* tbl);
    util::Result<void> unlock_table(OdbcTable* tbl);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace openads::sql_backend
