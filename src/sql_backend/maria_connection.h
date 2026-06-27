#pragma once

#include "sql_backend/maria_table.h"
#include "sql_backend/maria_uri.h"
#include "util/result.h"

#include <cstdint>
#include <memory>
#include <string>

namespace openads::sql_backend {

class MariaConnection {
public:
    MariaConnection();
    ~MariaConnection();

    MariaConnection(MariaConnection&&) noexcept;
    MariaConnection& operator=(MariaConnection&&) noexcept;

    MariaConnection(const MariaConnection&)            = delete;
    MariaConnection& operator=(const MariaConnection&) = delete;

    static util::Result<MariaConnection> open(const MariaUri& uri);

    void disconnect() noexcept;
    bool valid() const noexcept;

    util::Result<std::unique_ptr<MariaTable>>
        open_table(const std::string& table_name);

    util::Result<void> goto_top(MariaTable* tbl);
    util::Result<void> goto_bottom(MariaTable* tbl);
    util::Result<void> skip(MariaTable* tbl, std::int32_t step);

    util::Result<bool>          at_eof(MariaTable* tbl) const;
    util::Result<bool>          at_bof(MariaTable* tbl) const;
    util::Result<std::uint32_t> record_count(MariaTable* tbl);

    util::Result<std::vector<MariaTable::FieldDesc>>
        describe_table(MariaTable* tbl);

    util::Result<void> read_field(MariaTable* tbl,
                                  const std::string& field_name,
                                  std::string& buf,
                                  bool& is_null) const;

    util::Result<bool> seek_index(MariaTable* tbl,
                                  const std::string& column,
                                  const std::string& key,
                                  bool soft,
                                  bool last_key);

    // Write surface (mirrors FirebirdConnection): append_blank stages a blank
    // row, set_field stages one column, flush_record emits an INSERT
    // (pending_append) or a PK-keyed UPDATE, delete_record a PK-keyed DELETE.
    util::Result<void> append_blank(MariaTable* tbl);
    util::Result<void> set_field(MariaTable* tbl,
                                 const std::string& field_name,
                                 const std::string& value);
    util::Result<void> flush_record(MariaTable* tbl);
    util::Result<void> delete_record(MariaTable* tbl);

    // rLock()/fLock() emulated with MariaDB named locks (GET_LOCK / RELEASE_LOCK):
    // session-scoped, cross-connection, held across statements. recno is the
    // 1-based ACE record number (0 = current). lock_record fails if another
    // session holds it.
    util::Result<void> lock_record(MariaTable* tbl, std::uint32_t recno);
    util::Result<void> unlock_record(MariaTable* tbl, std::uint32_t recno);
    util::Result<void> lock_table(MariaTable* tbl);
    util::Result<void> unlock_table(MariaTable* tbl);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace openads::sql_backend