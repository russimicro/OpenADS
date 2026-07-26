// Tests for navigation behaviour on an empty table.
// An empty table (zero records) must enter "Limbo" state after any
// positioning call: both BOF and EOF are set simultaneously, and
// AdsGetField must report AE_NO_CURRENT_RECORD.
#include "doctest.h"
#include "openads/ace.h"

#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

extern "C" {
UNSIGNED32 AdsGotoTop(ADSHANDLE hTable);
UNSIGNED32 AdsGotoBottom(ADSHANDLE hTable);
UNSIGNED32 AdsSkip(ADSHANDLE hTable, SIGNED32 lRows);
UNSIGNED32 AdsAtBOF(ADSHANDLE hTable, UNSIGNED16* pbAtBegin);
UNSIGNED32 AdsAtEOF(ADSHANDLE hTable, UNSIGNED16* pbAtEnd);
UNSIGNED32 AdsGetRecordNum(ADSHANDLE hTable, UNSIGNED16, UNSIGNED32* pulRec);
}

namespace {

// RAII wrapper: creates a fresh empty CDX table for navigation tests.
struct EmptyTable {
    ADSHANDLE hConn = 0;
    ADSHANDLE hT    = 0;
    fs::path  dir;

    explicit EmptyTable(const char* subdir) {
        dir = fs::temp_directory_path() / subdir;
        std::error_code ec;
        fs::remove_all(dir, ec);
        fs::create_directories(dir);

        UNSIGNED8 srv[256];
        std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
        REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                             nullptr, nullptr, 0, &hConn) == 0);

        UNSIGNED8 def[]   = "FSTR,C,4,0";
        UNSIGNED8 tname[] = "nav";
        REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                               0, 0, 0, 0, def, &hT) == 0);
    }

    ~EmptyTable() {
        AdsCloseTable(hT);
        AdsDisconnect(hConn);
        std::error_code ec;
        fs::remove_all(dir, ec);
    }
};

static void check_limbo(ADSHANDLE hT, const char* label) {
    UNSIGNED16 bof = 99, eof = 99;
    REQUIRE(AdsAtBOF(hT, &bof) == 0);
    REQUIRE(AdsAtEOF(hT, &eof) == 0);
    INFO(label << ": bof=" << bof << " eof=" << eof);
    CHECK(bof == 1);
    CHECK(eof == 1);
}

} // namespace

TEST_CASE("Navigation empty table: GotoTop sets BOF+EOF (Limbo)") {
    EmptyTable et("openads_nav_empty_gotop");
    REQUIRE(AdsGotoTop(et.hT) == 0);
    check_limbo(et.hT, "GotoTop");
}

TEST_CASE("Navigation empty table: GotoBottom sets BOF+EOF (Limbo)") {
    EmptyTable et("openads_nav_empty_gobottom");
    REQUIRE(AdsGotoBottom(et.hT) == 0);
    check_limbo(et.hT, "GotoBottom");
}

TEST_CASE("Navigation empty table: Skip(1) from Limbo stays at EOF") {
    EmptyTable et("openads_nav_empty_skip_fwd");
    REQUIRE(AdsGotoTop(et.hT) == 0);
    REQUIRE(AdsSkip(et.hT, 1) == 0);
    UNSIGNED16 eof = 99;
    REQUIRE(AdsAtEOF(et.hT, &eof) == 0);
    CHECK(eof == 1);
}

TEST_CASE("Navigation empty table: Skip(-1) from Limbo stays at BOF") {
    EmptyTable et("openads_nav_empty_skip_back");
    REQUIRE(AdsGotoTop(et.hT) == 0);
    REQUIRE(AdsSkip(et.hT, -1) == 0);
    UNSIGNED16 bof = 99;
    REQUIRE(AdsAtBOF(et.hT, &bof) == 0);
    CHECK(bof == 1);
}

TEST_CASE("Navigation empty table: AdsGetField at EOF/Limbo returns AE_NO_CURRENT_RECORD") {
    EmptyTable et("openads_nav_empty_getfield");
    REQUIRE(AdsGotoTop(et.hT) == 0);

    // Confirm Limbo before the read attempt.
    UNSIGNED16 eof = 99;
    REQUIRE(AdsAtEOF(et.hT, &eof) == 0);
    REQUIRE(eof == 1);

    UNSIGNED8 fld[8]  = "FSTR";
    UNSIGNED8 buf[16] = {};
    UNSIGNED32 cap    = sizeof(buf);
    UNSIGNED32 rc     = AdsGetField(et.hT, fld, buf, &cap, 0);
    // Must not silently succeed when there is no current record.
    // The canonical error for this state is AE_NO_CURRENT_RECORD (5026).
    INFO("AdsGetField on empty-table Limbo returned: " << rc);
    CHECK(rc == AE_NO_CURRENT_RECORD);
}
