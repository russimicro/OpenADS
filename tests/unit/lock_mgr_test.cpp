#include "doctest.h"
#include "engine/lock_mgr.h"
#include "platform/file.h"

#include <filesystem>

namespace fs = std::filesystem;
using openads::engine::LockMgr;
using openads::engine::LockingMode;
using openads::engine::TableTypeForLock;
using openads::platform::File;
using openads::platform::OpenMode;

TEST_CASE("LockMgr Compatible/NTX file lock at byte 1_000_000_000") {
    auto p = fs::temp_directory_path() / "openads_m2_lock_ntx";
    fs::remove(p);
    {
        auto fres = File::open(p.string(), OpenMode::CreateRW);
        REQUIRE(fres.has_value());
        File f = std::move(fres).value();
        LockMgr mgr;
        auto lock = mgr.lock_table_excl(f, TableTypeForLock::Ntx,
                                        LockingMode::Compatible);
        REQUIRE(lock.has_value());
        CHECK(lock.value().offset() == 1'000'000'000ull);
        CHECK(lock.value().length() == 1ull);
    }
    fs::remove(p);
}

TEST_CASE("LockMgr Compatible/CDX record lock at 0x7FFFFFFE - recno") {
    auto p = fs::temp_directory_path() / "openads_m2_lock_cdx";
    fs::remove(p);
    {
        auto fres = File::open(p.string(), OpenMode::CreateRW);
        REQUIRE(fres.has_value());
        File f = std::move(fres).value();
        LockMgr mgr;
        auto lock = mgr.lock_record_excl(f, TableTypeForLock::Cdx,
                                         LockingMode::Compatible, 7);
        REQUIRE(lock.has_value());
        CHECK(lock.value().offset() == (0x7FFFFFFEull - 7ull));
        CHECK(lock.value().length() == 1ull);
    }
    fs::remove(p);
}

TEST_CASE("LockMgr re-entrant record lock from same handle is a no-op") {
    auto p = fs::temp_directory_path() / "openads_m2_lock_reenter";
    fs::remove(p);
    {
        auto fres = File::open(p.string(), OpenMode::CreateRW);
        REQUIRE(fres.has_value());
        File f = std::move(fres).value();
        LockMgr mgr;
        auto a = mgr.lock_record_excl(f, TableTypeForLock::Cdx,
                                      LockingMode::Compatible, 42);
        REQUIRE(a.has_value());
        auto b = mgr.lock_record_excl(f, TableTypeForLock::Cdx,
                                      LockingMode::Compatible, 42);
        REQUIRE(b.has_value());
    }
    fs::remove(p);
}
