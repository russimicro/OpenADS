#include "doctest.h"
#include "openads/ace.h"
#include "openads/error.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;

namespace {

// Build a minimal DBF with 5 records: records 2 and 4 are deleted ('*'),
// the rest are live (' ').  rec_len = 1 (status) + 4 (CHAR TAG) = 5.
fs::path stage_five_rec_dbf(const fs::path& dir) {
    fs::create_directories(dir);
    auto p = dir / "data.dbf";
    fs::remove(p);

    std::vector<std::uint8_t> file;
    auto push = [&](const void* d, std::size_t n) {
        const auto* b = static_cast<const std::uint8_t*>(d);
        file.insert(file.end(), b, b + n);
    };

    // 32-byte header
    std::array<std::uint8_t, 32> hdr{};
    hdr[0]  = 0x03;           // dBASE III
    hdr[4]  = 5;              // record count (low byte)
    hdr[8]  = 32 + 32 + 1;   // header length
    hdr[9]  = 0;
    hdr[10] = 1 + 4;          // record length: 1 status + 4 chars
    hdr[11] = 0;
    push(hdr.data(), hdr.size());

    // 32-byte field descriptor: CHAR(4) "TAG"
    std::array<std::uint8_t, 32> fd{};
    std::strncpy(reinterpret_cast<char*>(fd.data()), "TAG", 11);
    fd[11] = 'C';
    fd[16] = 4;
    push(fd.data(), fd.size());
    file.push_back(0x0D); // header terminator

    // 5 records — positions 2 and 4 are deleted
    auto rec = [&](bool del, const char* s) {
        file.push_back(del ? '*' : ' ');
        for (int i = 0; i < 4; ++i)
            file.push_back(i < static_cast<int>(std::strlen(s))
                           ? static_cast<std::uint8_t>(s[i]) : ' ');
    };
    rec(false, "AAA1");   // live   — recno 1
    rec(true,  "BBB2");   // deleted — recno 2
    rec(false, "CCC3");   // live   — recno 3
    rec(true,  "DDD4");   // deleted — recno 4
    rec(false, "EEE5");   // live   — recno 5
    file.push_back(0x1A); // EOF marker

    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(file.data()),
        static_cast<std::streamsize>(file.size()));
    return p;
}

} // namespace

// ---------------------------------------------------------------------------
// AdsGetRecordCount + ADS_IGNOREFILTERS always returns the physical count.
// ---------------------------------------------------------------------------
TEST_CASE("AdsGetRecordCount ADS_IGNOREFILTERS returns physical count including deleted") {
    const auto dir = fs::temp_directory_path() / "openads_reccount_ignore";
    std::error_code ec;
    fs::remove_all(dir, ec);
    stage_five_rec_dbf(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    ADSHANDLE hTable = 0;
    UNSIGNED8 name[] = "data";
    REQUIRE(AdsOpenTable(hConn, name, name, ADS_CDX, 1, 1, 0, 1, &hTable) == 0);

    // SET DELETED ON — hide deleted records during navigation
    REQUIRE(AdsShowDeleted(0) == 0);

    UNSIGNED32 cnt = 0;
    REQUIRE(AdsGetRecordCount(hTable, ADS_IGNOREFILTERS, &cnt) == 0);
    // Physical count must include all 5 rows regardless of SET DELETED.
    CHECK(cnt == 5u);

    // Restore default (show deleted)
    REQUIRE(AdsShowDeleted(1) == 0);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// AdsGetRecordCount + ADS_RESPECTFILTERS with SET DELETED ON returns only
// live (non-deleted) records.
// ---------------------------------------------------------------------------
TEST_CASE("AdsGetRecordCount ADS_RESPECTFILTERS counts only live records when SET DELETED ON") {
    const auto dir = fs::temp_directory_path() / "openads_reccount_respect";
    std::error_code ec;
    fs::remove_all(dir, ec);
    stage_five_rec_dbf(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    ADSHANDLE hTable = 0;
    UNSIGNED8 name[] = "data";
    REQUIRE(AdsOpenTable(hConn, name, name, ADS_CDX, 1, 1, 0, 1, &hTable) == 0);

    // Position on record 3 so we can verify cursor is restored afterwards.
    REQUIRE(AdsGotoRecord(hTable, 3) == 0);

    // SET DELETED ON: deleted records are invisible during navigation.
    REQUIRE(AdsShowDeleted(0) == 0);

    UNSIGNED32 cnt = 0;
    REQUIRE(AdsGetRecordCount(hTable, ADS_RESPECTFILTERS, &cnt) == 0);
    // 5 physical - 2 deleted = 3 live rows.
    CHECK(cnt == 3u);

    // Verify the cursor was restored to recno 3.
    UNSIGNED32 recno = 0;
    REQUIRE(AdsGetRecordNum(hTable, ADS_IGNOREFILTERS, &recno) == 0);
    CHECK(recno == 3u);

    // Restore default (show deleted)
    REQUIRE(AdsShowDeleted(1) == 0);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// AdsGetRecordCount + ADS_RESPECTFILTERS with SET DELETED OFF (show deleted)
// falls through to the physical count — deleted rows are visible.
// ---------------------------------------------------------------------------
TEST_CASE("AdsGetRecordCount ADS_RESPECTFILTERS with SET DELETED OFF returns physical count") {
    const auto dir = fs::temp_directory_path() / "openads_reccount_respect_off";
    std::error_code ec;
    fs::remove_all(dir, ec);
    stage_five_rec_dbf(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    ADSHANDLE hTable = 0;
    UNSIGNED8 name[] = "data";
    REQUIRE(AdsOpenTable(hConn, name, name, ADS_CDX, 1, 1, 0, 1, &hTable) == 0);

    // SET DELETED OFF (the default — deleted records are visible).
    REQUIRE(AdsShowDeleted(1) == 0);

    UNSIGNED32 cnt = 0;
    REQUIRE(AdsGetRecordCount(hTable, ADS_RESPECTFILTERS, &cnt) == 0);
    // When deleted rows are visible, the physical count is returned.
    CHECK(cnt == 5u);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// AdsSetRelation must return AE_FUNCTION_NOT_AVAILABLE, not AE_SUCCESS.
// Callers must be able to detect that relation following is unavailable.
// ---------------------------------------------------------------------------
TEST_CASE("AdsSetRelation returns AE_FUNCTION_NOT_AVAILABLE") {
    // AdsSetRelation is not yet implemented. Verify it returns the proper
    // "not available" error code so callers are not silently misled.
    UNSIGNED32 rc = AdsSetRelation(0, 0, nullptr);
    CHECK(rc == openads::AE_FUNCTION_NOT_AVAILABLE);
    CHECK(rc != openads::AE_SUCCESS);
}
