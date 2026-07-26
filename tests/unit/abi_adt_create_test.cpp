// M4 ADT creation test.
// Verifies AdsCreateTable(ADS_ADT) produces a readable .adt file with the
// correct schema, and that records appended via the ABI round-trip correctly.
#include "doctest.h"
#include "openads/ace.h"

#include <cstring>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

TEST_CASE("M4 ADT create: AdsCreateTable(ADS_ADT) + append + reopen") {
    fs::path tmp = fs::temp_directory_path() / "openads_adt_create_test";
    { std::error_code ec; fs::create_directories(tmp, ec); }
    { std::error_code ec;
      fs::remove(tmp / "people.adt", ec);
      fs::remove(tmp / "people.adm", ec); }

    UNSIGNED8 srv[260]{};
    std::memcpy(srv, tmp.string().c_str(), tmp.string().size());
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn)
            == AE_SUCCESS);

    // Field def: Name(CHAR 30) | Age(Numeric 5, maps to INTEGER) | Active(LOGICAL)
    UNSIGNED8 tbl[]    = "people.adt";
    UNSIGNED8 flddef[] = "Name,Character,30;Age,Numeric,5;Active,Logical";
    ADSHANDLE hTable   = 0;
    REQUIRE(AdsCreateTable(hConn, tbl, nullptr, ADS_ADT, ADS_ANSI, 0, 0, 0,
                           flddef, &hTable) == AE_SUCCESS);
    CHECK(hTable != 0);

    // Schema: 3 fields
    UNSIGNED16 nflds = 0;
    REQUIRE(AdsGetNumFields(hTable, &nflds) == AE_SUCCESS);
    CHECK(nflds == 3);

    // Field 1 must be "Name" of type ADS_STRING
    UNSIGNED8  fname[64]{};
    UNSIGNED16 fnlen = sizeof(fname);
    REQUIRE(AdsGetFieldName(hTable, 1, fname, &fnlen) == AE_SUCCESS);
    CHECK(std::string(reinterpret_cast<char*>(fname)) == "Name");

    UNSIGNED16 ftype = 0;
    REQUIRE(AdsGetFieldType(hTable, fname, &ftype) == AE_SUCCESS);
    CHECK(ftype == ADS_STRING);

    // Append record 1: Name="Alice", Age=30, Active=true
    REQUIRE(AdsAppendRecord(hTable) == AE_SUCCESS);
    {
        UNSIGNED8 name_f[]   = "Name";
        UNSIGNED8 name_val[] = "Alice";
        REQUIRE(AdsSetString(hTable, name_f, name_val,
                             static_cast<UNSIGNED32>(std::strlen("Alice")))
                == AE_SUCCESS);

        UNSIGNED8 age_f[] = "Age";
        REQUIRE(AdsSetDouble(hTable, age_f, 30.0) == AE_SUCCESS);

        UNSIGNED8 act_f[] = "Active";
        REQUIRE(AdsSetLogical(hTable, act_f, 1) == AE_SUCCESS);
    }
    REQUIRE(AdsWriteRecord(hTable) == AE_SUCCESS);

    // Append record 2: Name="Bob", Age=25, Active=false
    REQUIRE(AdsAppendRecord(hTable) == AE_SUCCESS);
    {
        UNSIGNED8 name_f[]   = "Name";
        UNSIGNED8 name_val[] = "Bob";
        REQUIRE(AdsSetString(hTable, name_f, name_val,
                             static_cast<UNSIGNED32>(std::strlen("Bob")))
                == AE_SUCCESS);

        UNSIGNED8 age_f[] = "Age";
        REQUIRE(AdsSetDouble(hTable, age_f, 25.0) == AE_SUCCESS);

        UNSIGNED8 act_f[] = "Active";
        REQUIRE(AdsSetLogical(hTable, act_f, 0) == AE_SUCCESS);
    }
    REQUIRE(AdsWriteRecord(hTable) == AE_SUCCESS);

    REQUIRE(AdsCloseTable(hTable) == AE_SUCCESS);

    // ── Reopen and verify ────────────────────────────────────────────────────
    hTable = 0;
    REQUIRE(AdsOpenTable(hConn, tbl, nullptr, ADS_ADT, ADS_ANSI, ADS_READONLY,
                         ADS_COMPATIBLE_LOCKING, ADS_DEFAULT,
                         &hTable) == AE_SUCCESS);

    UNSIGNED32 nrecs = 0;
    REQUIRE(AdsGetRecordCount(hTable, ADS_RESPECTFILTERS, &nrecs) == AE_SUCCESS);
    CHECK(nrecs == 2);

    // Record 1: Name="Alice", Age=30, Active=true
    REQUIRE(AdsGotoRecord(hTable, 1) == AE_SUCCESS);
    {
        UNSIGNED8 name_f[]  = "Name";
        UNSIGNED8 name_buf[64]{};
        UNSIGNED32 name_len = sizeof(name_buf);
        REQUIRE(AdsGetString(hTable, name_f, name_buf, &name_len, 0)
                == AE_SUCCESS);
        CHECK(std::string(reinterpret_cast<char*>(name_buf), name_len) == "Alice");

        UNSIGNED8 age_f[]  = "Age";
        SIGNED32  age_val  = 0;
        REQUIRE(AdsGetLong(hTable, age_f, &age_val) == AE_SUCCESS);
        CHECK(age_val == 30);

        UNSIGNED8  act_f[]  = "Active";
        UNSIGNED16 act_val  = 0;
        REQUIRE(AdsGetLogical(hTable, act_f, &act_val) == AE_SUCCESS);
        CHECK(act_val != 0);
    }

    // Record 2: Name="Bob", Age=25, Active=false
    REQUIRE(AdsGotoRecord(hTable, 2) == AE_SUCCESS);
    {
        UNSIGNED8 name_f[]  = "Name";
        UNSIGNED8 name_buf[64]{};
        UNSIGNED32 name_len = sizeof(name_buf);
        REQUIRE(AdsGetString(hTable, name_f, name_buf, &name_len, 0)
                == AE_SUCCESS);
        CHECK(std::string(reinterpret_cast<char*>(name_buf), name_len) == "Bob");

        UNSIGNED8 age_f[]  = "Age";
        SIGNED32  age_val  = 0;
        REQUIRE(AdsGetLong(hTable, age_f, &age_val) == AE_SUCCESS);
        CHECK(age_val == 25);

        UNSIGNED8  act_f[]  = "Active";
        UNSIGNED16 act_val  = 0;
        REQUIRE(AdsGetLogical(hTable, act_f, &act_val) == AE_SUCCESS);
        CHECK(act_val == 0);
    }

    AdsCloseTable(hTable);
    AdsDisconnect(hConn);
}

// Regression: field-name resolution through the ABI must be case-insensitive
// (native ACE semantics). resolve_field_index used an exact-case compare, so
// AdsGetString/AdsSetString with a case differing from the stored field name
// — and, crucially, CDX/NTX index expressions stored in a different case than
// the (upper-cased) DBF field names — spuriously failed with COLUMN_NOT_FOUND.
TEST_CASE("ABI field-name resolution is case-insensitive") {
    fs::path tmp = fs::temp_directory_path() / "openads_field_ci_test";
    { std::error_code ec; fs::create_directories(tmp, ec);
      fs::remove(tmp / "ci.adt", ec); fs::remove(tmp / "ci.adm", ec); }

    UNSIGNED8 srv[260]{};
    std::memcpy(srv, tmp.string().c_str(), tmp.string().size());
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn)
            == AE_SUCCESS);

    // Stored field name is mixed-case "MixedName".
    UNSIGNED8 tbl[]    = "ci.adt";
    UNSIGNED8 flddef[] = "MixedName,Character,20";
    ADSHANDLE hT = 0;
    REQUIRE(AdsCreateTable(hConn, tbl, nullptr, ADS_ADT, ADS_ANSI, 0, 0, 0,
                           flddef, &hT) == AE_SUCCESS);

    // Write through a DIFFERENT case than stored.
    REQUIRE(AdsAppendRecord(hT) == AE_SUCCESS);
    UNSIGNED8 wf[] = "MIXEDNAME";
    UNSIGNED8 wv[] = "hello";
    REQUIRE(AdsSetString(hT, wf, wv,
                         static_cast<UNSIGNED32>(std::strlen("hello")))
            == AE_SUCCESS);
    REQUIRE(AdsWriteRecord(hT) == AE_SUCCESS);
    REQUIRE(AdsGotoTop(hT) == AE_SUCCESS);

    // Read through several cases — all must resolve to the same field.
    const char* variants[] = {"mixedname", "MIXEDNAME", "MixedName", "mIxEdNaMe"};
    for (const char* v : variants) {
        UNSIGNED8 rf[32]{};
        std::memcpy(rf, v, std::strlen(v));
        UNSIGNED8  rb[64]{};
        UNSIGNED32 rl = sizeof(rb);
        REQUIRE(AdsGetString(hT, rf, rb, &rl, 0) == AE_SUCCESS);
        CHECK(std::string(reinterpret_cast<char*>(rb), rl) == "hello");
    }

    AdsCloseTable(hT);
    AdsDisconnect(hConn);
    { std::error_code ec; fs::remove_all(tmp, ec); }
}

// ---------------------------------------------------------------------------
// Regression test for the reported 5000 on ADS_ADT open of *.DAT tables
// (Russoft ERP convention: data in .DAT even for ADT format, + possible
// stale/high rec_count in header after migration/crash/partial write).
// The driver must cap rec_count_ to what physically fits on disk and
// must succeed the open + first record access without 5000.
// ---------------------------------------------------------------------------
TEST_CASE("ADT open .DAT with inflated header rec_count must not 5000 (cap + lock fix)") {
    fs::path src = "C:/OpenADS/OpenADS-main/testdata/Aquarium/animals.adt";
    REQUIRE(fs::exists(src));

    fs::path tmp = fs::temp_directory_path() / "openads_adt_dat_cap_test";
    { std::error_code ec; fs::create_directories(tmp, ec); }

    fs::path dat = tmp / "cajas.dat";   // exactly the naming the user hits: CAJAS.DAT with ADS_ADT
    { std::error_code ec; fs::remove(dat, ec); }
    fs::copy_file(src, dat, fs::copy_options::overwrite_existing);

    // Force the bad condition the user sees: header claims way more records
    // than the file actually contains (rec_count at offset 24, little-endian).
    {
        std::uint32_t fake = 98765;
        FILE* f = fopen(dat.string().c_str(), "r+b");
        REQUIRE(f != nullptr);
        fseek(f, 24, SEEK_SET);
        uint8_t b[4] = {
            static_cast<uint8_t>(fake & 0xFF),
            static_cast<uint8_t>((fake >> 8) & 0xFF),
            static_cast<uint8_t>((fake >> 16) & 0xFF),
            static_cast<uint8_t>((fake >> 24) & 0xFF)
        };
        fwrite(b, 1, 4, f);
        fclose(f);
    }

    UNSIGNED8 srv[512]{};
    std::memcpy(srv, tmp.string().c_str(), tmp.string().size());
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == AE_SUCCESS);

    ADSHANDLE hT = 0;
    UNSIGNED8 namebuf[64]{};   // pass "cajas.dat" (no path, relative to connect dir)
    std::memcpy(namebuf, "cajas.dat", 9);

    // This is the call that was returning 5000 for the user.
    UNSIGNED32 rcOpen = AdsOpenTable(hConn, namebuf, nullptr,
                                     ADS_ADT, ADS_ANSI, ADS_READONLY,
                                     ADS_COMPATIBLE_LOCKING, ADS_DEFAULT,
                                     &hT);
    REQUIRE(rcOpen == AE_SUCCESS);
    REQUIRE(hT != 0);

    // Must report the REAL count (capped), not the fake 98765.
    UNSIGNED32 nrec = 0;
    UNSIGNED32 rcCount = AdsGetRecordCount(hT, ADS_RESPECTFILTERS, &nrec);
    REQUIRE(rcCount == AE_SUCCESS);
    CHECK(nrec == 7);   // animals.adt has 7 records

    // Exercise first record access (common after open in lUsaTab / RDD).
    UNSIGNED32 rcGoto = AdsGotoRecord(hT, 1);
    REQUIRE(rcGoto == AE_SUCCESS);

    // Read something to force a read_record_raw path.
    UNSIGNED8 fld[] = "Name";   // first field in the sample
    UNSIGNED8 buf[128]{};
    UNSIGNED32 blen = sizeof(buf);
    UNSIGNED32 rcRead = AdsGetString(hT, fld, buf, &blen, 0);
    REQUIRE(rcRead == AE_SUCCESS);

    AdsCloseTable(hT);
    AdsDisconnect(hConn);

    { std::error_code ec; fs::remove_all(tmp, ec); }

    // Also try the user's real file from 0X (if present in this machine)
    {
        const char* realf = "F:\\zerus64\\0X\\bases\\CAJAS.DAT";
        if (fs::exists(realf)) {
            fs::path rdir = fs::path(realf).parent_path();
            UNSIGNED8 sbuf[512]{}; std::string ss = rdir.string(); std::memcpy(sbuf, ss.c_str(), ss.size());
            ADSHANDLE hc=0;
            if (AdsConnect60(sbuf, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hc) == AE_SUCCESS) {
                ADSHANDLE ht=0;
                UNSIGNED8 nm[] = "CAJAS.DAT";
                UNSIGNED32 rrc = AdsOpenTable(hc, nm, nullptr, ADS_ADT, ADS_ANSI, ADS_READONLY, ADS_COMPATIBLE_LOCKING, ADS_DEFAULT, &ht);
                std::string m2 = std::string("REAL 0X CAJAS.DAT AdsOpenTable rc=") + std::to_string(rrc);
                MESSAGE(m2.c_str());
                if (ht) AdsCloseTable(ht);
                AdsDisconnect(hc);
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Real user table test: the actual 0X\BASES\CAJAS.DAT that opens fine with
// real SAP ADS (ADS_ADT) but gives 5000 with OpenADS.
// This exercises the exact file the user has (148 fields, hdr_len=30000,
// no autoinc, no memos).
// ---------------------------------------------------------------------------
TEST_CASE("REAL CAJAS.DAT from 0X with ADS_ADT must open without 5000") {
    const char* real_path = "F:\\zerus64\\0X\\bases\\CAJAS.DAT";
    if (!std::filesystem::exists(real_path)) {
        MESSAGE("Real CAJAS.DAT not present in this env, skipping real-file test");
        return;
    }

    // Connect to the directory that contains CAJAS.DAT
    fs::path dir = fs::path(real_path).parent_path();
    UNSIGNED8 srv[512]{};
    std::string s = dir.string();
    std::memcpy(srv, s.c_str(), s.size());

    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == AE_SUCCESS);

    ADSHANDLE hT = 0;
    // Pass just the leaf name (as the ERP does) or full; try leaf first.
    UNSIGNED8 name[] = "CAJAS.DAT";
    UNSIGNED32 rc = AdsOpenTable(hConn, name, nullptr,
                                 ADS_ADT, ADS_ANSI, ADS_READONLY,
                                 ADS_COMPATIBLE_LOCKING, ADS_DEFAULT, &hT);

    std::string msg = std::string("AdsOpenTable on real CAJAS.DAT rc=") + std::to_string(rc);
    MESSAGE(msg.c_str());
    if (rc != AE_SUCCESS) {
        UNSIGNED32 sub = 0;
        UNSIGNED8 msg[256] = {};
        // best effort
    }
    CHECK(rc == AE_SUCCESS);
    CHECK(hT != 0);

    UNSIGNED16 nf = 0;
    REQUIRE(AdsGetNumFields(hT, &nf) == AE_SUCCESS);
    CHECK(nf == 148);  // we know from header analysis

    UNSIGNED32 nrec = 0;
    REQUIRE(AdsGetRecordCount(hT, ADS_RESPECTFILTERS, &nrec) == AE_SUCCESS);
    CHECK(nrec == 23);

    // Try to position and read first record (what USE + implicit GO often does).
    REQUIRE(AdsGotoRecord(hT, 1) == AE_SUCCESS);

    // Read the first field we saw (CCODIGOCAJ, short char).
    UNSIGNED8 f1[] = "CCODIGOCAJ";
    UNSIGNED8 vbuf[32]{};
    UNSIGNED32 vlen = sizeof(vbuf);
    UNSIGNED32 rdr = AdsGetString(hT, f1, vbuf, &vlen, 0);
    REQUIRE(rdr == AE_SUCCESS);

    AdsCloseTable(hT);
    AdsDisconnect(hConn);
}
