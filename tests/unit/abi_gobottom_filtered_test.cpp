#include "doctest.h"
#include "openads/ace.h"

#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

extern "C" {
UNSIGNED32 AdsGotoBottom(ADSHANDLE hTable);
UNSIGNED32 AdsGotoTop(ADSHANDLE hTable);
UNSIGNED32 AdsSkip(ADSHANDLE hTable, SIGNED32 lRows);
UNSIGNED32 AdsAtBOF(ADSHANDLE hTable, UNSIGNED16* pbAtBegin);
UNSIGNED32 AdsAtEOF(ADSHANDLE hTable, UNSIGNED16* pbAtEnd);
UNSIGNED32 AdsGetRecordNum(ADSHANDLE hTable, UNSIGNED16,
                           UNSIGNED32* pulRec);
UNSIGNED32 AdsShowDeleted(UNSIGNED16);
UNSIGNED32 AdsDeleteRecord(ADSHANDLE hTable);
UNSIGNED32 AdsGotoRecord(ADSHANDLE hTable, UNSIGNED32);
}

static void rprint(ADSHANDLE hT, const char* tag) {
    UNSIGNED32 r = 0; UNSIGNED16 b = 0, e = 0;
    AdsGetRecordNum(hT, 0, &r);
    AdsAtBOF(hT, &b);
    AdsAtEOF(hT, &e);
    INFO(tag << " recno=" << r << " bof=" << b << " eof=" << e);
    MESSAGE(tag << " recno=" << r << " bof=" << b << " eof=" << e);
}

TEST_CASE("DBGOBOTTOM with FOR-clause + deletes lands on last live recno") {
    auto dir = fs::temp_directory_path() / "openads_gobottom";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "FNUM,N,10,0;FSTR,C,4,0";
    UNSIGNED8 tname[] = "gob";
    ADSHANDLE hT = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hT) == 0);

    UNSIGNED8 fnum_n[] = "FNUM";
    UNSIGNED8 fstr_n[] = "FSTR";
    for (int i = 1; i <= 15; ++i) {
        REQUIRE(AdsAppendRecord(hT) == 0);
        AdsSetDouble(hT, fnum_n, static_cast<double>((i + 2) / 3));
        UNSIGNED8 sval[5];
        sval[0] = static_cast<UNSIGNED8>(((i + 2) / 3) + '0');
        sval[1] = sval[2] = sval[3] = ' ';
        sval[4] = 0;
        AdsSetString(hT, fstr_n, sval, 4);
    }
    REQUIRE(AdsWriteRecord(hT) == 0);

    UNSIGNED8 bag[]  = "gob.cdx";
    UNSIGNED8 tag[]  = "TG_C";
    UNSIGNED8 expr[] = "FSTR";
    UNSIGNED8 cond[] = "RECNO()<>5";
    ADSHANDLE hI = 0;
    REQUIRE(AdsCreateIndex61(hT, bag, tag, expr, cond,
                             nullptr, 0, 0, &hI) == 0);

    // Delete recnos 1, 3, 6, 7, 13, 14, 15 like the rddtst flow.
    UNSIGNED32 dels[] = {1u, 3u, 6u, 7u, 13u, 14u, 15u};
    for (UNSIGNED32 r : dels) {
        REQUIRE(AdsGotoRecord(hT, r) == 0);
        REQUIRE(AdsDeleteRecord(hT) == 0);
    }
    REQUIRE(AdsWriteRecord(hT) == 0);

    AdsShowDeleted(0);  // hide deleted

    REQUIRE(AdsGotoBottom(hT) == 0);
    rprint(hT, "GOBOTTOM");
    UNSIGNED32 r = 0; UNSIGNED16 b = 99, e = 99;
    AdsGetRecordNum(hT, 0, &r);
    AdsAtBOF(hT, &b);
    AdsAtEOF(hT, &e);
    CHECK(r == 12);
    CHECK(b == 0);
    CHECK(e == 0);

    // Reproduce rddtst flow: GOTOP, SKIP(8) -> Eof, SKIP(-20)
    REQUIRE(AdsGotoTop(hT) == 0);
    rprint(hT, "GOTOP");
    REQUIRE(AdsSkip(hT, 8) == 0);
    rprint(hT, "SKIP(8)");
    REQUIRE(AdsSkip(hT, -20) == 0);
    rprint(hT, "SKIP(-20)");
    AdsGetRecordNum(hT, 0, &r);
    AdsAtBOF(hT, &b);
    AdsAtEOF(hT, &e);
    CHECK(r == 2);
    CHECK(b == 1);
    CHECK(e == 0);

    // Repro of post-SKIP(20) -> GOBOTTOM Limbo path.
    REQUIRE(AdsGotoBottom(hT) == 0);
    REQUIRE(AdsSkip(hT, 20) == 0);
    rprint(hT, "SKIP(20)");
    REQUIRE(AdsGotoBottom(hT) == 0);
    rprint(hT, "GOBOTTOM-after-SKIP20");
    AdsGetRecordNum(hT, 0, &r);
    AdsAtBOF(hT, &b);
    AdsAtEOF(hT, &e);
    CHECK(r == 12);
    CHECK(b == 0);
    CHECK(e == 0);

    AdsShowDeleted(1);
    AdsCloseTable(hT);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}
