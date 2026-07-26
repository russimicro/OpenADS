#include "doctest.h"
#include "openads/ace.h"

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

extern "C" {
UNSIGNED32 AdsGetIndexOrderByHandle(ADSHANDLE hIndex, UNSIGNED16* pusOrder);
UNSIGNED32 AdsGetKeyCount(ADSHANDLE hIndex, UNSIGNED16 usFilterOption,
                          UNSIGNED32* pulCount);
UNSIGNED32 AdsCloseAllIndexes(ADSHANDLE hTable);
}

// Reproduces Harbour rddads' OrdNumber(cTag): it resolves the tag name to
// an index handle via AdsGetIndexHandle, then asks the ordinal of that
// handle via AdsGetIndexOrderByHandle (see contrib/rddads/ads1.c,
// adsOrderInfo / DBOI_NUMBER). A stubbed AdsGetIndexOrderByHandle makes
// OrdNumber() return 0 for *every* tag, including the active one.
TEST_CASE("OrdNumber: AdsGetIndexOrderByHandle returns the 1-based tag ordinal") {
    auto dir = fs::temp_directory_path() / "openads_ordnumber";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "FNUM,N,10,0;FSTR,C,4,0";
    UNSIGNED8 tname[] = "ord";
    ADSHANDLE hT = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hT) == 0);

    // One CDX bag with two tags, created in this order: ORDER1 then ORDER2.
    UNSIGNED8 bag[]   = "ord.cdx";
    UNSIGNED8 tg1[]   = "ORDER1", tg2[] = "ORDER2";
    UNSIGNED8 ex1[]   = "FNUM",   ex2[] = "FSTR";
    ADSHANDLE h1 = 0, h2 = 0;
    REQUIRE(AdsCreateIndex61(hT, bag, tg1, ex1,
                             nullptr, nullptr, 0, 0, &h1) == 0);
    REQUIRE(AdsCreateIndex61(hT, bag, tg2, ex2,
                             nullptr, nullptr, 0, 0, &h2) == 0);

    UNSIGNED16 n = 0;
    REQUIRE(AdsGetNumIndexes(hT, &n) == 0);
    REQUIRE(n == 2);

    auto ord_number = [&](const char* tag) -> int {
        UNSIGNED8 name[ADS_MAX_TAG_NAME + 1] = {0};
        std::memcpy(name, tag, std::strlen(tag) + 1);
        ADSHANDLE hIdx = 0;
        if (AdsGetIndexHandle(hT, name, &hIdx) != 0) return -1;
        UNSIGNED16 ord = 0xFFFF;
        if (AdsGetIndexOrderByHandle(hIdx, &ord) != 0) return -2;
        return static_cast<int>(ord);
    };

    // The bug: OrdNumber("ORDER2") returned 0; both tags must resolve to
    // their real 1-based file ordinal.
    CHECK(ord_number("ORDER1") == 1);
    CHECK(ord_number("ORDER2") == 2);

    // AdsGetIndexHandleByOrder must be the exact inverse of
    // AdsGetIndexOrderByHandle for every ordinal.
    for (UNSIGNED16 k = 1; k <= 2; ++k) {
        ADSHANDLE hk = 0;
        REQUIRE(AdsGetIndexHandleByOrder(hT, k, &hk) == 0);
        UNSIGNED16 back = 0;
        REQUIRE(AdsGetIndexOrderByHandle(hk, &back) == 0);
        CHECK(back == k);
    }

    AdsCloseTable(hT);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

// Regression for "the index is bigger than the table and keeps growing".
// A second tag's CREATE INDEX used to re-park a *duplicate* binding for
// the first tag; every dbAppend then wrote each key into the same on-disk
// CDX tag twice, so the index doubled (and AdsGetNumIndexes over-counted).
// After the fix every tag holds exactly one binding and its key count
// tracks the record count one-to-one.
TEST_CASE("CDX multi-tag: appends do not double-write the index") {
    auto dir = fs::temp_directory_path() / "openads_ordgrow";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "K1,C,4,0;K2,C,4,0";
    UNSIGNED8 tname[] = "grow";
    ADSHANDLE hT = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hT) == 0);

    UNSIGNED8 bag[] = "grow.cdx";
    UNSIGNED8 tg1[] = "ORDER1", tg2[] = "ORDER2";
    UNSIGNED8 ex1[] = "K1",     ex2[] = "K2";
    ADSHANDLE h1 = 0, h2 = 0;
    REQUIRE(AdsCreateIndex61(hT, bag, tg1, ex1,
                             nullptr, nullptr, 0, 0, &h1) == 0);
    REQUIRE(AdsCreateIndex61(hT, bag, tg2, ex2,
                             nullptr, nullptr, 0, 0, &h2) == 0);

    const UNSIGNED32 N = 8;
    UNSIGNED8 f1[] = "K1", f2[] = "K2";
    for (UNSIGNED32 i = 0; i < N; ++i) {
        REQUIRE(AdsAppendRecord(hT) == 0);
        char v1[8], v2[8];
        std::snprintf(v1, sizeof(v1), "A%03u", i);
        std::snprintf(v2, sizeof(v2), "B%03u", i);
        REQUIRE(AdsSetString(hT, f1, reinterpret_cast<UNSIGNED8*>(v1), 4) == 0);
        REQUIRE(AdsSetString(hT, f2, reinterpret_cast<UNSIGNED8*>(v2), 4) == 0);
    }

    UNSIGNED32 rc = 0;
    REQUIRE(AdsGetRecordCount(hT, 0, &rc) == 0);
    CHECK(rc == N);

    // Each tag must hold exactly N keys — not 2*N.
    UNSIGNED8 nm1[ADS_MAX_TAG_NAME + 1] = "ORDER1";
    UNSIGNED8 nm2[ADS_MAX_TAG_NAME + 1] = "ORDER2";
    ADSHANDLE hi1 = 0, hi2 = 0;
    REQUIRE(AdsGetIndexHandle(hT, nm1, &hi1) == 0);
    REQUIRE(AdsGetIndexHandle(hT, nm2, &hi2) == 0);
    UNSIGNED32 kc1 = 0, kc2 = 0;
    REQUIRE(AdsGetKeyCount(hi1, 0, &kc1) == 0);
    REQUIRE(AdsGetKeyCount(hi2, 0, &kc2) == 0);
    CHECK(kc1 == N);
    CHECK(kc2 == N);

    // And the binding set stays at exactly two tags.
    UNSIGNED16 n = 0;
    REQUIRE(AdsGetNumIndexes(hT, &n) == 0);
    CHECK(n == 2);

    AdsCloseTable(hT);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

// Regression for "the .cdx is bigger than the table and keeps growing,
// like in a loop". Re-running `INDEX ON ... TAG ...` for every tag (what
// rddads does on each program start: ORDLSTCLEAR then re-create each tag)
// rebuilt every tag's B+tree; clear_data() dropped the old root but never
// reclaimed its pages, so the bag grew by a full tree on every reindex,
// without bound. After the fix the bag size stabilises across reindex
// cycles.
TEST_CASE("CDX reindex cycle does not leak pages (bag size stabilises)") {
    auto dir = fs::temp_directory_path() / "openads_ordleak";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "K1,C,8,0;K2,C,8,0";
    UNSIGNED8 tname[] = "leak";
    ADSHANDLE hT = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hT) == 0);

    UNSIGNED8 f1[] = "K1", f2[] = "K2";
    const UNSIGNED32 N = 200;
    for (UNSIGNED32 i = 0; i < N; ++i) {
        REQUIRE(AdsAppendRecord(hT) == 0);
        char v1[16], v2[16];
        std::snprintf(v1, sizeof(v1), "K%07u", i);
        std::snprintf(v2, sizeof(v2), "Z%07u", (N - i));
        REQUIRE(AdsSetString(hT, f1, reinterpret_cast<UNSIGNED8*>(v1), 8) == 0);
        REQUIRE(AdsSetString(hT, f2, reinterpret_cast<UNSIGNED8*>(v2), 8) == 0);
    }

    UNSIGNED8 bag[] = "leak.cdx";
    UNSIGNED8 tg1[] = "ORDER1", tg2[] = "ORDER2";
    UNSIGNED8 ex1[] = "K1",     ex2[] = "K2";
    auto bag_path = dir / "leak.cdx";

    auto reindex = [&]() {
        REQUIRE(AdsCloseAllIndexes(hT) == 0);   // rddads ORDLSTCLEAR
        ADSHANDLE h1 = 0, h2 = 0;
        REQUIRE(AdsCreateIndex61(hT, bag, tg1, ex1,
                                 nullptr, nullptr, 0, 0, &h1) == 0);
        REQUIRE(AdsCreateIndex61(hT, bag, tg2, ex2,
                                 nullptr, nullptr, 0, 0, &h2) == 0);
    };

    reindex();
    std::uintmax_t after_first = fs::file_size(bag_path, ec);
    for (int cycle = 0; cycle < 6; ++cycle) reindex();
    std::uintmax_t after_many = fs::file_size(bag_path, ec);

    // Make the leak/no-leak visible in the test log (always printed, not
    // only on failure): before the free-list fix this went e.g.
    // 7680 -> 44544 bytes and kept climbing every reindex; after it the
    // bag size is flat.
    MESSAGE("leak.cdx size after 1 reindex = " << after_first
            << " bytes; after 7 reindex cycles = " << after_many
            << " bytes (delta = " << (static_cast<long long>(after_many)
               - static_cast<long long>(after_first)) << ")");
    INFO("after_first=" << after_first << " after_many=" << after_many);
    // The bag must not balloon with every reindex. Allow modest slack for
    // page-alignment, but nothing like the per-cycle full-tree leak.
    CHECK(after_many <= after_first + 4096);

    AdsCloseTable(hT);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}
