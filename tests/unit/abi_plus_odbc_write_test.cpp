#include "doctest.h"
#include "openads/ace.h"
#include "openads/error.h"

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#if defined(OPENADS_WITH_ODBC)

namespace {

// Live ODBC fixture: a connection string pointing at a data source that
// already holds a `clientes` table seeded with
//   (1,'Ana',10.5), (2,'Bob',NULL), (3,'Cid',0.0)
// id INTEGER PRIMARY KEY, nome VARCHAR(64), saldo (FLOAT/DOUBLE).
// tools/scripts/run_odbc_tests_live.ps1 seeds it and exports the variable.
// When unset the live test is skipped. The test is self-restoring: it
// appends a row and then deletes it, leaving the table back at 3 rows so
// the read/seek cases (which expect 3) pass regardless of order.
const char* test_odbc_connstr() {
    const char* v = std::getenv("OPENADS_TEST_ODBC_CONNSTR");
    return (v != nullptr && v[0] != '\0') ? v : nullptr;
}

ADSHANDLE connect_odbc(const char* connstr) {
    const std::string uri = std::string("odbc://") + connstr;
    std::vector<UNSIGNED8> srv(uri.size() + 1);
    std::memcpy(srv.data(), uri.c_str(), uri.size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv.data(), ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);
    return hConn;
}

ADSHANDLE open_clientes(ADSHANDLE hConn) {
    UNSIGNED8 tbl[32] = "clientes";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsOpenTable(hConn, tbl, tbl, ADS_DEFAULT, 0, 0, 0,
                         ADS_DEFAULT, &hTable) == 0);
    return hTable;
}

void set_str(ADSHANDLE hTable, const char* field, const char* value) {
    UNSIGNED8 f[64];
    std::memcpy(f, field, std::strlen(field) + 1);
    UNSIGNED8 v[256];
    std::memcpy(v, value, std::strlen(value) + 1);
    REQUIRE(AdsSetString(hTable, f, v,
                         static_cast<UNSIGNED32>(std::strlen(value))) == 0);
}

void set_dbl(ADSHANDLE hTable, const char* field, double value) {
    UNSIGNED8 f[64];
    std::memcpy(f, field, std::strlen(field) + 1);
    REQUIRE(AdsSetDouble(hTable, f, value) == 0);
}

std::string read_str(ADSHANDLE hTable, const char* field) {
    UNSIGNED8 f[64];
    std::memcpy(f, field, std::strlen(field) + 1);
    UNSIGNED8  buf[256] = {0};
    UNSIGNED32 cap = sizeof(buf);
    REQUIRE(AdsGetField(hTable, f, buf, &cap, 0) == 0);
    return std::string(reinterpret_cast<const char*>(buf), cap);
}

UNSIGNED32 count_rows(ADSHANDLE hTable) {
    UNSIGNED32 n = 0;
    REQUIRE(AdsGetRecordCount(hTable, 0, &n) == 0);
    return n;
}

} // namespace

TEST_CASE("ABI: odbc navigational write append/update/delete") {
    const char* connstr = test_odbc_connstr();
    if (connstr == nullptr) {
        MESSAGE("OPENADS_TEST_ODBC_CONNSTR not set; skipping live ODBC write");
        return;
    }

    ADSHANDLE hConn  = connect_odbc(connstr);
    ADSHANDLE hTable = open_clientes(hConn);
    REQUIRE(count_rows(hTable) == 3);

    // --- APPEND a new row (id=4, 'Dan', 99.9). WriteRecord leaves the
    //     cursor positioned on the freshly inserted row. ---
    REQUIRE(AdsAppendRecord(hTable) == 0);
    set_str(hTable, "id", "4");
    set_str(hTable, "nome", "Dan");
    set_dbl(hTable, "saldo", 99.9);
    REQUIRE(AdsWriteRecord(hTable) == 0);
    CHECK(count_rows(hTable) == 4);
    CHECK(read_str(hTable, "nome").find("Dan") != std::string::npos);

    // --- UPDATE the saldo of the positioned row ---
    set_dbl(hTable, "saldo", 42.0);
    REQUIRE(AdsWriteRecord(hTable) == 0);
    CHECK(read_str(hTable, "saldo").find("42") != std::string::npos);
    CHECK(count_rows(hTable) == 4);   // update must not add a row

    // --- DELETE it, restoring the table to 3 rows ---
    REQUIRE(AdsDeleteRecord(hTable) == 0);
    CHECK(count_rows(hTable) == 3);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
}

#endif // OPENADS_WITH_ODBC
