#include "doctest.h"
#include "openads/ace.h"
#include "openads/error.h"

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

#if defined(OPENADS_WITH_SQLITE)
#include <sqlite3.h>
#endif

namespace fs = std::filesystem;

#if defined(OPENADS_WITH_SQLITE)

namespace {

void seed_db(const fs::path& db_path) {
    sqlite3* db = nullptr;
    REQUIRE(sqlite3_open(db_path.string().c_str(), &db) == SQLITE_OK);
    auto exec = [&](const char* sql) {
        char* err = nullptr;
        const int rc = sqlite3_exec(db, sql, nullptr, nullptr, &err);
        if (rc != SQLITE_OK) {
            const std::string msg = err ? err : "sqlite exec failed";
            if (err) sqlite3_free(err);
            FAIL(msg);
        }
    };
    exec("CREATE TABLE clientes ("
         "id INTEGER PRIMARY KEY, nome TEXT, saldo REAL)");
    exec("INSERT INTO clientes (id, nome, saldo) VALUES "
         "(1, 'Ana', 10.5), (2, 'Bob', NULL), (3, 'Cid', 0.0)");
    sqlite3_close(db);
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

TEST_CASE("ABI Plus: sqlite read-only AdsOpenTable navigation") {
    const auto dir = fs::temp_directory_path() / "openads_plus_sqlite";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    const auto db_path = dir / "app.db";
    seed_db(db_path);

    const std::string uri = "sqlite://" + db_path.string();
    std::vector<UNSIGNED8> srv(uri.size() + 1);
    std::memcpy(srv.data(), uri.c_str(), uri.size() + 1);

    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv.data(), ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 tbl_name[32] = "clientes";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsOpenTable(hConn, tbl_name, tbl_name,
                         ADS_DEFAULT, 0, 0, 0, ADS_READONLY,
                         &hTable) == 0);

    UNSIGNED16 nfields = 0;
    REQUIRE(AdsGetNumFields(hTable, &nfields) == 0);
    CHECK(nfields == 3);

    UNSIGNED32 count = 0;
    REQUIRE(AdsGetRecordCount(hTable, 0, &count) == 0);
    CHECK(count == 3);

    REQUIRE(AdsGotoTop(hTable) == 0);

    UNSIGNED16 bof = 1;
    REQUIRE(AdsAtBOF(hTable, &bof) == 0);
    CHECK(bof == 0);

    CHECK(field_str(hTable, "nome") == std::string(64, ' ').replace(0, 3, "Ana"));

    REQUIRE(AdsSkip(hTable, 1) == 0);
    std::string saldo = field_str(hTable, "saldo");
    CHECK(saldo.empty());

    REQUIRE(AdsSkip(hTable, 1) == 0);
    CHECK(field_str(hTable, "nome") == std::string(64, ' ').replace(0, 3, "Cid"));

    UNSIGNED16 eof = 0;
    REQUIRE(AdsAtEOF(hTable, &eof) == 0);
    CHECK(eof == 0);

    REQUIRE(AdsSkip(hTable, 1) == 0);

    REQUIRE(AdsAtEOF(hTable, &eof) == 0);
    CHECK(eof == 1);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);

    fs::remove_all(dir, ec);
}

// Regression: rddads' adsGetValue() reads fields by ORDINAL, passing
// ADSFIELD(n) -- a 1-based field number cast to a pointer, NOT a
// NUL-terminated name. The SQL backends' get_field must resolve that
// ordinal by index (as the metadata getters already do) instead of
// running to_internal()/strlen() on the tiny pointer, which dereferences
// invalid memory and crashes. Without this, USE+FieldGet from plain
// Harbour over any SQL Plus backend access-violates on the first cell.
TEST_CASE("ABI Plus: sqlite AdsGetField by ADSFIELD ordinal (rddads idiom)") {
    const auto dir = fs::temp_directory_path() / "openads_plus_sqlite_ord";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    const auto db_path = dir / "app.db";
    seed_db(db_path);

    const std::string uri = "sqlite://" + db_path.string();
    std::vector<UNSIGNED8> srv(uri.size() + 1);
    std::memcpy(srv.data(), uri.c_str(), uri.size() + 1);

    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv.data(), ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 tbl_name[32] = "clientes";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsOpenTable(hConn, tbl_name, tbl_name,
                         ADS_DEFAULT, 0, 0, 0, ADS_READONLY,
                         &hTable) == 0);

    REQUIRE(AdsGotoTop(hTable) == 0);

    // Field 2 (nome) by ordinal -- must equal the by-name read, padded.
    UNSIGNED8 buf[128] = {0};
    UNSIGNED32 cap = sizeof(buf);
    REQUIRE(AdsGetField(hTable, ADSFIELD(2), buf, &cap, 0) == 0);
    const std::string nome(reinterpret_cast<const char*>(buf), cap);
    CHECK(nome == std::string(64, ' ').replace(0, 3, "Ana"));

    // Field 1 (id) by ordinal -- numeric value served as text.
    UNSIGNED8 ibuf[64] = {0};
    UNSIGNED32 icap = sizeof(ibuf);
    REQUIRE(AdsGetField(hTable, ADSFIELD(1), ibuf, &icap, 0) == 0);
    const std::string id(reinterpret_cast<const char*>(ibuf), icap);
    CHECK(id == "1");

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);

    fs::remove_all(dir, ec);
}

#else

TEST_CASE("ABI Plus: sqlite backend disabled at compile time") {
    UNSIGNED8 uri[] = "sqlite://:memory:";
    ADSHANDLE hConn = 0;
    const UNSIGNED32 rc = AdsConnect60(uri, ADS_LOCAL_SERVER,
                                       nullptr, nullptr, 0, &hConn);
    CHECK(rc == openads::AE_FUNCTION_NOT_AVAILABLE);
}

#endif