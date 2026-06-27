// Round-trip settings (AdsSetDefault/Get, AdsSetSearchPath/Get,
// AdsSetDecimals), AdsRefreshAOF re-evaluation, and handle validation on
// the cache/diagnostic hooks that used to be blind no-ops.
#include "doctest.h"
#include "openads/ace.h"
#include "openads/error.h"

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace {
int aof_visible_count(ADSHANDLE hT) {
    if (AdsGotoTop(hT) != openads::AE_SUCCESS) return -1;
    int n = 0;
    for (;;) {
        UNSIGNED16 eof = 0;
        if (AdsAtEOF(hT, &eof) != openads::AE_SUCCESS) return -1;
        if (eof) break;
        ++n;
        if (AdsSkip(hT, 1) != openads::AE_SUCCESS) return -1;
    }
    return n;
}
}  // namespace

TEST_CASE("AdsSetDefault / AdsSetSearchPath round-trip through their getters") {
    UNSIGNED8 def[] = "C:\\DATA\\APP";
    REQUIRE(AdsSetDefault(def) == openads::AE_SUCCESS);
    UNSIGNED8 buf[64];
    UNSIGNED16 len = sizeof(buf);
    REQUIRE(AdsGetDefault(buf, &len) == openads::AE_SUCCESS);
    CHECK(std::string((char*)buf, len) == "C:\\DATA\\APP");

    UNSIGNED8 sp[] = "C:\\A;C:\\B";
    REQUIRE(AdsSetSearchPath(sp) == openads::AE_SUCCESS);
    len = sizeof(buf);
    REQUIRE(AdsGetSearchPath(buf, &len) == openads::AE_SUCCESS);
    CHECK(std::string((char*)buf, len) == "C:\\A;C:\\B");

    // Clearing back to empty round-trips too.
    UNSIGNED8 empty[] = "";
    REQUIRE(AdsSetDefault(empty) == openads::AE_SUCCESS);
    len = sizeof(buf);
    REQUIRE(AdsGetDefault(buf, &len) == openads::AE_SUCCESS);
    CHECK(len == 0);
}

TEST_CASE("AdsSetDecimals and the cache/diagnostic hooks validate their input") {
    CHECK(AdsSetDecimals(4) == openads::AE_SUCCESS);

    // Advisory cache hints succeed without a handle.
    CHECK(AdsCacheOpenTables(1) == openads::AE_SUCCESS);
    CHECK(AdsCacheOpenCursors(1) == openads::AE_SUCCESS);

    // Handle-bearing hooks now reject an unknown handle.
    CHECK(AdsCacheRecords(0, 10) != openads::AE_SUCCESS);
    CHECK(AdsTestRecLocks(0) != openads::AE_SUCCESS);
}

TEST_CASE("AdsShowError accepts null, empty, and non-empty messages") {
    CHECK(AdsShowError(nullptr) == openads::AE_SUCCESS);
    UNSIGNED8 empty[] = "";
    CHECK(AdsShowError(empty) == openads::AE_SUCCESS);
    UNSIGNED8 msg[] = "openads test diagnostic";
    CHECK(AdsShowError(msg) == openads::AE_SUCCESS);
}

TEST_CASE("AdsRefreshAOF re-evaluates the stored filter over current data") {
    const auto dir = fs::temp_directory_path() / "openads_refresh_aof";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn)
            == openads::AE_SUCCESS);

    UNSIGNED8 def[]   = "N,Numeric,4,0";
    UNSIGNED8 tname[] = "raof";
    ADSHANDLE hT = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX, 0, 0, 0, 0, def, &hT)
            == openads::AE_SUCCESS);
    UNSIGNED8 fN[8] = "N";
    for (int i = 1; i <= 5; ++i) {
        REQUIRE(AdsAppendRecord(hT) == openads::AE_SUCCESS);
        REQUIRE(AdsSetDouble(hT, fN, (double)i) == openads::AE_SUCCESS);
        REQUIRE(AdsWriteRecord(hT) == openads::AE_SUCCESS);
    }

    // Refresh with no AOF set is a harmless success.
    REQUIRE(AdsRefreshAOF(hT) == openads::AE_SUCCESS);

    std::string cond = "N <= 3";
    REQUIRE(AdsSetAOF(hT, (UNSIGNED8*)cond.data(), 0) == openads::AE_SUCCESS);
    CHECK(aof_visible_count(hT) == 3);

    // Append a new matching record. The stale AOF bitmap (size 5) still
    // hides it until a refresh re-evaluates the whole table.
    REQUIRE(AdsAppendRecord(hT) == openads::AE_SUCCESS);
    REQUIRE(AdsSetDouble(hT, fN, 2.0) == openads::AE_SUCCESS);
    REQUIRE(AdsWriteRecord(hT) == openads::AE_SUCCESS);
    CHECK(aof_visible_count(hT) == 3);

    REQUIRE(AdsRefreshAOF(hT) == openads::AE_SUCCESS);
    CHECK(aof_visible_count(hT) == 4);

    // Bad handle is rejected.
    CHECK(AdsRefreshAOF(0) != openads::AE_SUCCESS);

    REQUIRE(AdsCloseTable(hT) == openads::AE_SUCCESS);
    REQUIRE(AdsDisconnect(hConn) == openads::AE_SUCCESS);
    fs::remove_all(dir, ec);
}
