#include "doctest.h"
#include "openads/ace.h"
#include "test_dd_make.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

// Minimal DBF with two C fields: NAME C(10) and CODE C(4), zero records.
void make_dbf_name_code(const fs::path& p) {
    std::vector<std::uint8_t> file;
    std::array<std::uint8_t, 32> hdr{};
    hdr[0] = 0x03;
    const std::uint16_t hl = 32 + 32 * 2 + 1;
    const std::uint16_t rl = 1 + 10 + 4;
    hdr[8]  = static_cast<std::uint8_t>( hl       & 0xFFu);
    hdr[9]  = static_cast<std::uint8_t>((hl >> 8) & 0xFFu);
    hdr[10] = static_cast<std::uint8_t>( rl       & 0xFFu);
    hdr[11] = static_cast<std::uint8_t>((rl >> 8) & 0xFFu);
    file.insert(file.end(), hdr.begin(), hdr.end());

    std::array<std::uint8_t, 32> f1{};
    std::memcpy(f1.data(), "NAME", 4);
    f1[11] = 'C'; f1[16] = 10;
    file.insert(file.end(), f1.begin(), f1.end());

    std::array<std::uint8_t, 32> f2{};
    std::memcpy(f2.data(), "CODE", 4);
    f2[11] = 'C'; f2[16] = 4;
    file.insert(file.end(), f2.begin(), f2.end());

    file.push_back(0x0D);
    file.push_back(0x1A);
    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(file.data()),
        static_cast<std::streamsize>(file.size()));
}

// Create a CDX bag `bag` with tag `tag`/expression `expr` on a DD table,
// register the bag in the DD, and mark `tag` as the table's primary key.
void define_pk(ADSHANDLE hConn, const char* table, const char* bag,
               const char* tag, const char* expr) {
    UNSIGNED8 nm[64];
    std::memcpy(nm, table, std::strlen(table) + 1);

    ADSHANDLE hTable = 0;
    REQUIRE(AdsOpenTable(hConn, nm, nm, ADS_CDX, 1, 1, 0, 1, &hTable) == 0);

    UNSIGNED8 bagb[64];  std::memcpy(bagb,  bag,  std::strlen(bag)  + 1);
    UNSIGNED8 tagb[64];  std::memcpy(tagb,  tag,  std::strlen(tag)  + 1);
    UNSIGNED8 exprb[128]; std::memcpy(exprb, expr, std::strlen(expr) + 1);
    ADSHANDLE hIdx = 0;
    REQUIRE(AdsCreateIndex61(hTable, bagb, tagb, exprb,
                             nullptr, nullptr, 0, 512, &hIdx) == 0);
    REQUIRE(AdsCloseIndex(hIdx) == 0);
    REQUIRE(AdsCloseTable(hTable) == 0);

    std::string idxfile = std::string(bag) + ".cdx";
    UNSIGNED8 idxf[64]; std::memcpy(idxf, idxfile.c_str(), idxfile.size() + 1);
    UNSIGNED8 cmt[1] = {0};
    REQUIRE(AdsDDAddIndexFile(hConn, nm, idxf, cmt) == 0);

    REQUIRE(AdsDDSetTableProperty(
                hConn, nm, /*ADS_DD_TABLE_PRIMARY_KEY*/ 202,
                const_cast<char*>(tag),
                static_cast<UNSIGNED16>(std::strlen(tag))) == 0);
}

int sql_count(ADSHANDLE hConn, const char* sql) {
    ADSHANDLE stmt = 0;
    if (AdsCreateSQLStatement(hConn, &stmt) != 0) return -1;
    std::vector<UNSIGNED8> buf(std::strlen(sql) + 1);
    std::memcpy(buf.data(), sql, buf.size());
    ADSHANDLE cur = 0;
    if (AdsExecuteSQLDirect(stmt, buf.data(), &cur) != 0) {
        AdsCloseSQLStatement(stmt);
        return -1;
    }
    UNSIGNED32 cnt = 0;
    AdsGetRecordCount(cur, 0, &cnt);
    AdsCloseSQLStatement(stmt);
    return static_cast<int>(cnt);
}

// Run SELECT, return the trimmed values of one field, top-to-bottom.
std::vector<std::string> sql_col(ADSHANDLE hConn, const char* sql,
                                 const char* field_name) {
    std::vector<std::string> out;
    ADSHANDLE stmt = 0;
    if (AdsCreateSQLStatement(hConn, &stmt) != 0) return out;
    std::vector<UNSIGNED8> buf(std::strlen(sql) + 1);
    std::memcpy(buf.data(), sql, buf.size());
    ADSHANDLE cur = 0;
    if (AdsExecuteSQLDirect(stmt, buf.data(), &cur) != 0) {
        AdsCloseSQLStatement(stmt);
        return out;
    }
    UNSIGNED8 fname[64];
    std::memcpy(fname, field_name, std::strlen(field_name) + 1);
    AdsGotoTop(cur);
    for (int i = 0; i < 300; ++i) {
        UNSIGNED16 eof = 0;
        AdsAtEOF(cur, &eof);
        if (eof) break;
        UNSIGNED8 vbuf[256] = {};
        UNSIGNED32 vlen = sizeof(vbuf) - 1;
        AdsGetField(cur, fname, vbuf, &vlen, 0);
        std::string s(reinterpret_cast<const char*>(vbuf), vlen);
        while (!s.empty() && s.back() == ' ') s.pop_back();
        out.push_back(s);
        AdsSkip(cur, 1);
    }
    AdsCloseSQLStatement(stmt);
    return out;
}

ADSHANDLE connect_dd(const fs::path& dir) {
    UNSIGNED8 addpath[512];
    auto ap = (dir / "test.add").string();
    std::memcpy(addpath, ap.c_str(), ap.size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(addpath, 1, nullptr, nullptr, 0, &hConn) == 0);
    return hConn;
}

} // namespace

// ---------------------------------------------------------------------------
// system.primarykeys — single-column primary key
// ---------------------------------------------------------------------------

TEST_CASE("system.primarykeys reports the column of a single-field PK") {
    const auto dir = fs::temp_directory_path() / "openads_syspk_single";
    std::error_code ec; fs::remove_all(dir, ec);
    fs::create_directories(dir);

    make_dbf_name_code(dir / "emp.dbf");
    openads_test::make_dd(dir / "test.add", "TABLE emp=emp.dbf\n");

    ADSHANDLE hConn = connect_dd(dir);
    define_pk(hConn, "emp", "emp", "PKEMP", "NAME");

    CHECK(sql_count(hConn,
        "SELECT * FROM system.primarykeys WHERE TABLE_NAME = 'emp'") == 1);

    auto cols = sql_col(hConn,
        "SELECT * FROM system.primarykeys WHERE TABLE_NAME = 'emp'",
        "COLUMN_NAME");
    REQUIRE(cols.size() == 1u);
    CHECK(cols[0] == "NAME");

    auto seq = sql_col(hConn,
        "SELECT * FROM system.primarykeys WHERE TABLE_NAME = 'emp'",
        "KEY_SEQ");
    REQUIRE(seq.size() == 1u);
    CHECK(seq[0] == "1");

    auto pkn = sql_col(hConn,
        "SELECT * FROM system.primarykeys WHERE TABLE_NAME = 'emp'",
        "PK_NAME");
    REQUIRE(pkn.size() == 1u);
    CHECK(pkn[0] == "PKEMP");

    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// system.primarykeys — composite key of simple fields → one row per field
// ---------------------------------------------------------------------------

TEST_CASE("system.primarykeys reports each field of a composite PK in order") {
    const auto dir = fs::temp_directory_path() / "openads_syspk_comp";
    std::error_code ec; fs::remove_all(dir, ec);
    fs::create_directories(dir);

    make_dbf_name_code(dir / "emp.dbf");
    openads_test::make_dd(dir / "test.add", "TABLE emp=emp.dbf\n");

    ADSHANDLE hConn = connect_dd(dir);
    define_pk(hConn, "emp", "emp", "PKEMP", "NAME+CODE");

    CHECK(sql_count(hConn,
        "SELECT * FROM system.primarykeys WHERE TABLE_NAME = 'emp'") == 2);

    auto cols = sql_col(hConn,
        "SELECT * FROM system.primarykeys WHERE TABLE_NAME = 'emp'",
        "COLUMN_NAME");
    REQUIRE(cols.size() == 2u);
    CHECK(cols[0] == "NAME");
    CHECK(cols[1] == "CODE");

    auto seq = sql_col(hConn,
        "SELECT * FROM system.primarykeys WHERE TABLE_NAME = 'emp'",
        "KEY_SEQ");
    REQUIRE(seq.size() == 2u);
    CHECK(seq[0] == "1");
    CHECK(seq[1] == "2");

    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// system.primarykeys — calculated expression degrades to zero rows
// (never report a guessed column).
// ---------------------------------------------------------------------------

TEST_CASE("system.primarykeys emits no rows for a calculated PK expression") {
    const auto dir = fs::temp_directory_path() / "openads_syspk_calc";
    std::error_code ec; fs::remove_all(dir, ec);
    fs::create_directories(dir);

    make_dbf_name_code(dir / "emp.dbf");
    openads_test::make_dd(dir / "test.add", "TABLE emp=emp.dbf\n");

    ADSHANDLE hConn = connect_dd(dir);
    define_pk(hConn, "emp", "emp", "PKEMP", "UPPER(NAME)");

    CHECK(sql_count(hConn,
        "SELECT * FROM system.primarykeys WHERE TABLE_NAME = 'emp'") == 0);

    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// system.primarykeys — table with no primary key contributes no rows,
// and the system table itself resolves (not a 5018 unknown-table error).
// ---------------------------------------------------------------------------

TEST_CASE("system.primarykeys is empty for a table without a primary key") {
    const auto dir = fs::temp_directory_path() / "openads_syspk_none";
    std::error_code ec; fs::remove_all(dir, ec);
    fs::create_directories(dir);

    make_dbf_name_code(dir / "emp.dbf");
    openads_test::make_dd(dir / "test.add", "TABLE emp=emp.dbf\n");

    ADSHANDLE hConn = connect_dd(dir);

    CHECK(sql_count(hConn, "SELECT * FROM system.primarykeys") == 0);

    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}
