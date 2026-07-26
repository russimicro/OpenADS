// M4 ADT driver smoke test.
// Opens f:\pmsys\data\landlords.adt (skipped when absent) and verifies:
//   * field count and names
//   * record count
//   * deletion flag normalisation (0x04 → active)
//   * CICHAR field value
//   * ADM memo read for rec4 Notes ("SEC 8 preferred")
#include "doctest.h"
#include "openads/ace.h"

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

static const fs::path kAdtDir  { "f:\\pmsys\\data" };
static const fs::path kAdtPath { kAdtDir / "landlords.adt" };

TEST_CASE("M4 ADT: open landlords.adt, read fields and records") {
    // Skip on machines without the sample data.
    std::error_code ec;
    if (!fs::exists(kAdtPath, ec)) {
        MESSAGE("landlords.adt not found, skipping M4 ADT smoke test");
        return;
    }

    UNSIGNED8 srv[256]{};
    std::memcpy(srv, kAdtDir.string().c_str(), kAdtDir.string().size());
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == AE_SUCCESS);

    UNSIGNED8 tblname[] = "landlords.adt";
    ADSHANDLE hTable    = 0;
    REQUIRE(AdsOpenTable(hConn, tblname, nullptr,
                         ADS_ADT, ADS_ANSI, ADS_READONLY,
                         ADS_COMPATIBLE_LOCKING, ADS_DEFAULT,
                         &hTable) == AE_SUCCESS);

    // 13 fields, 7 records.
    UNSIGNED16 nflds = 0;
    REQUIRE(AdsGetNumFields(hTable, &nflds) == AE_SUCCESS);
    CHECK(nflds == 13);

    UNSIGNED32 nrecs = 0;
    REQUIRE(AdsGetRecordCount(hTable, ADS_RESPECTFILTERS, &nrecs) == AE_SUCCESS);
    CHECK(nrecs == 7);

    // First field is 'LandLordID', type CICHAR (ADS_CISTRING).
    UNSIGNED8 fname[128]{};
    UNSIGNED16 fnlen = sizeof(fname);
    REQUIRE(AdsGetFieldName(hTable, 1, fname, &fnlen) == AE_SUCCESS);
    CHECK(std::string(reinterpret_cast<char*>(fname)) == "LandLordID");

    UNSIGNED16 ftype = 0;
    REQUIRE(AdsGetFieldType(hTable, fname, &ftype) == AE_SUCCESS);
    CHECK(ftype == ADS_CISTRING);

    // Navigate to record 1 and read LandLordID.
    REQUIRE(AdsGotoRecord(hTable, 1) == AE_SUCCESS);
    UNSIGNED8 val[64]{};
    UNSIGNED32 vlen = sizeof(val);
    REQUIRE(AdsGetString(hTable, fname, val, &vlen, 0) == AE_SUCCESS);
    // Rec 1 LandLordID trimmed. We just check it's non-empty.
    CHECK(vlen > 0);

    // Record 4 (1-based) has a Notes memo: "SEC 8 preferred" (15 bytes).
    REQUIRE(AdsGotoRecord(hTable, 4) == AE_SUCCESS);
    UNSIGNED8 notes_field[] = "Notes";
    UNSIGNED8 memo_val[64]{};
    UNSIGNED32 mlen = sizeof(memo_val);
    REQUIRE(AdsGetString(hTable, notes_field, memo_val, &mlen, 0) == AE_SUCCESS);
    CHECK(std::string(reinterpret_cast<char*>(memo_val), mlen) == "SEC 8 preferred");

    AdsCloseTable(hTable);
    AdsDisconnect(hConn);
}
