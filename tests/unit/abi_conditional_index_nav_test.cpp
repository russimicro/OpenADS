#include "doctest.h"
#include "openads/ace.h"

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>

// A conditional (FOR) index only indexes the matching rows. The ERP builds
// these as temporary search indexes (OrdCondSet + OrdCreate) and browses them
// in a TXBrowse — SET FILTER is avoided for performance. The browse's
// position math (paint via skip; scrollbar/bookmark via OrdKeyNo / recno)
// must stay mutually consistent over the conditional order, or "select" lands
// on the wrong record (the painted/highlighted row desyncs from the record
// pointer that :bChange captures).
//
// This pins the engine primitives TXBrowse builds on over a conditional
// order: goto_top/skip, OrdKeyNo (AdsGetKeyNum), recno, key count, and the
// recno<->position round-trip (bookmark restore = recno -> AdsGotoRecord)
// must all agree.

namespace fs = std::filesystem;

TEST_CASE("conditional index: nav primitives stay consistent (TXBrowse position math)") {
    auto dir = fs::temp_directory_path() / "openads_cond_index_nav";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "VAL,N,4,0;INC,C,1,0";
    UNSIGNED8 tname[] = "data";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hTable) == 0);

    // recno -> (VAL, INC). Matching rows (INC='Y') interleaved with non-matching:
    //   rec1(10,Y) rec2(20,N) rec3(30,Y) rec4(40,N) rec5(50,Y) rec6(60,N)
    // Conditional index BYVAL FOR INC='Y' -> entries 10(rec1) 30(rec3) 50(rec5).
    UNSIGNED8 fVal[] = "VAL";
    UNSIGNED8 fInc[] = "INC";
    const double vals[6] = {10, 20, 30, 40, 50, 60};
    const char*  incs[6] = {"Y", "N", "Y", "N", "Y", "N"};
    for (int i = 0; i < 6; ++i) {
        REQUIRE(AdsAppendRecord(hTable) == 0);
        AdsSetDouble(hTable, fVal, vals[i]);
        AdsSetString(hTable, fInc, reinterpret_cast<UNSIGNED8*>(
            const_cast<char*>(incs[i])), 1);
    }
    REQUIRE(AdsWriteRecord(hTable) == 0);

    auto idx_path = (dir / "data.cdx").string();
    UNSIGNED8 idx_buf[260];
    std::memcpy(idx_buf, idx_path.c_str(), idx_path.size() + 1);
    UNSIGNED8 tag[64]  = "BYVAL";
    UNSIGNED8 expr[64] = "VAL";
    UNSIGNED8 cond[64] = "INC='Y'";
    ADSHANDLE hIndex = 0;
    REQUIRE(AdsCreateIndex(hTable, idx_buf, tag, expr, cond, 0, 0, &hIndex) == 0);

    // Key count = matching rows only.
    UNSIGNED32 kc = 0;
    REQUIRE(AdsGetKeyCount(hIndex, 0, &kc) == 0);
    CHECK(kc == 3u);

    // Walk top->EOF: must visit recnos 1,3,5 with OrdKeyNo 1,2,3.
    const UNSIGNED32 expect_recno[3] = {1, 3, 5};
    REQUIRE(AdsGotoTop(hTable) == 0);
    for (int i = 0; i < 3; ++i) {
        UNSIGNED32 rn = 0, kn = 0;
        REQUIRE(AdsGetRecordNum(hTable, 0, &rn) == 0);
        REQUIRE(AdsGetKeyNum(hTable, 0, &kn) == 0);
        CHECK(rn == expect_recno[i]);
        CHECK(kn == static_cast<UNSIGNED32>(i + 1));
        if (i < 2) REQUIRE(AdsSkip(hTable, 1) == 0);
    }
    UNSIGNED16 eof = 0;
    REQUIRE(AdsSkip(hTable, 1) == 0);
    REQUIRE(AdsAtEOF(hTable, &eof) == 0);
    CHECK(eof == 1);

    // The "select" round-trip: land on each matching row by RECNO (the bookmark
    // is recno-based), then OrdKeyNo and a forward SKIP must be consistent with
    // that position — this is what desyncs the highlighted row from the pointer.
    for (int i = 0; i < 3; ++i) {
        REQUIRE(AdsGotoRecord(hTable, expect_recno[i]) == 0);
        UNSIGNED32 kn = 0;
        REQUIRE(AdsGetKeyNum(hTable, 0, &kn) == 0);
        CHECK(kn == static_cast<UNSIGNED32>(i + 1));         // ordinal matches the row
        if (i < 2) {
            REQUIRE(AdsSkip(hTable, 1) == 0);                 // next matching, not a sibling
            UNSIGNED32 rn = 0;
            REQUIRE(AdsGetRecordNum(hTable, 0, &rn) == 0);
            CHECK(rn == expect_recno[i + 1]);
        }
    }

    // Bookmark round-trip on the middle matching row (rec3).
    REQUIRE(AdsGotoRecord(hTable, 3) == 0);
    UNSIGNED8 bm[8] = {0};
    UNSIGNED32 bmlen = sizeof(bm);
    REQUIRE(AdsGetBookmark60(hTable, bm, &bmlen) == 0);
    REQUIRE(AdsGotoTop(hTable) == 0);                         // navigate away
    REQUIRE(AdsGotoBookmark60(hTable, bm, bmlen) == 0);
    UNSIGNED32 rn = 0, kn = 0;
    REQUIRE(AdsGetRecordNum(hTable, 0, &rn) == 0);
    REQUIRE(AdsGetKeyNum(hTable, 0, &kn) == 0);
    CHECK(rn == 3u);
    CHECK(kn == 2u);

    REQUIRE(AdsCloseIndex(hIndex) == 0);
    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}
