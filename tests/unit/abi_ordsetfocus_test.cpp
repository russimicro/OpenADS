#include "doctest.h"
#include "openads/ace.h"

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

extern "C" {
UNSIGNED32 AdsGetIndexHandleByOrder(ADSHANDLE hTable, UNSIGNED16 usOrder,
                                    ADSHANDLE* phIndex);
}

// M(rddads-compat) — verify CREATE INDEX A then CREATE INDEX B
// makes B the active order, A still reachable via ordinal 1.
TEST_CASE("AdsGetIndexHandleByOrder returns indexes in creation order") {
    auto dir = fs::temp_directory_path() / "openads_ordfocus";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[] = "FNUM,N,10,0;FSTR,C,4,0";
    UNSIGNED8 tname[] = "ord_t";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hTable) == 0);

    // Append three rows so the indexes have keys to walk.
    UNSIGNED8 fnum_n[]  = "FNUM";
    UNSIGNED8 fstr_n[]  = "FSTR";
    for (int i = 1; i <= 3; ++i) {
        REQUIRE(AdsAppendRecord(hTable) == 0);
        AdsSetDouble(hTable, fnum_n, static_cast<double>(i));
        UNSIGNED8 sval[5];
        sval[0] = static_cast<UNSIGNED8>('A' + i - 1);
        sval[1] = sval[2] = sval[3] = sval[0];
        sval[4] = 0;
        AdsSetString(hTable, fstr_n, sval, 4);
    }
    REQUIRE(AdsWriteRecord(hTable) == 0);

    UNSIGNED8 bag[]    = "ord_t.cdx";
    UNSIGNED8 tg_n[]   = "TG_N";
    UNSIGNED8 tg_c[]   = "TG_C";
    UNSIGNED8 expr_n[] = "FNUM";
    UNSIGNED8 expr_c[] = "FSTR";
    ADSHANDLE hIdx_n = 0, hIdx_c = 0;
    REQUIRE(AdsCreateIndex61(hTable, bag, tg_n, expr_n,
                              nullptr, nullptr, 0, 0, &hIdx_n) == 0);
    REQUIRE(AdsCreateIndex61(hTable, bag, tg_c, expr_c,
                              nullptr, nullptr, 0, 0, &hIdx_c) == 0);

    // Ordinal 1 should be the first-created tag, TG_N.
    ADSHANDLE hByOrd = 0;
    REQUIRE(AdsGetIndexHandleByOrder(hTable, 1, &hByOrd) == 0);
    UNSIGNED8 nm[64] = {0};
    UNSIGNED16 nl = sizeof(nm);
    REQUIRE(AdsGetIndexName(hByOrd, nm, &nl) == 0);
    CHECK(std::string(reinterpret_cast<char*>(nm), nl) == "TG_N");

    // Ordinal 2 should be the second-created tag, TG_C.
    REQUIRE(AdsGetIndexHandleByOrder(hTable, 2, &hByOrd) == 0);
    nl = sizeof(nm);
    REQUIRE(AdsGetIndexName(hByOrd, nm, &nl) == 0);
    CHECK(std::string(reinterpret_cast<char*>(nm), nl) == "TG_C");

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}
