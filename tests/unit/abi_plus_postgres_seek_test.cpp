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
#endif

#if defined(OPENADS_WITH_POSTGRESQL)

namespace {

constexpr const char* kDefaultPgUri =
    "postgresql://postgres@127.0.0.1:5433/postgres";

const char* test_pg_uri() {
    const char* uri_env = std::getenv("OPENADS_TEST_PG_URI");
    if (uri_env != nullptr && uri_env[0] != '\0') {
        return uri_env;
    }
    return kDefaultPgUri;
}

void seed_fixture(PGconn* conn) {
    auto exec = [&](const char* sql) {
        PGresult* res = PQexec(conn, sql);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            PQclear(res);
            FAIL("seed failed");
        }
        PQclear(res);
    };
    exec("DROP TABLE IF EXISTS clientes");
    exec("CREATE TABLE clientes (id INTEGER PRIMARY KEY, nome TEXT)");
    exec("INSERT INTO clientes (id, nome) VALUES (1,'Ana'),(2,'Bob'),(3,'Cid')");
}

std::string field_str(ADSHANDLE hTable, const char* name) {
    UNSIGNED8 fld[32];
    std::memcpy(fld, name, std::strlen(name) + 1);
    UNSIGNED8 buf[128] = {0};
    UNSIGNED32 cap = sizeof(buf);
    REQUIRE(AdsGetField(hTable, fld, buf, &cap, 0) == 0);
    return std::string(reinterpret_cast<const char*>(buf), cap);
}

} // namespace

TEST_CASE("ABI: postgresql AdsSeek on column index") {
    const char* uri_cstr = test_pg_uri();

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

    UNSIGNED8 tbl[32] = "clientes";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsOpenTable(hConn, tbl, tbl, ADS_DEFAULT, 0, 0, 0,
                         ADS_READONLY, &hTable) == 0);

    UNSIGNED8 tag[16] = "id";
    ADSHANDLE hIndex = 0;
    UNSIGNED16 nIdx = 1;
    REQUIRE(AdsOpenIndex(hTable, tag, &hIndex, &nIdx) == 0);
    CHECK(nIdx == 1);

    const char key[] = "2";
    UNSIGNED16 found = 0;
    REQUIRE(AdsSeek(hIndex,
                    reinterpret_cast<UNSIGNED8*>(const_cast<char*>(key)),
                    static_cast<UNSIGNED16>(std::strlen(key)),
                    ADS_STRINGKEY, 0, &found) == 0);
    CHECK(found == 1);

    UNSIGNED16 is_found = 0;
    REQUIRE(AdsIsFound(hTable, &is_found) == 0);
    CHECK(is_found == 1);

    CHECK(field_str(hTable, "nome").find("Bob") != std::string::npos);

    const char miss[] = "9";
    REQUIRE(AdsSeek(hIndex,
                    reinterpret_cast<UNSIGNED8*>(const_cast<char*>(miss)),
                    static_cast<UNSIGNED16>(std::strlen(miss)),
                    ADS_STRINGKEY, 0, &found) == 0);
    CHECK(found == 0);

    REQUIRE(AdsCloseIndex(hIndex) == 0);
    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
}

#endif