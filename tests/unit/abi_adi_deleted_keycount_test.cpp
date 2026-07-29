// AdsGetKeyCount over an ADI (and NTX) order must exclude deleted records
// while SET DELETED is ON, exactly as the CDX branch of the same function
// already does.
//
// The cached-walk fast path added for NTX/ADI returned the raw walk size, so
// OrdKeyCount() over an ADT company disagreed with the same table under
// DBFCDX as soon as anything was deleted. A browse sizes its scrollbar and
// its "record n of m" from that number.

#include "doctest.h"
#include "openads/ace.h"

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

TEST_CASE("ADI: AdsGetKeyCount honours SET DELETED") {
    fs::path tmp = fs::temp_directory_path() / "openads_adi_del_keycount";
    { std::error_code ec; fs::remove_all(tmp, ec); fs::create_directories(tmp, ec); }

    UNSIGNED8 srv[260]{};
    std::memcpy(srv, tmp.string().c_str(), tmp.string().size());
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn)
            == AE_SUCCESS);

    UNSIGNED8 tbl[]    = "del.adt";
    UNSIGNED8 flddef[] = "CCODIGO,Character,10";
    ADSHANDLE hTable   = 0;
    REQUIRE(AdsCreateTable(hConn, tbl, nullptr, ADS_ADT, ADS_ANSI, 0, 0, 0,
                           flddef, &hTable) == AE_SUCCESS);

    const std::uint32_t kRows = 20;
    for (std::uint32_t i = 0; i < kRows; ++i) {
        REQUIRE(AdsAppendRecord(hTable) == AE_SUCCESS);
        char cod[16];
        std::snprintf(cod, sizeof(cod), "C%08u", i);
        AdsSetString(hTable, (UNSIGNED8*)"CCODIGO", (UNSIGNED8*)cod,
                     (UNSIGNED32)std::strlen(cod));
    }
    REQUIRE(AdsWriteRecord(hTable) == AE_SUCCESS);

    UNSIGNED8 idxfile[] = "del.adi";
    ADSHANDLE hIdx = 0;
    REQUIRE(AdsCreateIndex61(hTable, idxfile, (UNSIGNED8*)"TCODIGO",
                             (UNSIGNED8*)"CCODIGO", nullptr, nullptr,
                             0, 0, &hIdx) == AE_SUCCESS);

    // Baseline BEFORE any delete: the order holds one key per record.
    UNSIGNED32 kc0 = 0;
    REQUIRE(AdsGetKeyCount(hIdx, 0, &kc0) == AE_SUCCESS);
    CHECK(kc0 == kRows);

    // Delete 5 rows (records 1..5 in key order — the key ascends with recno).
    AdsShowDeleted(1);
    REQUIRE(AdsGotoTop(hTable) == AE_SUCCESS);
    for (int i = 0; i < 5; ++i) {
        REQUIRE(AdsDeleteRecord(hTable) == AE_SUCCESS);
        REQUIRE(AdsSkip(hTable, 1) == AE_SUCCESS);
    }

    UNSIGNED32 kc = 0;

    SUBCASE("SET DELETED ON: deleted rows are not counted") {
        AdsShowDeleted(0);
        REQUIRE(AdsGetKeyCount(hIdx, 0, &kc) == AE_SUCCESS);
        CHECK(kc == kRows - 5);   // was kRows before the fix
    }

    SUBCASE("SET DELETED OFF: every key is counted") {
        AdsShowDeleted(1);
        REQUIRE(AdsGetKeyCount(hIdx, 0, &kc) == AE_SUCCESS);
        CHECK(kc == kRows);
    }

    AdsShowDeleted(0);
    AdsCloseTable(hTable);
    AdsDisconnect(hConn);
    { std::error_code ec; fs::remove_all(tmp, ec); }
}
