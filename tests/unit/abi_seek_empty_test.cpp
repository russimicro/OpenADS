#include "doctest.h"
#include "openads/ace.h"

#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

TEST_CASE("Seek empty key on empty CDX-indexed table reports Limbo") {
    auto dir = fs::temp_directory_path() / "openads_seek_empty";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "FSTR,C,4,0";
    UNSIGNED8 tname[] = "se";
    ADSHANDLE hT = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hT) == 0);

    UNSIGNED8 bag[]    = "se.cdx";
    UNSIGNED8 tag[]    = "TG_C";
    UNSIGNED8 expr[]   = "FSTR";
    ADSHANDLE hI = 0;
    REQUIRE(AdsCreateIndex61(hT, bag, tag, expr,
                             nullptr, nullptr, 0, 0, &hI) == 0);

    UNSIGNED8 key[1] = {0};
    UNSIGNED16 found = 99;
    REQUIRE(AdsSeek(hI, key, 0, ADS_STRINGKEY,
                    /*soft*/ 1, &found) == 0);

    UNSIGNED16 bofv = 99, eofv = 99;
    REQUIRE(AdsAtBOF(hT, &bofv) == 0);
    REQUIRE(AdsAtEOF(hT, &eofv) == 0);
    UNSIGNED32 r = 0;
    AdsGetRecordNum(hT, 0, &r);

    INFO("found=" << found << " bof=" << bofv
         << " eof=" << eofv << " recno=" << r);
    CHECK(bofv == 1);
    CHECK(eofv == 1);
    CHECK(r == 1);

    AdsCloseTable(hT);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}
