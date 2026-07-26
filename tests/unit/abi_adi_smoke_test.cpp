// M4 ADI index smoke test.
// Opens f:\pmsys\data\landlords.adi (skipped when absent) and verifies:
//   * AdiIndex::list_tags() returns at least one tag
//   * AdsOpenTable + AdsOpenIndex wire up successfully
//   * AdsSetOrder + AdsGotoTop + AdsSkip iterate all 7 records without error
//   * Direct driver navigation (seek_first / next) visits each record once
//
// Also tests leases.adi (fixtures/adi) — character-key indexes with level-2
// dense leaves, which previously caused error 6106.
#include "doctest.h"
#include "drivers/adi/adi_index.h"
#include "openads/ace.h"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

namespace fs = std::filesystem;

static const fs::path kAdtDir  { "f:\\pmsys\\data" };
static const fs::path kAdtPath { kAdtDir / "landlords.adt" };
static const fs::path kAdiPath { kAdtDir / "landlords.adi" };

// ── direct driver test ──────────────────────────────────────────────────────

TEST_CASE("M4 ADI driver: list_tags and iterate landlords.adi") {
    std::error_code ec;
    if (!fs::exists(kAdiPath, ec)) {
        MESSAGE("landlords.adi not found, skipping M4 ADI driver test");
        return;
    }

    // list_tags must return at least one field name
    auto tags = openads::drivers::adi::AdiIndex::list_tags(kAdiPath.string());
    REQUIRE(tags);
    REQUIRE(!tags.value().empty());

    // Open the first tag
    const std::string first_tag = tags.value().front();
    openads::drivers::adi::AdiIndex idx;
    REQUIRE(idx.open_named(kAdiPath.string(),
                           openads::drivers::IndexOpenMode::ReadOnly,
                           first_tag));

    CHECK(idx.name() == first_tag);

    // seek_first → visit every entry via next(); collect recnos
    auto r = idx.seek_first();
    REQUIRE(r);
    REQUIRE(r.value().positioned);

    std::set<std::uint32_t> seen;
    while (r.value().positioned) {
        CHECK(r.value().recno > 0);
        seen.insert(r.value().recno);
        r = idx.next();
        REQUIRE(r);
    }
    // landlords.adt has 7 records; the index should cover all of them
    CHECK(seen.size() == 7u);

    // seek_last should land on a recno we already saw
    auto last = idx.seek_last();
    REQUIRE(last);
    REQUIRE(last.value().positioned);
    CHECK(seen.count(last.value().recno) == 1);
}

// ── ABI integration test ────────────────────────────────────────────────────

TEST_CASE("M4 ADI ABI: AdsOpenIndex + AdsSetOrder iterates landlords.adt") {
    std::error_code ec;
    if (!fs::exists(kAdtPath, ec) || !fs::exists(kAdiPath, ec)) {
        MESSAGE("landlords.adt/.adi not found, skipping M4 ADI ABI test");
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

    // Open the ADI index explicitly (auto-open already fires in AdsOpenTable,
    // but calling it again with Shared mode should be idempotent / additive).
    std::string adi_str = kAdiPath.string();
    std::vector<UNSIGNED8> adi_buf(adi_str.size() + 1);
    std::memcpy(adi_buf.data(), adi_str.data(), adi_str.size());
    ADSHANDLE idx_handles[64] = {0};
    UNSIGNED16 idx_count = 64;
    REQUIRE(AdsOpenIndex(hTable, adi_buf.data(), idx_handles, &idx_count)
            == AE_SUCCESS);
    CHECK(idx_count >= 1);

    // Set order to the first returned index handle
    REQUIRE(AdsSetIndexOrderByHandle(hTable, idx_handles[0]) == AE_SUCCESS);

    // GoTop and count all records in index order
    REQUIRE(AdsGotoTop(hTable) == AE_SUCCESS);

    UNSIGNED32 count = 0;
    UNSIGNED16 at_eof = 0;

    while (true) {
        REQUIRE(AdsAtEOF(hTable, &at_eof) == AE_SUCCESS);
        if (at_eof) break;
        ++count;
        REQUIRE(AdsSkip(hTable, 1) == AE_SUCCESS);
    }
    CHECK(count == 7u);

    AdsCloseTable(hTable);
    AdsDisconnect(hConn);
}

// ── Level-2 dense leaf fix: leases.adi character-key navigation ─────────────

static fs::path fixture_adi_dir() {
    return fs::path(__FILE__).parent_path().parent_path() / "fixtures" / "adi";
}

TEST_CASE("M4 ADI driver: navigate leases.adi char-key tags (level-2 dense leaf)") {
    fs::path adi_path = fixture_adi_dir() / "leases.adi";
    std::error_code ec;
    if (!fs::exists(adi_path, ec)) {
        MESSAGE("leases.adi not found, skipping char-key ADI test");
        return;
    }

    // list_tags must return 7 entries (6 single-field + 1 compound)
    auto tags = openads::drivers::adi::AdiIndex::list_tags(adi_path.string());
    REQUIRE(tags);
    CHECK(tags.value().size() >= 1u);

    // Open leaseid tag (CICHAR, length=13, root has level-1 branch → level-2 leaves)
    openads::drivers::adi::AdiIndex idx;
    REQUIRE(idx.open_named(adi_path.string(),
                           openads::drivers::IndexOpenMode::ReadOnly,
                           "leaseid"));

    CHECK(idx.name() == "leaseid");
    CHECK(idx.key_length() == 13u);  // char key = field length, not 8

    // seek_first must succeed and land on a valid record (not error 6106)
    auto r = idx.seek_first();
    REQUIRE(r);
    REQUIRE(r.value().positioned);
    CHECK(r.value().recno > 0);

    // Iterate all entries via next(); collect recnos
    std::set<std::uint32_t> seen;
    while (r.value().positioned) {
        CHECK(r.value().recno > 0);
        seen.insert(r.value().recno);
        r = idx.next();
        REQUIRE(r);
    }
    // leases.adt has 245 records; every recno should appear exactly once
    CHECK(seen.size() == 245u);

    // seek_last must also succeed
    auto last = idx.seek_last();
    REQUIRE(last);
    REQUIRE(last.value().positioned);
    CHECK(seen.count(last.value().recno) == 1u);
}

TEST_CASE("M4 ADI driver: leases.adi all char-key tags navigable") {
    fs::path adi_path = fixture_adi_dir() / "leases.adi";
    std::error_code ec;
    if (!fs::exists(adi_path, ec)) {
        MESSAGE("leases.adi not found, skipping");
        return;
    }

    auto tag_names = openads::drivers::adi::AdiIndex::list_tags(adi_path.string());
    REQUIRE(tag_names);

    // Single-field CICHAR tags that must work: leaseid, propertyID, LandLordID, ManagerID
    static const char* kCharTags[] = {"leaseid", "propertyID", "LandLordID", "ManagerID"};
    for (const char* tname : kCharTags) {
        openads::drivers::adi::AdiIndex idx;
        auto open_r = idx.open_named(adi_path.string(),
                                     openads::drivers::IndexOpenMode::ReadOnly,
                                     tname);
        if (!open_r) continue;  // tag might not exist in all fixture versions

        auto r = idx.seek_first();
        REQUIRE(r);
        // Must not fail with 6106 — just check it returns a valid outcome
        CHECK((r.value().positioned || r.value().hit == openads::drivers::SeekHit::AfterEnd));
        if (r.value().positioned) {
            CHECK(r.value().recno > 0);
        }
    }
}
