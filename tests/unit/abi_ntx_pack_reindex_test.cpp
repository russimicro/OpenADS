#include "doctest.h"
#include "openads/ace.h"

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>

// Regression coverage for the NTX PACK 5004 defect.
//
// PACK on a DBF that carries an NTX index and has >=1 deleted record
// rebuilds the index via Table::reindex(): step 1 erases every key, step 2
// re-inserts the survivors. The NTX erase walk leaves the file
// "structurally intact" -> root_page_ stays != 0 pointing at a root leaf
// with key_count == 0. NtxIndex::insert only recognised the empty index
// when root_page_ == 0, so the first post-clear re-insert ran
// seek_key_for_write_ into the empty-but-rooted leaf, came back with an
// empty descent stack, and raised:
//
//     code 5004  "NTX insert: empty stack post-seek"
//
// The same flow on CDX is clean. This test pins the NTX path: a PACK that
// reindexes an NTX table must succeed and leave a correct, walkable index.

namespace fs = std::filesystem;

TEST_CASE("PACK reindexes an NTX table with deleted records (no 5004 empty-stack)") {
    auto dir = fs::temp_directory_path() / "openads_ntx_pack_reindex";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "NM,C,4,0";
    UNSIGNED8 tname[] = "data";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_NTX,
                           0, 0, 0, 0, def, &hTable) == 0);

    UNSIGNED8 fld[] = "NM";
    for (const char* k : {"AAAA", "BBBB", "CCCC", "DDDD"}) {   // recnos 1..4
        REQUIRE(AdsAppendRecord(hTable) == 0);
        REQUIRE(AdsSetString(hTable, fld, reinterpret_cast<UNSIGNED8*>(
                    const_cast<char*>(k)), 4) == 0);
    }
    REQUIRE(AdsWriteRecord(hTable) == 0);

    UNSIGNED8 idx_file[32] = "data.ntx";
    UNSIGNED8 tag[16]      = "BYNM";
    UNSIGNED8 expr[16]     = "NM";
    ADSHANDLE hIndex = 0;
    REQUIRE(AdsCreateIndex61(hTable, idx_file, tag, expr,
                             nullptr, nullptr, 0, 1024, &hIndex) == 0);

    // Delete "BBBB" (recno 2), then PACK -> reindex() empties the NTX and
    // re-inserts the 3 survivors. This raised 5004 before the fix.
    REQUIRE(AdsGotoRecord(hTable, 2) == 0);
    REQUIRE(AdsDeleteRecord(hTable) == 0);
    REQUIRE(AdsWriteRecord(hTable) == 0);
    REQUIRE(AdsPackTable(hTable) == 0);                 // RED: returned 5004

    UNSIGNED32 cnt = 0;
    REQUIRE(AdsGetRecordCount(hTable, 0, &cnt) == 0);
    CHECK(cnt == 3u);

    // Walk the rebuilt NTX top->EOF: exactly the 3 survivors, in key order.
    REQUIRE(AdsGotoTop(hTable) == 0);
    const char* expected[] = {"AAAA", "CCCC", "DDDD"};
    int visited = 0;
    UNSIGNED16 eof = 0;
    REQUIRE(AdsAtEOF(hTable, &eof) == 0);
    while (eof == 0 && visited < 50) {
        UNSIGNED8  buf[16] = {0};
        UNSIGNED32 cap = sizeof(buf);
        REQUIRE(AdsGetField(hTable, fld, buf, &cap, 0) == 0);
        if (visited < 3) {
            CHECK(std::string(reinterpret_cast<const char*>(buf), 4)
                  == expected[visited]);
        }
        ++visited;
        REQUIRE(AdsSkip(hTable, 1) == 0);
        REQUIRE(AdsAtEOF(hTable, &eof) == 0);
    }
    CHECK(visited == 3);

    // The deleted key is gone; a survivor is found via the rebuilt index.
    UNSIGNED8 kgone[8] = "BBBB";
    UNSIGNED16 found = 9;
    REQUIRE(AdsSeek(hIndex, kgone, 4, 0, 0 /*hard*/, &found) == 0);
    CHECK(found == 0);

    UNSIGNED8 khit[8] = "CCCC";
    found = 9;
    REQUIRE(AdsSeek(hIndex, khit, 4, 0, 0 /*hard*/, &found) == 0);
    CHECK(found == 1);

    REQUIRE(AdsCloseIndex(hIndex) == 0);
    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}
