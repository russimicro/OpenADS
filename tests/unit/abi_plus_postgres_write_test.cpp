#include "doctest.h"
#include "openads/ace.h"
#include "openads/error.h"

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>

#if defined(OPENADS_WITH_POSTGRESQL)
#include <libpq-fe.h>

namespace {

constexpr const char* kDefaultPgUri =
    "postgresql://postgres@127.0.0.1:5433/postgres";

const char* test_pg_uri() {
    const char* uri_env = std::getenv("OPENADS_TEST_PG_URI");
    if (uri_env != nullptr && uri_env[0] != '\0') return uri_env;
    return kDefaultPgUri;
}

bool pg_reachable(const char* uri) {
    PGconn* c = PQconnectdb(uri);
    const bool ok = (PQstatus(c) == CONNECTION_OK);
    PQfinish(c);
    return ok;
}

void seed_fixture(PGconn* conn) {
    auto exec = [&](const char* sql) {
        PGresult* res = PQexec(conn, sql);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            const char* msg = PQerrorMessage(conn);
            std::string detail = "seed failed";
            if (msg != nullptr && msg[0] != '\0') { detail += ": "; detail += msg; }
            PQclear(res);
            FAIL(detail);
        }
        PQclear(res);
    };
    exec("DROP TABLE IF EXISTS clientes");
    exec("CREATE TABLE clientes ("
         "id INTEGER PRIMARY KEY, nome TEXT, saldo DOUBLE PRECISION)");
    exec("INSERT INTO clientes (id, nome, saldo) VALUES "
         "(1, 'Ana', 10.5), (2, 'Bob', NULL), (3, 'Cid', 0.0)");
}

std::string rtrim(std::string s) {
    while (!s.empty() && s.back() == ' ') s.pop_back();
    return s;
}

std::string field_str(ADSHANDLE hTable, const char* name) {
    UNSIGNED8 fld[32];
    std::memcpy(fld, name, std::strlen(name) + 1);
    UNSIGNED8 buf[256] = {0};
    UNSIGNED32 cap = sizeof(buf);
    REQUIRE(AdsGetField(hTable, fld, buf, &cap, 0) == 0);
    return std::string(reinterpret_cast<const char*>(buf), cap);
}

void set_str(ADSHANDLE hTable, const char* field, const char* value) {
    UNSIGNED8 f[64];
    std::memcpy(f, field, std::strlen(field) + 1);
    UNSIGNED8 v[256];
    std::memcpy(v, value, std::strlen(value) + 1);
    REQUIRE(AdsSetString(hTable, f, v,
                         static_cast<UNSIGNED32>(std::strlen(value))) == 0);
}

UNSIGNED32 row_count(ADSHANDLE hTable) {
    UNSIGNED32 count = 0;
    REQUIRE(AdsGetRecordCount(hTable, 0, &count) == 0);
    return count;
}

} // namespace

TEST_CASE("ABI: postgresql AdsAppendRecord + AdsSetString + AdsWriteRecord + AdsDeleteRecord") {
    const char* uri_cstr = test_pg_uri();
    if (!pg_reachable(uri_cstr)) {
        MESSAGE("PostgreSQL not reachable; skipping live write test");
        return;
    }

    PGconn* seed = PQconnectdb(uri_cstr);
    REQUIRE(PQstatus(seed) == CONNECTION_OK);
    seed_fixture(seed);
    PQfinish(seed);

    const std::string uri = uri_cstr;
    std::vector<UNSIGNED8> srv(uri.size() + 1);
    std::memcpy(srv.data(), uri.c_str(), uri.size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv.data(), ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 tbl_name[32] = "clientes";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsOpenTable(hConn, tbl_name, tbl_name,
                         ADS_DEFAULT, 0, 0, 0, ADS_DEFAULT, &hTable) == 0);

    CHECK(row_count(hTable) == 3);

    // INSERT via dbAppend + REPLACE + AdsWriteRecord.
    REQUIRE(AdsAppendRecord(hTable) == 0);
    set_str(hTable, "id", "99");
    set_str(hTable, "nome", "Dan");
    set_str(hTable, "saldo", "42.5");
    REQUIRE(AdsWriteRecord(hTable) == 0);
    CHECK(row_count(hTable) == 4);

    // The appended row reads back (id=99 sorts last by PK).
    REQUIRE(AdsGotoBottom(hTable) == 0);
    CHECK(rtrim(field_str(hTable, "nome")) == "Dan");

    // UPDATE the positioned row (REPLACE then WriteRecord).
    set_str(hTable, "nome", "DanX");
    REQUIRE(AdsWriteRecord(hTable) == 0);
    REQUIRE(AdsGotoBottom(hTable) == 0);
    CHECK(rtrim(field_str(hTable, "nome")) == "DanX");

    // DELETE the row, restoring the fixture to 3 rows.
    REQUIRE(AdsDeleteRecord(hTable) == 0);
    CHECK(row_count(hTable) == 3);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
}

TEST_CASE("ABI: postgresql AdsLockRecord is cross-connection (advisory lock)") {
    const char* uri_cstr = test_pg_uri();
    if (!pg_reachable(uri_cstr)) {
        MESSAGE("PostgreSQL not reachable; skipping live lock test");
        return;
    }
    PGconn* seed = PQconnectdb(uri_cstr);
    REQUIRE(PQstatus(seed) == CONNECTION_OK);
    seed_fixture(seed);
    PQfinish(seed);

    const std::string uri = uri_cstr;
    auto open_clientes = [&](ADSHANDLE& hConn) -> ADSHANDLE {
        std::vector<UNSIGNED8> srv(uri.size() + 1);
        std::memcpy(srv.data(), uri.c_str(), uri.size() + 1);
        REQUIRE(AdsConnect60(srv.data(), ADS_LOCAL_SERVER,
                             nullptr, nullptr, 0, &hConn) == 0);
        UNSIGNED8 tbl_name[32] = "clientes";
        ADSHANDLE hTable = 0;
        REQUIRE(AdsOpenTable(hConn, tbl_name, tbl_name,
                             ADS_DEFAULT, 0, 0, 0, ADS_DEFAULT, &hTable) == 0);
        return hTable;
    };

    ADSHANDLE hConnA = 0, hConnB = 0;
    ADSHANDLE hA = open_clientes(hConnA);   // two distinct PG sessions
    ADSHANDLE hB = open_clientes(hConnB);

    // A locks record 1; B (a different connection) must be refused.
    CHECK(AdsLockRecord(hA, 1) == 0);
    CHECK(AdsLockRecord(hB, 1) != 0);

    // After A releases, B can take it.
    CHECK(AdsUnlockRecord(hA, 1) == 0);
    CHECK(AdsLockRecord(hB, 1) == 0);
    CHECK(AdsUnlockRecord(hB, 1) == 0);

    REQUIRE(AdsCloseTable(hA) == 0);
    REQUIRE(AdsCloseTable(hB) == 0);
    REQUIRE(AdsDisconnect(hConnA) == 0);
    REQUIRE(AdsDisconnect(hConnB) == 0);
}

#endif
