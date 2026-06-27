#include "doctest.h"
#include "openads/ace.h"

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>

// Re-filter regression (the ERP's real path, FW_FUNCSST1.PRG BuscaGenericaBrowse).
// The browse builds a temporary conditional ("search") index with
// OrdCondSet + OrdCreate, ALWAYS reusing the SAME bag file (cFilInd) and the
// SAME tag "ORDERX". Switching the filter to another column calls OrdCreate
// again on the same bag+tag with a NEW key expression and NEW FOR condition,
// WITHOUT destroying the previous one. rddads' OrdCreate maps to
// AdsCreateIndex61 (verified: rddads.lib imports AdsCreateIndex61).
//
// Crucially the ERP's key expression DEPENDS ON THE COLUMN TYPE:
//   character -> bare field             (text key, width = field len)
//   numeric   -> STR(field, nLonCam)    (text key, different width)
//   date      -> DTOS(field)            (text key, width 8)
// So a re-filter from a char column to a numeric column OVERWRITES tag ORDERX
// with a DIFFERENT key expression AND a DIFFERENT key length. The overwrite
// must fully replace the tag's on-disk B+tree; a stale tree or a key-length
// mismatch leaves the browse mis-ordered ("se desordeno").

namespace fs = std::filesystem;

namespace {
UNSIGNED32 ordcreate(ADSHANDLE hTable, const std::string& bag,
                     const char* tag, const char* expr, const char* cond,
                     ADSHANDLE* phIndex) {
    UNSIGNED8 bag_buf[260];
    std::memcpy(bag_buf, bag.c_str(), bag.size() + 1);
    UNSIGNED8 tag_buf[64];   std::memcpy(tag_buf, tag, std::strlen(tag) + 1);
    UNSIGNED8 expr_buf[128]; std::memcpy(expr_buf, expr, std::strlen(expr) + 1);
    UNSIGNED8 cond_buf[128]; std::memcpy(cond_buf, cond, std::strlen(cond) + 1);
    return AdsCreateIndex61(hTable, bag_buf, tag_buf, expr_buf, cond_buf,
                            nullptr, 0, 0, phIndex);
}
} // namespace

TEST_CASE("conditional re-filter char->numeric (same bag+tag, key-len changes)") {
    auto dir = fs::temp_directory_path() / "openads_cond_refilter_types";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    // NAME (char 6), VAL (num 4), INC (char 1).
    UNSIGNED8 def[]   = "NAME,C,6,0;VAL,N,4,0;INC,C,1,0";
    UNSIGNED8 tname[] = "data";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hTable) == 0);

    //   rec1(PERA ,10,Y) rec2(MANGO,20,N) rec3(UVA  ,30,Y)
    //   rec4(KIWI ,40,N) rec5(BANANA? no, 6 chars) -> use distinct names
    //   rec5(FRESA,50,Y)
    UNSIGNED8 fName[] = "NAME", fVal[] = "VAL", fInc[] = "INC";
    const char* names[5] = {"PERA",  "MANGO", "UVA",   "KIWI",  "FRESA"};
    const double vals[5] = {10,      20,      30,      40,      50};
    const char*  incs[5] = {"Y",     "N",     "Y",     "N",     "Y"};
    for (int i = 0; i < 5; ++i) {
        REQUIRE(AdsAppendRecord(hTable) == 0);
        AdsSetString(hTable, fName, reinterpret_cast<UNSIGNED8*>(
            const_cast<char*>(names[i])),
            static_cast<UNSIGNED32>(std::strlen(names[i])));
        AdsSetDouble(hTable, fVal, vals[i]);
        AdsSetString(hTable, fInc, reinterpret_cast<UNSIGNED8*>(
            const_cast<char*>(incs[i])), 1);
    }
    REQUIRE(AdsWriteRecord(hTable) == 0);

    const auto bag = (dir / "filtmp.cdx").string();

    // -- Filter A: ORDERX = NAME (char, bare) FOR INC='Y'.
    //    matching r1(PERA) r3(UVA) r5(FRESA); NAME order FRESA(r5) PERA(r1) UVA(r3).
    ADSHANDLE hA = 0;
    REQUIRE(ordcreate(hTable, bag, "ORDERX", "NAME", "INC='Y'", &hA) == 0);
    UNSIGNED32 kc = 0;
    REQUIRE(AdsGetKeyCount(hA, 0, &kc) == 0);
    CHECK(kc == 3u);
    REQUIRE(AdsGotoTop(hTable) == 0);
    UNSIGNED32 rn = 0;
    REQUIRE(AdsGetRecordNum(hTable, 0, &rn) == 0);
    CHECK(rn == 5u);                                   // FRESA sorts first

    // rddads' OrdCreate cycle calls ORDLSTCLEAR (AdsCloseAllIndexes) BEFORE the
    // next AdsCreateIndex61 — every binding is dropped, but the temp bag still
    // holds the old ORDERX tag on disk. The re-filter rebuilds ORDERX in place.
    REQUIRE(AdsCloseAllIndexes(hTable) == 0);

    // -- Filter B (same bag+tag): ORDERX = STR(VAL,8) FOR VAL>=20.
    //    Key expr/length differ from A. matching r2..r5; VAL order 20,30,40,50.
    ADSHANDLE hB = 0;
    REQUIRE(ordcreate(hTable, bag, "ORDERX", "STR(VAL,8)", "VAL>=20", &hB) == 0);

    UNSIGNED32 kcB = 0;
    REQUIRE(AdsGetKeyCount(hB, 0, &kcB) == 0);
    CHECK(kcB == 4u);

    const UNSIGNED32 expect_recno[4] = {2, 3, 4, 5};   // VAL 20,30,40,50
    REQUIRE(AdsGotoTop(hTable) == 0);
    for (int i = 0; i < 4; ++i) {
        UNSIGNED32 r = 0, kn = 0;
        REQUIRE(AdsGetRecordNum(hTable, 0, &r) == 0);
        REQUIRE(AdsGetKeyNum(hTable, 0, &kn) == 0);
        CHECK(r == expect_recno[i]);
        CHECK(kn == static_cast<UNSIGNED32>(i + 1));
        if (i < 3) REQUIRE(AdsSkip(hTable, 1) == 0);
    }
    UNSIGNED16 eof = 0;
    REQUIRE(AdsSkip(hTable, 1) == 0);
    REQUIRE(AdsAtEOF(hTable, &eof) == 0);
    CHECK(eof == 1);

    UNSIGNED16 nidx = 0;
    REQUIRE(AdsGetNumIndexes(hTable, &nidx) == 0);
    CHECK(nidx == 1u);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}
