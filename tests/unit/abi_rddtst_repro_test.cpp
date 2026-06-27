#include "doctest.h"
#include "openads/ace.h"

#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

static void rprint(ADSHANDLE hT, const char* tag) {
    UNSIGNED32 r = 0; UNSIGNED16 b = 0, e = 0;
    AdsGetRecordNum(hT, 0, &r);
    AdsAtBOF(hT, &b);
    AdsAtEOF(hT, &e);
    MESSAGE(tag << " recno=" << r << " bof=" << b << " eof=" << e);
}

TEST_CASE("rddtst-flow: deletes/recalls/redelete + DBGOTOP under FOR + SET DELETED") {
    auto dir = fs::temp_directory_path() / "openads_rddtst_repro";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "FNUM,N,10,0;FSTR,C,4,0";
    UNSIGNED8 tname[] = "rt";
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

    UNSIGNED8 bag[]  = "rt.cdx";
    UNSIGNED8 tag[]  = "TG_C";
    UNSIGNED8 expr[] = "FSTR";
    UNSIGNED8 cond[] = "RECNO()<>5";
    ADSHANDLE hI = 0;
    REQUIRE(AdsCreateIndex61(hT, bag, tag, expr, cond,
                             nullptr, 0, 0, &hI) == 0);

    // Round 1 deletes: 1, 3, 6, 7, 13, 14, 15
    AdsShowDeleted(0);  // SET DELETE ON
    UNSIGNED32 dels[] = {1u, 3u, 6u, 7u, 13u, 14u, 15u};
    for (UNSIGNED32 r : dels) {
        REQUIRE(AdsGotoRecord(hT, r) == 0);
        REQUIRE(AdsDeleteRecord(hT) == 0);
    }
    REQUIRE(AdsWriteRecord(hT) == 0);

    // DELETE ALL — mark every still-live record deleted.
    AdsShowDeleted(1);
    REQUIRE(AdsGotoTop(hT) == 0);
    while (true) {
        UNSIGNED16 e = 0; AdsAtEOF(hT, &e);
        if (e) break;
        AdsDeleteRecord(hT);
        AdsSkip(hT, 1);
    }
    REQUIRE(AdsWriteRecord(hT) == 0);

    // DBRECALL on 7
    AdsGotoRecord(hT, 7);
    AdsRecallRecord(hT);

    // RECALL ALL
    REQUIRE(AdsGotoTop(hT) == 0);
    while (true) {
        UNSIGNED16 e = 0; AdsAtEOF(hT, &e);
        if (e) break;
        AdsRecallRecord(hT);
        AdsSkip(hT, 1);
    }
    REQUIRE(AdsWriteRecord(hT) == 0);

    // SET DELETE ON, redelete 1,3,6,7,13,14,15
    AdsShowDeleted(0);
    for (UNSIGNED32 r : dels) {
        REQUIRE(AdsGotoRecord(hT, r) == 0);
        REQUIRE(AdsDeleteRecord(hT) == 0);
    }
    REQUIRE(AdsWriteRecord(hT) == 0);
    REQUIRE(AdsGotoRecord(hT, 16) == 0);  // GOTO past-end -> Limbo

    // Now: DBGOTOP with active TG_C FOR RECNO()<>5 + SET DELETE ON.
    // Expected: recno 2 (first live in B+tree).
    REQUIRE(AdsGotoTop(hT) == 0);
    rprint(hT, "DBGOTOP-after-redeletes");

    UNSIGNED32 r = 0; UNSIGNED16 b = 0, e = 0;
    AdsGetRecordNum(hT, 0, &r);
    AdsAtBOF(hT, &b);
    AdsAtEOF(hT, &e);
    CHECK(r == 2);
    CHECK(b == 0);
    CHECK(e == 0);

    AdsShowDeleted(1);
    AdsCloseTable(hT);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}
