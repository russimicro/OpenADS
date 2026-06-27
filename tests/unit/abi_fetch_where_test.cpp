// tests/unit/abi_fetch_where_test.cpp
// Task 2 ABI verification: AdsFetchWhere* result-set exports.
// Task 3 ABI orchestration contract: COUNT / LOCATE / multi-batch scan.
//
// Test cases:
//   1. Local table: filtered batch with field values and per-row recnos.
//   2. Remote wire: filtered batch with field values and per-row recnos.
//   3. Remote wire: count-only (pszCols = nullptr → no column data).
//   4. FetchWhere orchestration: COUNT via maxRows=UINT32_MAX + no cols.
//   5. FetchWhere orchestration: LOCATE via maxRows=1 + WANT_RECNO + AdsGotoRecord.
//   6. FetchWhere orchestration: scan batching covers matches exactly, no gaps/dups.
//
// Row indices passed to the accessor functions are 0-based.
// Orchestration tests (4-6) encode the sequence the patched ads1.c will drive.
#include "doctest.h"
#include "network/server.h"
#include "openads/ace.h"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// ── fixture helpers ──────────────────────────────────────────────────────────
namespace {

fs::path fw_tmp_dir() {
    return fs::temp_directory_path() / "openads_fw_abi_test";
}

void fw_wipe() {
    std::error_code ec;
    fs::remove_all(fw_tmp_dir(), ec);
    fs::create_directories(fw_tmp_dir(), ec);
}

// Seed a local CDX/DBF table "fw.dbf" with three records:
//   recno 1 → NM = "A"
//   recno 2 → NM = "B"
//   recno 3 → NM = "C"
void seed_nm_fixture(const fs::path& dir) {
    UNSIGNED8 srv[260]{};
    std::memcpy(srv, dir.string().c_str(), dir.string().size());
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == AE_SUCCESS);

    UNSIGNED8 def[]   = "NM,C,4,0";
    UNSIGNED8 tname[] = "fw.dbf";
    ADSHANDLE hTable  = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX, ADS_ANSI,
                           0, 0, 0, def, &hTable) == AE_SUCCESS);

    UNSIGNED8 fld[] = "NM";
    auto set_nm = [&](const char* val) {
        UNSIGNED32 len = static_cast<UNSIGNED32>(std::strlen(val));
        REQUIRE(AdsSetString(hTable, fld,
                             reinterpret_cast<UNSIGNED8*>(const_cast<char*>(val)),
                             len) == AE_SUCCESS);
    };

    REQUIRE(AdsAppendRecord(hTable) == AE_SUCCESS);
    set_nm("A"); REQUIRE(AdsWriteRecord(hTable) == AE_SUCCESS);

    REQUIRE(AdsAppendRecord(hTable) == AE_SUCCESS);
    set_nm("B"); REQUIRE(AdsWriteRecord(hTable) == AE_SUCCESS);

    REQUIRE(AdsAppendRecord(hTable) == AE_SUCCESS);
    set_nm("C"); REQUIRE(AdsWriteRecord(hTable) == AE_SUCCESS);

    REQUIRE(AdsCloseTable(hTable) == AE_SUCCESS);
    REQUIRE(AdsDisconnect(hConn) == AE_SUCCESS);
}

// Trim trailing ASCII spaces (CHARACTER fields may be padded to field width).
std::string trim_right(std::string s) {
    while (!s.empty() && s.back() == ' ') s.pop_back();
    return s;
}

} // namespace

// ── 1. Local table: filtered batch + recnos + field access ──────────────────
TEST_CASE("AdsFetchWhere local: filtered batch with recnos and field access") {
    fw_wipe();
    auto dir = fw_tmp_dir();
    seed_nm_fixture(dir);

    UNSIGNED8 srv[260]{};
    std::memcpy(srv, dir.string().c_str(), dir.string().size());
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == AE_SUCCESS);

    UNSIGNED8 tname[] = "fw.dbf";
    ADSHANDLE hTable  = 0;
    REQUIRE(AdsOpenTable(hConn, tname, nullptr, ADS_CDX, ADS_ANSI, ADS_SHARED,
                         ADS_COMPATIBLE_LOCKING, ADS_DEFAULT, &hTable)
            == AE_SUCCESS);

    UNSIGNED8 expr[] = "NM >= 'B'";
    UNSIGNED8 cols[] = "NM";
    ADSHANDLE hRes   = 0;
    REQUIRE(AdsFetchWhere(hTable, expr, cols, 100, 0x01u, &hRes) == AE_SUCCESS);

    UNSIGNED32 nrows = 0;
    REQUIRE(AdsFetchWhereRows(hRes, &nrows) == AE_SUCCESS);
    CHECK(nrows == 2u);

    UNSIGNED32 rec0 = 0, rec1 = 0;
    REQUIRE(AdsFetchWhereRecno(hRes, 0, &rec0) == AE_SUCCESS);
    REQUIRE(AdsFetchWhereRecno(hRes, 1, &rec1) == AE_SUCCESS);
    CHECK(rec0 == 2u);
    CHECK(rec1 == 3u);

    UNSIGNED8 nm_fld[] = "NM";
    UNSIGNED8 buf[16]{};
    UNSIGNED16 blen = sizeof(buf);
    REQUIRE(AdsFetchWhereField(hRes, 0, nm_fld, buf, &blen) == AE_SUCCESS);
    CHECK(trim_right(std::string(reinterpret_cast<char*>(buf), blen)) == "B");

    blen = sizeof(buf);
    REQUIRE(AdsFetchWhereField(hRes, 1, nm_fld, buf, &blen) == AE_SUCCESS);
    CHECK(trim_right(std::string(reinterpret_cast<char*>(buf), blen)) == "C");

    UNSIGNED16 eof_flag = 0;
    REQUIRE(AdsFetchWhereEof(hRes, &eof_flag) == AE_SUCCESS);
    CHECK(eof_flag == 1u);

    REQUIRE(AdsFetchWhereClose(hRes) == AE_SUCCESS);
    REQUIRE(AdsCloseTable(hTable) == AE_SUCCESS);
    REQUIRE(AdsDisconnect(hConn) == AE_SUCCESS);
}

// ── 2. Remote wire: filtered batch + recnos + field access ──────────────────
TEST_CASE("AdsFetchWhere remote wire: filtered batch with recnos and field access") {
    fw_wipe();
    auto dir = fw_tmp_dir();
    seed_nm_fixture(dir);

    openads::network::Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    char uri[512];
    std::snprintf(uri, sizeof(uri), "tcp://127.0.0.1:%u/%s",
                  static_cast<unsigned>(srv.port()), dir.string().c_str());

    UNSIGNED8 srvbuf[512]{};
    std::memcpy(srvbuf, uri, std::strlen(uri) + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srvbuf, ADS_REMOTE_SERVER, nullptr, nullptr, 0, &hConn)
            == AE_SUCCESS);

    UNSIGNED8 tname[] = "fw.dbf";
    ADSHANDLE hTable  = 0;
    REQUIRE(AdsOpenTable(hConn, tname, nullptr, ADS_CDX, ADS_ANSI, ADS_SHARED,
                         ADS_COMPATIBLE_LOCKING, ADS_DEFAULT, &hTable)
            == AE_SUCCESS);

    // Position at top before the filtered scan.
    REQUIRE(AdsGotoTop(hTable) == AE_SUCCESS);

    // Fetch rows where NM >= 'B', requesting column "NM" and per-row recnos.
    UNSIGNED8 expr[] = "NM >= 'B'";
    UNSIGNED8 cols[] = "NM";
    ADSHANDLE hRes   = 0;
    UNSIGNED32 rc = AdsFetchWhere(hTable, expr, cols,
                                  /*ulMaxRows*/ 100,
                                  /*ulFlags*/  0x01u, // WANT_RECNO
                                  &hRes);
    REQUIRE(rc == AE_SUCCESS);
    CHECK(hRes != 0);

    // Matched rows: "B" (recno 2) and "C" (recno 3) → 2 rows.
    UNSIGNED32 nrows = 0;
    REQUIRE(AdsFetchWhereRows(hRes, &nrows) == AE_SUCCESS);
    CHECK(nrows == 2u);

    // Per-row record numbers (1-based DBF recnos).
    UNSIGNED32 rec0 = 0, rec1 = 0;
    REQUIRE(AdsFetchWhereRecno(hRes, 0, &rec0) == AE_SUCCESS);
    REQUIRE(AdsFetchWhereRecno(hRes, 1, &rec1) == AE_SUCCESS);
    CHECK(rec0 == 2u);  // record 2 holds "B"
    CHECK(rec1 == 3u);  // record 3 holds "C"

    // Field values for column "NM" (may be space-padded to 4 chars).
    UNSIGNED8 nm_fld[] = "NM";
    UNSIGNED8 buf[32]{};
    UNSIGNED16 blen = sizeof(buf);
    REQUIRE(AdsFetchWhereField(hRes, 0, nm_fld, buf, &blen) == AE_SUCCESS);
    CHECK(trim_right(std::string(reinterpret_cast<char*>(buf), blen)) == "B");

    blen = sizeof(buf); std::memset(buf, 0, sizeof(buf));
    REQUIRE(AdsFetchWhereField(hRes, 1, nm_fld, buf, &blen) == AE_SUCCESS);
    CHECK(trim_right(std::string(reinterpret_cast<char*>(buf), blen)) == "C");

    // EOF: all three records were examined → server reached end-of-table.
    UNSIGNED16 eof_flag = 0;
    REQUIRE(AdsFetchWhereEof(hRes, &eof_flag) == AE_SUCCESS);
    CHECK(eof_flag != 0);

    // Close frees the handle; subsequent accessor calls must fail.
    REQUIRE(AdsFetchWhereClose(hRes) == AE_SUCCESS);
    UNSIGNED32 dummy = 0;
    CHECK(AdsFetchWhereRows(hRes, &dummy) != AE_SUCCESS);

    REQUIRE(AdsCloseTable(hTable) == AE_SUCCESS);
    REQUIRE(AdsDisconnect(hConn) == AE_SUCCESS);
    srv.stop();
}

// ── 3. Remote wire: count-only (no columns, no recnos) ──────────────────────
TEST_CASE("AdsFetchWhere remote wire: count-only (no columns, flags=0)") {
    fw_wipe();
    auto dir = fw_tmp_dir();
    seed_nm_fixture(dir);

    openads::network::Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    char uri[512];
    std::snprintf(uri, sizeof(uri), "tcp://127.0.0.1:%u/%s",
                  static_cast<unsigned>(srv.port()), dir.string().c_str());

    UNSIGNED8 srvbuf[512]{};
    std::memcpy(srvbuf, uri, std::strlen(uri) + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srvbuf, ADS_REMOTE_SERVER, nullptr, nullptr, 0, &hConn)
            == AE_SUCCESS);

    UNSIGNED8 tname[] = "fw.dbf";
    ADSHANDLE hTable  = 0;
    REQUIRE(AdsOpenTable(hConn, tname, nullptr, ADS_CDX, ADS_ANSI, ADS_SHARED,
                         ADS_COMPATIBLE_LOCKING, ADS_DEFAULT, &hTable)
            == AE_SUCCESS);

    REQUIRE(AdsGotoTop(hTable) == AE_SUCCESS);

    // pszCols = nullptr → no column data; flags = 0 → no recnos.
    UNSIGNED8 expr[] = "NM >= 'B'";
    ADSHANDLE hRes   = 0;
    REQUIRE(AdsFetchWhere(hTable, expr, nullptr, 100, 0, &hRes) == AE_SUCCESS);

    UNSIGNED32 nrows = 0;
    REQUIRE(AdsFetchWhereRows(hRes, &nrows) == AE_SUCCESS);
    CHECK(nrows == 2u);  // "B" and "C" matched

    // EOF: entire table was scanned.
    UNSIGNED16 eof_flag = 0;
    REQUIRE(AdsFetchWhereEof(hRes, &eof_flag) == AE_SUCCESS);
    CHECK(eof_flag != 0);

    REQUIRE(AdsFetchWhereClose(hRes) == AE_SUCCESS);
    REQUIRE(AdsCloseTable(hTable) == AE_SUCCESS);
    REQUIRE(AdsDisconnect(hConn) == AE_SUCCESS);
    srv.stop();
}

// ── Shared helper: open a remote table over a fresh server ───────────────────
namespace {

struct RemoteFixture {
    openads::network::Server srv;
    ADSHANDLE hConn  = 0;
    ADSHANDLE hTable = 0;

    // Returns false if setup failed (REQUIRE inside — propagates to doctest).
    bool open(const fs::path& dir, const char* tname_str) {
        REQUIRE(srv.start("127.0.0.1", 0).has_value());
        char uri[512];
        std::snprintf(uri, sizeof(uri), "tcp://127.0.0.1:%u/%s",
                      static_cast<unsigned>(srv.port()), dir.string().c_str());
        UNSIGNED8 srvbuf[512]{};
        std::memcpy(srvbuf, uri, std::strlen(uri) + 1);
        REQUIRE(AdsConnect60(srvbuf, ADS_REMOTE_SERVER, nullptr, nullptr, 0, &hConn)
                == AE_SUCCESS);
        UNSIGNED8 tname[64]{};
        std::memcpy(tname, tname_str, std::strlen(tname_str));
        REQUIRE(AdsOpenTable(hConn, tname, nullptr, ADS_CDX, ADS_ANSI, ADS_SHARED,
                             ADS_COMPATIBLE_LOCKING, ADS_DEFAULT, &hTable)
                == AE_SUCCESS);
        return true;
    }

    void close() {
        if (hTable) AdsCloseTable(hTable);
        if (hConn)  AdsDisconnect(hConn);
        srv.stop();
    }
};

} // namespace

// ── 4. Orchestration: COUNT ──────────────────────────────────────────────────
// Contract: AdsFetchWhere(maxRows=UINT32_MAX, pszCols=NULL, flags=0) followed
// by AdsFetchWhereRows yields the exact match count.  This is the rddads
// COUNT-FOR code path.  Server must exhaust the table (eof=1).
TEST_CASE("AdsFetchWhere orchestration: COUNT via maxRows=UINT32_MAX no cols") {
    fw_wipe();
    auto dir = fw_tmp_dir();
    seed_nm_fixture(dir);

    RemoteFixture fix;
    fix.open(dir, "fw.dbf");

    // Position at top to guarantee a full scan.
    REQUIRE(AdsGotoTop(fix.hTable) == AE_SUCCESS);

    UNSIGNED8  expr[] = "NM >= 'B'";
    ADSHANDLE  hRes   = 0;
    // maxRows = 0xFFFFFFFF → collect everything; pszCols = NULL → count-only mode.
    REQUIRE(AdsFetchWhere(fix.hTable, expr, nullptr,
                          0xFFFFFFFFu, /*flags*/ 0, &hRes) == AE_SUCCESS);
    CHECK(hRes != 0);

    // AdsFetchWhereRows must equal the true match count (2 out of 3 records).
    UNSIGNED32 nrows = 0;
    REQUIRE(AdsFetchWhereRows(hRes, &nrows) == AE_SUCCESS);
    CHECK(nrows == 2u);

    // Server must have reached EOF (all records examined).
    UNSIGNED16 eof_flag = 0;
    REQUIRE(AdsFetchWhereEof(hRes, &eof_flag) == AE_SUCCESS);
    CHECK(eof_flag != 0);

    REQUIRE(AdsFetchWhereClose(hRes) == AE_SUCCESS);
    // Post-close: any accessor must fail.
    UNSIGNED32 dummy = 0;
    CHECK(AdsFetchWhereRows(hRes, &dummy) != AE_SUCCESS);

    fix.close();
}

// ── 5. Orchestration: LOCATE ─────────────────────────────────────────────────
// Contract: AdsFetchWhere(maxRows=1, pszCols=NULL, WANT_RECNO) yields the
// first matching record's recno in AdsFetchWhereRecno(0), and AdsGotoRecord
// to that recno positions on a row satisfying the predicate.
TEST_CASE("AdsFetchWhere orchestration: LOCATE via maxRows=1 WANT_RECNO AdsGotoRecord") {
    fw_wipe();
    auto dir = fw_tmp_dir();
    seed_nm_fixture(dir);

    RemoteFixture fix;
    fix.open(dir, "fw.dbf");

    REQUIRE(AdsGotoTop(fix.hTable) == AE_SUCCESS);

    UNSIGNED8 expr[] = "NM >= 'B'";
    ADSHANDLE hRes   = 0;
    // maxRows=1 → stop at first match; WANT_RECNO → include recno in reply.
    REQUIRE(AdsFetchWhere(fix.hTable, expr, nullptr,
                          1u, /*flags WANT_RECNO*/ 0x01u, &hRes) == AE_SUCCESS);
    CHECK(hRes != 0);

    UNSIGNED32 nrows = 0;
    REQUIRE(AdsFetchWhereRows(hRes, &nrows) == AE_SUCCESS);
    CHECK(nrows == 1u);   // maxRows=1 capped the result to one row

    // AdsFetchWhereRecno(0) is the first match's 1-based DBF recno.
    UNSIGNED32 rec = 0;
    REQUIRE(AdsFetchWhereRecno(hRes, 0, &rec) == AE_SUCCESS);
    CHECK(rec == 2u);     // "B" is at recno 2

    REQUIRE(AdsFetchWhereClose(hRes) == AE_SUCCESS);

    // Reposition to that recno and confirm the row satisfies "NM >= 'B'".
    REQUIRE(AdsGotoRecord(fix.hTable, rec) == AE_SUCCESS);
    UNSIGNED8  fld[]  = "NM";
    UNSIGNED8  buf[32]{};
    UNSIGNED32 blen   = sizeof(buf);
    REQUIRE(AdsGetString(fix.hTable, fld, buf, &blen, 0) == AE_SUCCESS);
    // Value must be >= 'B' (confirming the predicate holds on the repositioned row).
    CHECK(trim_right(std::string(reinterpret_cast<char*>(buf), blen)) >= "B");

    fix.close();
}

// ── 6. Orchestration: scan batching ──────────────────────────────────────────
// Contract: repeated AdsFetchWhere(maxRows=1, all cols, WANT_RECNO) calls,
// each resuming the server-side cursor from where the prior call left off,
// must cover EXACTLY the matching records with no duplicates and no gaps.
// This models the rddads forward-scan batch read-ahead path.
TEST_CASE("AdsFetchWhere orchestration: scan batching covers matches exactly no dups") {
    fw_wipe();
    auto dir = fw_tmp_dir();
    seed_nm_fixture(dir);

    RemoteFixture fix;
    fix.open(dir, "fw.dbf");

    REQUIRE(AdsGotoTop(fix.hTable) == AE_SUCCESS);

    UNSIGNED8 expr[] = "NM >= 'B'";
    UNSIGNED8 cols[] = "NM";

    // Collect (recno, NM-value) pairs across all batches of size 1.
    // The server cursor is stateful per-session: after each AdsFetchWhere call
    // the cursor is left past the last examined record, so the next call resumes.
    std::vector<std::uint32_t> all_recnos;
    std::vector<std::string>   all_nms;

    for (;;) {
        ADSHANDLE  hRes  = 0;
        UNSIGNED32 rc    = AdsFetchWhere(fix.hTable, expr, cols,
                                         /*maxRows*/ 1u,
                                         /*flags WANT_RECNO*/ 0x01u,
                                         &hRes);
        REQUIRE(rc == AE_SUCCESS);
        CHECK(hRes != 0);

        UNSIGNED32 nrows = 0;
        REQUIRE(AdsFetchWhereRows(hRes, &nrows) == AE_SUCCESS);

        if (nrows > 0) {
            UNSIGNED32 rec = 0;
            REQUIRE(AdsFetchWhereRecno(hRes, 0, &rec) == AE_SUCCESS);
            all_recnos.push_back(rec);

            UNSIGNED8  nm_fld[] = "NM";
            UNSIGNED8  buf[32]{};
            UNSIGNED16 blen = sizeof(buf);
            REQUIRE(AdsFetchWhereField(hRes, 0, nm_fld, buf, &blen) == AE_SUCCESS);
            all_nms.push_back(trim_right(
                std::string(reinterpret_cast<char*>(buf), blen)));
        }

        UNSIGNED16 eof_flag = 0;
        REQUIRE(AdsFetchWhereEof(hRes, &eof_flag) == AE_SUCCESS);
        REQUIRE(AdsFetchWhereClose(hRes) == AE_SUCCESS);

        if (eof_flag || nrows == 0) break;
    }

    // Exactly 2 matches: recno 2 ("B") and recno 3 ("C"), in order.
    REQUIRE(all_recnos.size() == 2u);
    CHECK(all_recnos[0] == 2u);
    CHECK(all_recnos[1] == 3u);

    REQUIRE(all_nms.size() == 2u);
    CHECK(all_nms[0] == "B");
    CHECK(all_nms[1] == "C");

    // No duplicates (each recno appears exactly once).
    std::vector<std::uint32_t> sorted = all_recnos;
    std::sort(sorted.begin(), sorted.end());
    auto last = std::unique(sorted.begin(), sorted.end());
    CHECK(last == sorted.end());  // no duplicates

    fix.close();
}

// ── 7. V2: ApplyRow loads a batch row into the table row cache (no goto) ──────
// Contract: after AdsFetchWhereApplyRow(hRes, row, hTbl), AdsGetRecordNum /
// AdsGetField / AdsAtEOF serve that row from the client-side cache without an
// AdsGotoRecord round-trip. This is the rddads V2 forward-scan path: walk the
// matched rows entirely from the batch.
TEST_CASE("AdsFetchWhereApplyRow serves recno + fields from cache without a goto") {
    fw_wipe();
    auto dir = fw_tmp_dir();
    seed_nm_fixture(dir);

    RemoteFixture fix;
    fix.open(dir, "fw.dbf");
    REQUIRE(AdsGotoTop(fix.hTable) == AE_SUCCESS);

    // Fetch NM >= 'B' with the column + per-row recnos (rddads requests all
    // fields in field order; here the table has the single field NM).
    UNSIGNED8 expr[] = "NM >= 'B'";
    UNSIGNED8 cols[] = "NM";
    ADSHANDLE hRes = 0;
    REQUIRE(AdsFetchWhere(fix.hTable, expr, cols, 100, 0x01u, &hRes) == AE_SUCCESS);
    UNSIGNED32 nrows = 0;
    REQUIRE(AdsFetchWhereRows(hRes, &nrows) == AE_SUCCESS);
    REQUIRE(nrows == 2u);

    // Park the *server* cursor on a NON-matching record (recno 1, "A"). If
    // ApplyRow leaked the server position instead of serving from cache, the
    // assertions below would see recno 1 / "A".
    REQUIRE(AdsGotoRecord(fix.hTable, 1) == AE_SUCCESS);

    UNSIGNED8  nm_fld[] = "NM";
    UNSIGNED8  buf[32]{};
    UNSIGNED32 blen = 0;
    UNSIGNED32 rec  = 0;
    UNSIGNED16 eoff = 1;

    // Row 0 → "B", recno 2.
    REQUIRE(AdsFetchWhereApplyRow(hRes, 0, fix.hTable) == AE_SUCCESS);
    REQUIRE(AdsGetRecordNum(fix.hTable, 0, &rec) == AE_SUCCESS);
    CHECK(rec == 2u);
    blen = sizeof(buf); std::memset(buf, 0, sizeof(buf));
    REQUIRE(AdsGetField(fix.hTable, nm_fld, buf, &blen, 0) == AE_SUCCESS);
    CHECK(trim_right(std::string(reinterpret_cast<char*>(buf), blen)) == "B");
    REQUIRE(AdsAtEOF(fix.hTable, &eoff) == AE_SUCCESS);
    CHECK(eoff == 0);

    // Row 1 → "C", recno 3.
    REQUIRE(AdsFetchWhereApplyRow(hRes, 1, fix.hTable) == AE_SUCCESS);
    REQUIRE(AdsGetRecordNum(fix.hTable, 0, &rec) == AE_SUCCESS);
    CHECK(rec == 3u);
    blen = sizeof(buf); std::memset(buf, 0, sizeof(buf));
    REQUIRE(AdsGetField(fix.hTable, nm_fld, buf, &blen, 0) == AE_SUCCESS);
    CHECK(trim_right(std::string(reinterpret_cast<char*>(buf), blen)) == "C");

    // Out-of-range row and invalid result handle must fail cleanly.
    CHECK(AdsFetchWhereApplyRow(hRes, 99, fix.hTable) != AE_SUCCESS);
    CHECK(AdsFetchWhereApplyRow(0,    0,  fix.hTable) != AE_SUCCESS);

    REQUIRE(AdsFetchWhereClose(hRes) == AE_SUCCESS);
    fix.close();
}

