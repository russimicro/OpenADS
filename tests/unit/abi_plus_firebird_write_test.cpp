#include "doctest.h"
#include "openads/ace.h"
#include "openads/error.h"

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#if defined(OPENADS_WITH_FIREBIRD)

namespace {

// Live embedded-Firebird fixture (same `.fdb` the read test uses):
// OPENADS_TEST_FIREBIRD_DB points at a base seeded with `clientes`
//   (1,'Ana',10.5), (2,'Bob',NULL), (3,'Cid',0.0)
// ID INTEGER PRIMARY KEY, NOME VARCHAR(64), SALDO DOUBLE PRECISION.
// When unset the case is skipped. This drives the WRITE side of the ACE ABI
// (AdsAppendRecord / AdsSetString / AdsWriteRecord / AdsDeleteRecord) wired
// onto the Firebird backend, and is self-restoring: it appends one row and
// deletes it again, leaving the table back at 3 rows so the read/seek cases
// that expect 3 keep passing regardless of test order.
const char* test_firebird_db() {
    const char* v = std::getenv("OPENADS_TEST_FIREBIRD_DB");
    return (v != nullptr && v[0] != '\0') ? v : nullptr;
}

ADSHANDLE connect_firebird(const char* db) {
    const std::string uri = std::string("firebird:///") + db;
    std::vector<UNSIGNED8> srv(uri.size() + 1);
    std::memcpy(srv.data(), uri.c_str(), uri.size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv.data(), ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);
    return hConn;
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

TEST_CASE("ABI: firebird AdsAppendRecord + AdsSetString + AdsWriteRecord + AdsDeleteRecord") {
    const char* db = test_firebird_db();
    if (db == nullptr) {
        MESSAGE("OPENADS_TEST_FIREBIRD_DB not set; skipping live Firebird write test");
        return;
    }

    ADSHANDLE hConn = connect_firebird(db);

    UNSIGNED8 tbl_name[32] = "clientes";
    ADSHANDLE hTable = 0;
    // ADS_DEFAULT (not ADS_READONLY) => read/write open.
    REQUIRE(AdsOpenTable(hConn, tbl_name, tbl_name,
                         ADS_DEFAULT, 0, 0, 0, ADS_DEFAULT, &hTable) == 0);

    CHECK(row_count(hTable) == 3);

    // INSERT a staged row: dbAppend() + REPLACE field, value -> AdsWriteRecord.
    REQUIRE(AdsAppendRecord(hTable) == 0);
    set_str(hTable, "id", "99");
    set_str(hTable, "nome", "Dan");
    set_str(hTable, "saldo", "42.5");
    REQUIRE(AdsWriteRecord(hTable) == 0);

    CHECK(row_count(hTable) == 4);

    // Locate the appended row and verify it reads back.
    REQUIRE(AdsGotoBottom(hTable) == 0);
    UNSIGNED8 fld[64];
    std::memcpy(fld, "nome", 5);
    UNSIGNED8  buf[256] = {0};
    UNSIGNED32 cap = sizeof(buf);
    REQUIRE(AdsGetField(hTable, fld, buf, &cap, 0) == 0);
    CHECK(std::string(reinterpret_cast<const char*>(buf), cap)
              == std::string(64, ' ').replace(0, 3, "Dan"));

    // Clean up: delete the row we added, restoring the fixture to 3 rows.
    REQUIRE(AdsDeleteRecord(hTable) == 0);
    CHECK(row_count(hTable) == 3);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
}

TEST_CASE("ABI: firebird AdsLockRecord is cross-connection (lock table)") {
    const char* db = test_firebird_db();
    if (db == nullptr) {
        MESSAGE("OPENADS_TEST_FIREBIRD_DB not set; skipping live Firebird lock test");
        return;
    }

    auto open_clientes = [&](ADSHANDLE& hConn) -> ADSHANDLE {
        hConn = connect_firebird(db);
        UNSIGNED8 tbl_name[32] = "clientes";
        ADSHANDLE hTable = 0;
        REQUIRE(AdsOpenTable(hConn, tbl_name, tbl_name,
                             ADS_DEFAULT, 0, 0, 0, ADS_DEFAULT, &hTable) == 0);
        return hTable;
    };

    ADSHANDLE hConnA = 0, hConnB = 0;
    ADSHANDLE hA = open_clientes(hConnA);   // two distinct embedded attachments
    ADSHANDLE hB = open_clientes(hConnB);

    CHECK(AdsLockRecord(hA, 1) == 0);
    CHECK(AdsLockRecord(hB, 1) != 0);       // refused: A holds it
    CHECK(AdsUnlockRecord(hA, 1) == 0);
    CHECK(AdsLockRecord(hB, 1) == 0);       // now free
    CHECK(AdsUnlockRecord(hB, 1) == 0);

    REQUIRE(AdsCloseTable(hA) == 0);
    REQUIRE(AdsCloseTable(hB) == 0);
    REQUIRE(AdsDisconnect(hConnA) == 0);
    REQUIRE(AdsDisconnect(hConnB) == 0);
}

#endif
