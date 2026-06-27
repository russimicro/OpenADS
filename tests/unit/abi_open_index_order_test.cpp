#include "doctest.h"
#include "openads/ace.h"

#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

TEST_CASE("AdsOpenIndex returns ahIndex in CDX struct-tag insertion order") {
    auto dir = fs::temp_directory_path() / "openads_oi";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "FNUM,N,10,0;FSTR,C,4,0";
    UNSIGNED8 tname[] = "oi";
    ADSHANDLE hT = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hT) == 0);

    UNSIGNED8 bag[]    = "oi.cdx";
    UNSIGNED8 tg_n[]   = "TG_N", tg_c[] = "TG_C";
    UNSIGNED8 expr_n[] = "FNUM",  expr_c[] = "FSTR";
    ADSHANDLE hN = 0, hC = 0;
    REQUIRE(AdsCreateIndex61(hT, bag, tg_n, expr_n,
                             nullptr, nullptr, 0, 0, &hN) == 0);
    REQUIRE(AdsCreateIndex61(hT, bag, tg_c, expr_c,
                             nullptr, nullptr, 0, 0, &hC) == 0);

    ADSHANDLE arr[8] = {0};
    UNSIGNED16 cap = 8;
    REQUIRE(AdsOpenIndex(hT, bag, arr, &cap) == 0);
    INFO("count=" << cap);
    REQUIRE(cap == 2);

    UNSIGNED8 nm[64] = {0};
    UNSIGNED16 nl = sizeof(nm);
    REQUIRE(AdsGetIndexName(arr[0], nm, &nl) == 0);
    std::string first(reinterpret_cast<char*>(nm), nl);
    nl = sizeof(nm);
    REQUIRE(AdsGetIndexName(arr[1], nm, &nl) == 0);
    std::string second(reinterpret_cast<char*>(nm), nl);
    INFO("ahIndex order: [" << first << "," << second << "]");
    CHECK(first == "TG_N");
    CHECK(second == "TG_C");

    AdsCloseTable(hT);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}
