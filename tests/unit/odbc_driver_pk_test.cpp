// Tests the OpenADS ODBC driver's SQLPrimaryKeys against a dictionary-managed
// table whose primary key is defined in the Data Dictionary. The fixture is
// built through the ACE ABI (create a CDX tag, mark it as the table's PK, and
// persist the dictionary), then the driver's catalog call is driven directly.
#include "doctest.h"

#ifdef _WIN32
#  include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>

#include "openads/ace.h"
#include "test_dd_make.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

namespace {

// Minimal DBF with one C(10) field "NAME" and zero records.
void make_dbf_name(const fs::path& p) {
    std::vector<std::uint8_t> file;
    std::array<std::uint8_t, 32> hdr{};
    hdr[0] = 0x03;
    const std::uint16_t hl = 32 + 32 + 1, rl = 1 + 10;
    hdr[8]  = static_cast<std::uint8_t>( hl       & 0xFFu);
    hdr[9]  = static_cast<std::uint8_t>((hl >> 8) & 0xFFu);
    hdr[10] = static_cast<std::uint8_t>( rl       & 0xFFu);
    hdr[11] = static_cast<std::uint8_t>((rl >> 8) & 0xFFu);
    file.insert(file.end(), hdr.begin(), hdr.end());
    std::array<std::uint8_t, 32> fd{};
    std::memcpy(fd.data(), "NAME", 4);
    fd[11] = 'C'; fd[16] = 10;
    file.insert(file.end(), fd.begin(), fd.end());
    file.push_back(0x0D);
    file.push_back(0x1A);
    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(file.data()),
        static_cast<std::streamsize>(file.size()));
}

SQLCHAR* sc(const char* s) {
    return reinterpret_cast<SQLCHAR*>(const_cast<char*>(s));
}

} // namespace

TEST_CASE("openads ODBC driver: SQLPrimaryKeys reports the DD primary key") {
    auto dir = fs::temp_directory_path() / "openads_odbc_pk";
    std::error_code ec; fs::remove_all(dir, ec);
    fs::create_directories(dir);

    make_dbf_name(dir / "emp.dbf");
    const auto addp = dir / "test.add";
    openads_test::make_dd(addp, "TABLE emp=emp.dbf\n");

    // --- Build the PK fixture through the ACE ABI and persist it. ---
    {
        UNSIGNED8 ap[512];
        auto aps = addp.string();
        std::memcpy(ap, aps.c_str(), aps.size() + 1);
        ADSHANDLE hConn = 0;
        REQUIRE(AdsConnect60(ap, 1, nullptr, nullptr, 0, &hConn) == 0);

        UNSIGNED8 nm[8] = "emp";
        ADSHANDLE hTable = 0;
        REQUIRE(AdsOpenTable(hConn, nm, nm, ADS_CDX, 1, 1, 0, 1, &hTable) == 0);

        UNSIGNED8 bag[8]  = "emp";
        UNSIGNED8 tag[8]  = "PKEMP";
        UNSIGNED8 expr[8] = "NAME";
        ADSHANDLE hIdx = 0;
        REQUIRE(AdsCreateIndex61(hTable, bag, tag, expr,
                                 nullptr, nullptr, 0, 512, &hIdx) == 0);
        REQUIRE(AdsCloseIndex(hIdx) == 0);
        REQUIRE(AdsCloseTable(hTable) == 0);

        // Set PK first, then register the bag — AdsDDAddIndexFile persists the
        // whole dictionary (including the primary-key property) to the .add.
        REQUIRE(AdsDDSetTableProperty(hConn, nm, /*PRIMARY_KEY*/ 202,
                                      const_cast<char*>("PKEMP"), 5) == 0);
        UNSIGNED8 idxf[16] = "emp.cdx";
        UNSIGNED8 cmt[1]   = {0};
        REQUIRE(AdsDDAddIndexFile(hConn, nm, idxf, cmt) == 0);
        REQUIRE(AdsDisconnect(hConn) == 0);
    }

    // --- Drive the ODBC driver against the .add (dictionary loaded). ---
    SQLHENV env = SQL_NULL_HENV;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env) == SQL_SUCCESS);
    REQUIRE(SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION,
                          reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0)
            == SQL_SUCCESS);
    SQLHDBC dbc = SQL_NULL_HDBC;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc) == SQL_SUCCESS);

    std::string cs = "DRIVER={OpenADS};DataDir=" + addp.string()
                   + ";ServerType=local";
    REQUIRE(SQLDriverConnect(dbc, nullptr, sc(cs.c_str()), SQL_NTS,
                             nullptr, 0, nullptr, SQL_DRIVER_NOPROMPT)
            == SQL_SUCCESS);

    SQLHSTMT st = SQL_NULL_HSTMT;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &st) == SQL_SUCCESS);
    REQUIRE(SQLPrimaryKeys(st, nullptr, 0, nullptr, 0, sc("emp"), SQL_NTS)
            == SQL_SUCCESS);

    int rows = 0;
    std::string colname, keyseq, pkname;
    while (SQLFetch(st) == SQL_SUCCESS) {
        SQLCHAR cn[128] = {0}, ks[64] = {0}, pk[128] = {0};
        SQLLEN ind = 0;
        REQUIRE(SQLGetData(st, 4, SQL_C_CHAR, cn, sizeof(cn), &ind) == SQL_SUCCESS);
        REQUIRE(SQLGetData(st, 5, SQL_C_CHAR, ks, sizeof(ks), &ind) == SQL_SUCCESS);
        REQUIRE(SQLGetData(st, 6, SQL_C_CHAR, pk, sizeof(pk), &ind) == SQL_SUCCESS);
        colname = reinterpret_cast<char*>(cn);
        keyseq  = reinterpret_cast<char*>(ks);
        pkname  = reinterpret_cast<char*>(pk);
        ++rows;
    }
    CHECK(rows == 1);
    CHECK(colname == "NAME");
    CHECK(keyseq == "1");
    CHECK(pkname == "PKEMP");

    SQLFreeHandle(SQL_HANDLE_STMT, st);
    REQUIRE(SQLDisconnect(dbc) == SQL_SUCCESS);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
    fs::remove_all(dir, ec);
}
