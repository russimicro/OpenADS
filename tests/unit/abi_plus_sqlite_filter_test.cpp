// tests/unit/abi_plus_sqlite_filter_test.cpp
// Tier-2 SQL push-down (fase 2): AdsSetAOF / AdsSetFilter on a SQLite-backed
// table translate the xBase predicate to a SQL WHERE and push it to the
// backend, so navigation (GotoTop / Skip / GetRecordCount) walks only matching
// rows. A predicate that can't be translated returns a failure and leaves the
// table unfiltered (the caller then filters client-side) — never silently
// "succeeds" with the wrong rows.
#include "doctest.h"
#include "openads/ace.h"
#include "openads/error.h"

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

namespace fs = std::filesystem;

#if defined(OPENADS_WITH_SQLITE)
#include <sqlite3.h>

namespace {

void seed_items(const fs::path& db_path) {
    sqlite3* db = nullptr;
    REQUIRE(sqlite3_open(db_path.string().c_str(), &db) == SQLITE_OK);
    auto exec = [&](const char* sql) {
        char* err = nullptr;
        if (sqlite3_exec(db, sql, nullptr, nullptr, &err) != SQLITE_OK) {
            const std::string msg = err ? err : "exec failed";
            if (err) sqlite3_free(err);
            FAIL(msg);
        }
    };
    exec("CREATE TABLE items (id INTEGER PRIMARY KEY, nm TEXT, qty INTEGER)");
    exec("INSERT INTO items (id, nm, qty) VALUES "
         "(1,'a',50), (2,'b',100), (3,'c',150), (4,'d',200), (5,'e',10)");
    sqlite3_close(db);
}

std::string trimmed(ADSHANDLE hTable, const char* name) {
    UNSIGNED8 fld[32];
    std::memcpy(fld, name, std::strlen(name) + 1);
    UNSIGNED8 buf[128] = {0};
    UNSIGNED32 cap = sizeof(buf);
    REQUIRE(AdsGetField(hTable, fld, buf, &cap, 0) == 0);
    std::string s(reinterpret_cast<const char*>(buf), cap);
    while (!s.empty() && s.back()  == ' ') s.pop_back();
    std::size_t b = s.find_first_not_of(' ');
    return b == std::string::npos ? "" : s.substr(b);
}

// Walk the (possibly filtered) table top-to-bottom, collecting `qty` values.
std::set<std::string> scan_qty(ADSHANDLE hTable) {
    std::set<std::string> out;
    REQUIRE(AdsGotoTop(hTable) == 0);
    UNSIGNED16 eof = 0;
    REQUIRE(AdsAtEOF(hTable, &eof) == 0);
    while (eof == 0) {
        out.insert(trimmed(hTable, "qty"));
        REQUIRE(AdsSkip(hTable, 1) == 0);
        REQUIRE(AdsAtEOF(hTable, &eof) == 0);
    }
    return out;
}

ADSHANDLE g_count(ADSHANDLE hTable) {
    UNSIGNED32 c = 0;
    REQUIRE(AdsGetRecordCount(hTable, 0, &c) == 0);
    return c;
}

} // namespace

TEST_CASE("Tier-2 push-down: SET FILTER/AOF on a SQLite table filters server-side") {
    const auto dir = fs::temp_directory_path() / "openads_plus_sqlite_filter";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    const auto db_path = dir / "items.db";
    seed_items(db_path);

    const std::string uri = "sqlite://" + db_path.string();
    std::vector<UNSIGNED8> srv(uri.size() + 1);
    std::memcpy(srv.data(), uri.c_str(), uri.size() + 1);

    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv.data(), ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);
    UNSIGNED8 tbl_name[32] = "items";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsOpenTable(hConn, tbl_name, tbl_name, ADS_DEFAULT, 0, 0, 0,
                         ADS_READONLY, &hTable) == 0);

    // Unfiltered: all 5 rows.
    CHECK(g_count(hTable) == 5u);

    // Push a numeric predicate: QTY >= 100 -> rows 2,3,4 (qty 100,150,200).
    UNSIGNED8 cond[] = "QTY >= 100";
    REQUIRE(AdsSetAOF(hTable, cond, 0) == 0);   // translated + pushed
    CHECK(g_count(hTable) == 3u);
    CHECK(scan_qty(hTable) == std::set<std::string>{"100", "150", "200"});

    // Clearing restores every row.
    REQUIRE(AdsClearAOF(hTable) == 0);
    CHECK(g_count(hTable) == 5u);

    // A string function predicate also pushes down: UPPER(NM) = 'B' -> row 2.
    UNSIGNED8 scond[] = "UPPER(NM) = 'B'";
    REQUIRE(AdsSetAOF(hTable, scond, 0) == 0);
    CHECK(g_count(hTable) == 1u);
    CHECK(scan_qty(hTable) == std::set<std::string>{"100"});
    REQUIRE(AdsClearAOF(hTable) == 0);
    CHECK(g_count(hTable) == 5u);

    // AdsSetFilter pushes too (same mechanism).
    UNSIGNED8 filt[] = "QTY < 60";   // rows 1,5 (50,10)
    REQUIRE(AdsSetFilter(hTable, filt) == 0);
    CHECK(g_count(hTable) == 2u);
    REQUIRE(AdsClearAOF(hTable) == 0);

    // Untranslatable predicate (RECNO()) -> failure, and the table is left
    // UNFILTERED (not silently filtered with a half-applied predicate).
    UNSIGNED8 bad[] = "RECNO() > 1";
    CHECK(AdsSetAOF(hTable, bad, 0) != 0);
    CHECK(g_count(hTable) == 5u);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

#else

TEST_CASE("Tier-2 push-down: sqlite backend disabled at compile time") {
    CHECK(true);
}

#endif
