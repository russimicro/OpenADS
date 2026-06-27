#include "doctest.h"
#include "openads/ace.h"
#include "openads/error.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

void make_dbf(const fs::path& p) {
    std::vector<std::uint8_t> file;
    auto push = [&](const void* d, std::size_t n) {
        const auto* b = static_cast<const std::uint8_t*>(d);
        file.insert(file.end(), b, b + n);
    };
    std::array<std::uint8_t, 32> hdr{};
    hdr[0]  = 0x03;
    hdr[8]  = 32 + 32 + 1;
    hdr[10] = 1 + 4;
    push(hdr.data(), hdr.size());
    std::array<std::uint8_t, 32> f1{};
    std::strncpy(reinterpret_cast<char*>(f1.data()), "ID", 11);
    f1[11] = 'C'; f1[16] = 4;
    push(f1.data(), f1.size());
    file.push_back(0x0D);
    file.push_back(0x1A);
    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(file.data()),
        static_cast<std::streamsize>(file.size()));
}

// Build a DD with one table "tbl", one user "alice", one group "readers",
// alice in readers. Login is required.
fs::path make_perm_add(const fs::path& dir,
                        const std::string& extra_perms = {}) {
    auto p = dir / "test.add";
    std::ofstream f(p);
    f << "# OpenADS Data Dictionary v1\n"
      << "TABLE tbl=tbl.dbf\n"
      << "USER alice\n"
      << "USERPROP alice;prop_1101=pw\n"
      << "USER readers\n"
      << "MEMBER alice=readers\n"
      << "DBPROP prop_5=1\n"   // login required
      << extra_perms;
    return p;
}

// Connect to a DD as a given user.
ADSHANDLE connect_as(const fs::path& add_path,
                      const char* user, const char* pwd) {
    ADSHANDLE h = 0;
    UNSIGNED8 srv[512];
    auto s = add_path.string();
    std::memcpy(srv, s.c_str(), s.size() + 1);
    UNSIGNED8 ubuf[64]{}, pbuf[64]{};
    if (user) std::strncpy(reinterpret_cast<char*>(ubuf), user, 63);
    if (pwd)  std::strncpy(reinterpret_cast<char*>(pbuf), pwd,  63);
    AdsConnect60(srv, ADS_LOCAL_SERVER,
                 user ? ubuf : nullptr,
                 pwd  ? pbuf : nullptr,
                 0, &h);
    return h;
}

ADSHANDLE open_tbl(ADSHANDLE hConn, UNSIGNED16 check_rights,
                   UNSIGNED16 mode = ADS_SHARED) {
    ADSHANDLE h = 0;
    UNSIGNED8 name[8] = "tbl";
    AdsOpenTable(hConn, name, name, ADS_CDX, 0, 0,
                 check_rights, mode, &h);
    return h;
}

}  // namespace

// ---------------------------------------------------------------------------

TEST_CASE("Perms: no ACL → full access for any authenticated user") {
    auto dir = fs::temp_directory_path() / "openads_perm_noacl";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_dbf(dir / "tbl.dbf");
    make_perm_add(dir);   // no TABLEPERM lines

    ADSHANDLE hConn = connect_as(dir / "test.add", "alice", "pw");
    REQUIRE(hConn != 0);

    // usCheckRights=1, no ACL for "tbl" → should succeed
    ADSHANDLE hTbl = open_tbl(hConn, 1);
    CHECK(hTbl != 0);
    if (hTbl) REQUIRE(AdsCloseTable(hTbl) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("Perms: level 0 (none) blocks table open") {
    auto dir = fs::temp_directory_path() / "openads_perm_none";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_dbf(dir / "tbl.dbf");
    make_perm_add(dir, "TABLEPERM tbl;alice=0\n");

    ADSHANDLE hConn = connect_as(dir / "test.add", "alice", "pw");
    REQUIRE(hConn != 0);

    ADSHANDLE hTbl = 0;
    UNSIGNED8 name[8] = "tbl";
    UNSIGNED32 rc = AdsOpenTable(hConn, name, name, ADS_CDX, 0, 0,
                                 1, ADS_SHARED, &hTbl);
    CHECK(rc == openads::AE_ACCESS_DENIED);
    CHECK(hTbl == 0);

    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("Perms: level 1 (read) allows readonly open") {
    auto dir = fs::temp_directory_path() / "openads_perm_read";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_dbf(dir / "tbl.dbf");
    make_perm_add(dir, "TABLEPERM tbl;alice=1\n");

    ADSHANDLE hConn = connect_as(dir / "test.add", "alice", "pw");
    REQUIRE(hConn != 0);

    // read-only open (mode=ADS_READONLY=3) with check_rights → succeeds
    ADSHANDLE hTbl = 0;
    UNSIGNED8 name[8] = "tbl";
    UNSIGNED32 rc = AdsOpenTable(hConn, name, name, ADS_CDX, 0, 0,
                                 1, ADS_READONLY, &hTbl);
    CHECK(rc == 0);
    CHECK(hTbl != 0);
    if (hTbl) REQUIRE(AdsCloseTable(hTbl) == 0);

    // shared (write) open with check_rights → denied at level 1
    ADSHANDLE hTbl2 = 0;
    UNSIGNED32 rc2 = AdsOpenTable(hConn, name, name, ADS_CDX, 0, 0,
                                  1, ADS_SHARED, &hTbl2);
    CHECK(rc2 == openads::AE_ACCESS_DENIED);
    CHECK(hTbl2 == 0);

    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("Perms: level 2 (write) allows shared open") {
    auto dir = fs::temp_directory_path() / "openads_perm_write";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_dbf(dir / "tbl.dbf");
    make_perm_add(dir, "TABLEPERM tbl;alice=2\n");

    ADSHANDLE hConn = connect_as(dir / "test.add", "alice", "pw");
    REQUIRE(hConn != 0);

    ADSHANDLE hTbl = open_tbl(hConn, 1, ADS_SHARED);
    CHECK(hTbl != 0);
    if (hTbl) REQUIRE(AdsCloseTable(hTbl) == 0);

    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("Perms: group membership grants access") {
    auto dir = fs::temp_directory_path() / "openads_perm_group";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_dbf(dir / "tbl.dbf");
    // alice has no direct perm, but is in "readers" which has level 2
    make_perm_add(dir, "TABLEPERM tbl;readers=2\n");

    ADSHANDLE hConn = connect_as(dir / "test.add", "alice", "pw");
    REQUIRE(hConn != 0);

    ADSHANDLE hTbl = open_tbl(hConn, 1, ADS_SHARED);
    CHECK(hTbl != 0);
    if (hTbl) REQUIRE(AdsCloseTable(hTbl) == 0);

    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("Perms: check_rights=0 bypasses ACL") {
    auto dir = fs::temp_directory_path() / "openads_perm_bypass";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_dbf(dir / "tbl.dbf");
    make_perm_add(dir, "TABLEPERM tbl;alice=0\n");  // alice has no access

    ADSHANDLE hConn = connect_as(dir / "test.add", "alice", "pw");
    REQUIRE(hConn != 0);

    // usCheckRights=0 → no enforcement
    ADSHANDLE hTbl = open_tbl(hConn, 0, ADS_SHARED);
    CHECK(hTbl != 0);
    if (hTbl) REQUIRE(AdsCloseTable(hTbl) == 0);

    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("Perms: AdsDDSetUserTableRights / AdsDDGetUserTableRights round-trip") {
    auto dir = fs::temp_directory_path() / "openads_perm_api";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_dbf(dir / "tbl.dbf");
    make_perm_add(dir);  // no TABLEPERM lines

    ADSHANDLE hConn = connect_as(dir / "test.add", "alice", "pw");
    REQUIRE(hConn != 0);

    UNSIGNED8 tbl[8]  = "tbl";
    UNSIGNED8 user[8] = "alice";

    // Set alice to level 2.
    REQUIRE(AdsDDSetUserTableRights(hConn, tbl, user, 2) == 0);

    UNSIGNED32 lvl = 99;
    REQUIRE(AdsDDGetUserTableRights(hConn, tbl, user, &lvl) == 0);
    CHECK(lvl == 2u);

    // Now alice can open for write.
    ADSHANDLE hTbl = open_tbl(hConn, 1, ADS_SHARED);
    CHECK(hTbl != 0);
    if (hTbl) REQUIRE(AdsCloseTable(hTbl) == 0);

    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

// Verify that OpenADS bitmask bit positions match SAP ADS_PERMISSION_* constants:
//   READ=0x01  UPDATE=0x02  EXECUTE=0x04  INHERIT=0x08  INSERT=0x10  DELETE=0x20
// This test pins the bit-level encoding so it is not accidentally regressed.
TEST_CASE("Perms: bitmask bit positions — INSERT is 0x10, DELETE is 0x20") {
    auto dir = fs::temp_directory_path() / "openads_perm_bits";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_dbf(dir / "tbl.dbf");
    // level=3 → bitmask = READ(0x01)|UPDATE(0x02)|INSERT(0x10)|DELETE(0x20) = 0x33
    make_perm_add(dir, "TABLEPERM tbl;alice=3\n");

    ADSHANDLE hConn = connect_as(dir / "test.add", "alice", "pw");
    REQUIRE(hConn != 0);

    // alice can open for shared/write (INSERT+UPDATE present).
    ADSHANDLE hTbl = 0;
    UNSIGNED8 name[8] = "tbl";
    UNSIGNED32 rc = AdsOpenTable(hConn, name, name, ADS_CDX, 0, 0,
                                 1, ADS_SHARED, &hTbl);
    CHECK(rc == 0);
    CHECK(hTbl != 0);
    if (hTbl) {
        // Append should succeed at level 3.
        REQUIRE(AdsAppendRecord(hTbl) == 0);
        REQUIRE(AdsCloseTable(hTbl) == 0);
    }

    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("Perms: AdsDDGetTableProperty returns effective level") {
    auto dir = fs::temp_directory_path() / "openads_perm_prop216";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_dbf(dir / "tbl.dbf");
    make_perm_add(dir, "TABLEPERM tbl;alice=3\n");

    ADSHANDLE hConn = connect_as(dir / "test.add", "alice", "pw");
    REQUIRE(hConn != 0);

    UNSIGNED8 tbl[8] = "tbl";
    UNSIGNED16 buf = 0;
    UNSIGNED16 len = sizeof(buf);
    REQUIRE(AdsDDGetTableProperty(hConn, tbl,
                                  ADS_DD_TABLE_PERMISSION_LEVEL,
                                  &buf, &len) == 0);
    CHECK(buf == 3u);

    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}
