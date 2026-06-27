// tests/unit/abi_plus_postgres_filter_test.cpp
// Tier-2 SQL push-down on a PostgreSQL-backed table: AdsSetAOF / AdsSetFilter
// translate the xBase predicate to a SQL WHERE and push it to the backend, so
// navigation walks only matching rows. Mirrors the SQLite filter test. Needs a
// live PostgreSQL (OPENADS_TEST_PG_URI, default 127.0.0.1:5433) — the case is a
// no-op when the backend is compiled out.
#include "doctest.h"
#include "openads/ace.h"
#include "openads/error.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <set>
#include <string>
#include <vector>

#if defined(OPENADS_WITH_POSTGRESQL)
#include <libpq-fe.h>

namespace {

const char* test_pg_uri() {
    const char* e = std::getenv("OPENADS_TEST_PG_URI");
    return (e && e[0]) ? e : "postgresql://postgres@127.0.0.1:5433/postgres";
}

void seed_items(PGconn* conn) {
    auto exec = [&](const char* sql) {
        PGresult* r = PQexec(conn, sql);
        const auto st = PQresultStatus(r);
        if (st != PGRES_COMMAND_OK && st != PGRES_TUPLES_OK) {
            std::string msg = std::string("seed: ") + PQerrorMessage(conn);
            PQclear(r);
            FAIL(msg);
        }
        PQclear(r);
    };
    exec("DROP TABLE IF EXISTS items");
    exec("CREATE TABLE items (id INTEGER PRIMARY KEY, nm TEXT, qty INTEGER)");
    exec("INSERT INTO items (id, nm, qty) VALUES "
         "(1,'a',50), (2,'b',100), (3,'c',150), (4,'d',200), (5,'e',10)");
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

UNSIGNED32 g_count(ADSHANDLE hTable) {
    UNSIGNED32 c = 0;
    REQUIRE(AdsGetRecordCount(hTable, 0, &c) == 0);
    return c;
}

} // namespace

TEST_CASE("Tier-2 push-down: SET FILTER/AOF on a PostgreSQL table filters server-side") {
    const char* uri_cstr = test_pg_uri();
    PGconn* seed = PQconnectdb(uri_cstr);
    REQUIRE(PQstatus(seed) == CONNECTION_OK);
    seed_items(seed);
    PQfinish(seed);

    const std::string uri = uri_cstr;
    std::vector<UNSIGNED8> srv(uri.size() + 1);
    std::memcpy(srv.data(), uri.c_str(), uri.size() + 1);

    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv.data(), ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);
    UNSIGNED8 tbl_name[32] = "items";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsOpenTable(hConn, tbl_name, tbl_name, ADS_DEFAULT, 0, 0, 0,
                         ADS_READONLY, &hTable) == 0);

    CHECK(g_count(hTable) == 5u);

    UNSIGNED8 cond[] = "QTY >= 100";
    REQUIRE(AdsSetAOF(hTable, cond, 0) == 0);            // translated + pushed
    CHECK(g_count(hTable) == 3u);
    CHECK(scan_qty(hTable) == std::set<std::string>{"100", "150", "200"});

    REQUIRE(AdsClearAOF(hTable) == 0);
    CHECK(g_count(hTable) == 5u);

    UNSIGNED8 scond[] = "UPPER(NM) = 'B'";
    REQUIRE(AdsSetAOF(hTable, scond, 0) == 0);
    CHECK(g_count(hTable) == 1u);
    CHECK(scan_qty(hTable) == std::set<std::string>{"100"});
    REQUIRE(AdsClearAOF(hTable) == 0);

    UNSIGNED8 filt[] = "QTY < 60";
    REQUIRE(AdsSetFilter(hTable, filt) == 0);
    CHECK(g_count(hTable) == 2u);
    REQUIRE(AdsClearAOF(hTable) == 0);

    // Untranslatable -> failure, table left unfiltered (never half-applied).
    UNSIGNED8 bad[] = "RECNO() > 1";
    CHECK(AdsSetAOF(hTable, bad, 0) != 0);
    CHECK(g_count(hTable) == 5u);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
}

#else

TEST_CASE("Tier-2 push-down: postgresql backend disabled at compile time") {
    CHECK(true);
}

#endif
