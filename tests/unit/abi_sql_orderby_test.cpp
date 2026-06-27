#include "doctest.h"
#include "openads/ace.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

fs::path stage_dbf(const fs::path& dir) {
    fs::create_directories(dir);
    auto p = dir / "data.dbf";
    fs::remove(p);
    std::vector<std::uint8_t> file;
    auto push = [&](const void* d, std::size_t n) {
        const auto* b = static_cast<const std::uint8_t*>(d);
        file.insert(file.end(), b, b + n);
    };
    std::array<std::uint8_t, 32> hdr{};
    hdr[0]  = 0x03;
    hdr[4]  = 4;
    hdr[8]  = 32 + 32 + 1;
    hdr[10] = 1 + 4;
    push(hdr.data(), hdr.size());
    std::array<std::uint8_t, 32> fd{};
    std::strncpy(reinterpret_cast<char*>(fd.data()), "TAG", 11);
    fd[11] = 'C'; fd[16] = 4;
    push(fd.data(), fd.size());
    file.push_back(0x0D);
    auto rec = [&](const char* s) {
        file.push_back(' ');
        for (int i = 0; i < 4; ++i)
            file.push_back(i < (int)std::strlen(s)
                           ? static_cast<std::uint8_t>(s[i]) : ' ');
    };
    rec("CCCC"); rec("AAAA"); rec("DDDD"); rec("BBBB");
    file.push_back(0x1A);
    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(file.data()),
        static_cast<std::streamsize>(file.size()));
    return p;
}

std::vector<UNSIGNED32> walk(ADSHANDLE hCur) {
    std::vector<UNSIGNED32> out;
    if (AdsGotoTop(hCur) != 0) return out;
    while (true) {
        UNSIGNED16 atend = 0;
        if (AdsAtEOF(hCur, &atend) != 0 || atend) break;
        UNSIGNED32 r = 0;
        if (AdsGetRecordNum(hCur, 0, &r) != 0) break;
        out.push_back(r);
        if (AdsSkip(hCur, 1) != 0) break;
    }
    return out;
}

// The ORDER BY result is now a materialised static cursor (ADS_CDX semantics):
// its recnos are 1..N in result order, so asserting source recnos no longer
// distinguishes ASC from DESC. Walk the sorted COLUMN VALUE instead — that is
// the actual ORDER BY contract.
std::vector<std::string> walk_vals(ADSHANDLE hCur, const char* name) {
    std::vector<std::string> out;
    if (AdsGotoTop(hCur) != 0) return out;
    UNSIGNED8 fn[16] = {0};
    std::memcpy(fn, name, std::strlen(name) + 1);
    while (true) {
        UNSIGNED16 atend = 0;
        if (AdsAtEOF(hCur, &atend) != 0 || atend) break;
        UNSIGNED8 buf[32] = {0};
        UNSIGNED32 cap = sizeof(buf);
        std::string v;
        if (AdsGetField(hCur, fn, buf, &cap, 0) == 0) {
            v.assign(reinterpret_cast<char*>(buf), cap);
            while (!v.empty() && v.back() == ' ') v.pop_back();
        }
        out.push_back(v);
        if (AdsSkip(hCur, 1) != 0) break;
    }
    return out;
}

}  // namespace

TEST_CASE("M10.6 SQL ORDER BY ascending walks rows in sorted order") {
    auto dir = fs::temp_directory_path() / "openads_m10_6_asc";
    std::error_code ec;
    fs::remove_all(dir, ec);
    stage_dbf(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);
    ADSHANDLE hStmt = 0;
    REQUIRE(AdsCreateSQLStatement(hConn, &hStmt) == 0);

    UNSIGNED8 sql[160] = "SELECT * FROM data.dbf ORDER BY TAG";
    ADSHANDLE hCur = 0;
    REQUIRE(AdsExecuteSQLDirect(hStmt, sql, &hCur) == 0);

    auto seq = walk_vals(hCur, "TAG");
    REQUIRE(seq.size() == 4);
    CHECK(seq[0] == "AAAA");
    CHECK(seq[1] == "BBBB");
    CHECK(seq[2] == "CCCC");
    CHECK(seq[3] == "DDDD");

    REQUIRE(AdsCloseSQLStatement(hStmt) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("M10.6 SQL ORDER BY DESC reverses the order") {
    auto dir = fs::temp_directory_path() / "openads_m10_6_desc";
    std::error_code ec;
    fs::remove_all(dir, ec);
    stage_dbf(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);
    ADSHANDLE hStmt = 0;
    REQUIRE(AdsCreateSQLStatement(hConn, &hStmt) == 0);

    UNSIGNED8 sql[160] = "SELECT * FROM data.dbf ORDER BY TAG DESC";
    ADSHANDLE hCur = 0;
    REQUIRE(AdsExecuteSQLDirect(hStmt, sql, &hCur) == 0);

    auto seq = walk_vals(hCur, "TAG");
    REQUIRE(seq.size() == 4);
    CHECK(seq[0] == "DDDD");
    CHECK(seq[1] == "CCCC");
    CHECK(seq[2] == "BBBB");
    CHECK(seq[3] == "AAAA");

    REQUIRE(AdsCloseSQLStatement(hStmt) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

namespace {
fs::path stage_two_col_dbf(const fs::path& dir) {
    fs::create_directories(dir);
    auto p = dir / "data.dbf";
    fs::remove(p);
    std::vector<std::uint8_t> file;
    auto push = [&](const void* d, std::size_t n) {
        const auto* b = static_cast<const std::uint8_t*>(d);
        file.insert(file.end(), b, b + n);
    };
    std::array<std::uint8_t, 32> hdr{};
    hdr[0]  = 0x03;
    hdr[4]  = 5;
    std::uint16_t hl = 32 + 32 * 2 + 1;
    std::uint16_t rl = 1 + 4 + 2;
    hdr[8]  = static_cast<std::uint8_t>( hl       & 0xFFu);
    hdr[9]  = static_cast<std::uint8_t>((hl >> 8) & 0xFFu);
    hdr[10] = static_cast<std::uint8_t>( rl       & 0xFFu);
    hdr[11] = static_cast<std::uint8_t>((rl >> 8) & 0xFFu);
    push(hdr.data(), hdr.size());
    auto fld = [&](const char* nm, std::uint8_t L) {
        std::array<std::uint8_t, 32> fd{};
        std::strncpy(reinterpret_cast<char*>(fd.data()), nm, 11);
        fd[11] = 'C'; fd[16] = L;
        push(fd.data(), fd.size());
    };
    fld("CITY", 4);
    fld("PR",   2);
    file.push_back(0x0D);
    auto rec = [&](const char* city, const char* pr) {
        file.push_back(' ');
        for (int i = 0; i < 4; ++i)
            file.push_back(i < (int)std::strlen(city)
                           ? static_cast<std::uint8_t>(city[i]) : ' ');
        for (int i = 0; i < 2; ++i)
            file.push_back(i < (int)std::strlen(pr)
                           ? static_cast<std::uint8_t>(pr[i]) : ' ');
    };
    rec("LON ", "20");                           // recno 1
    rec("LON ", "10");                           // recno 2
    rec("NYC ", "30");                           // recno 3
    rec("NYC ", "20");                           // recno 4
    rec("NYC ", "10");                           // recno 5
    file.push_back(0x1A);
    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(file.data()),
        static_cast<std::streamsize>(file.size()));
    return p;
}
}

TEST_CASE("M10.37 multi-column ORDER BY cascades on ties") {
    auto dir = fs::temp_directory_path() / "openads_m10_37";
    std::error_code ec;
    fs::remove_all(dir, ec);
    stage_two_col_dbf(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);
    ADSHANDLE hStmt = 0;
    REQUIRE(AdsCreateSQLStatement(hConn, &hStmt) == 0);

    // ORDER BY CITY ASC, PR DESC.
    // CITY=LON: PR 20,10 → DESC = 20,10 → recnos 1, 2
    // CITY=NYC: PR 30,20,10 → DESC = 30,20,10 → recnos 3, 4, 5
    UNSIGNED8 sql[200] =
        "SELECT * FROM data.dbf ORDER BY CITY ASC, PR DESC";
    ADSHANDLE hCur = 0;
    REQUIRE(AdsExecuteSQLDirect(hStmt, sql, &hCur) == 0);

    auto seq = walk(hCur);
    REQUIRE(seq.size() == 5);
    CHECK(seq[0] == 1);
    CHECK(seq[1] == 2);
    CHECK(seq[2] == 3);
    CHECK(seq[3] == 4);
    CHECK(seq[4] == 5);

    REQUIRE(AdsCloseSQLStatement(hStmt) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("M10.6 SQL ORDER BY combines with WHERE") {
    auto dir = fs::temp_directory_path() / "openads_m10_6_where";
    std::error_code ec;
    fs::remove_all(dir, ec);
    stage_dbf(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);
    ADSHANDLE hStmt = 0;
    REQUIRE(AdsCreateSQLStatement(hConn, &hStmt) == 0);

    // TAG > 'AAAA' filters out AAAA. ORDER BY TAG → BBBB, CCCC, DDDD.
    UNSIGNED8 sql[200] =
        "SELECT * FROM data.dbf WHERE TAG > 'AAAA' ORDER BY TAG";
    ADSHANDLE hCur = 0;
    REQUIRE(AdsExecuteSQLDirect(hStmt, sql, &hCur) == 0);

    auto seq = walk_vals(hCur, "TAG");
    REQUIRE(seq.size() == 3);
    CHECK(seq[0] == "BBBB");
    CHECK(seq[1] == "CCCC");
    CHECK(seq[2] == "DDDD");

    REQUIRE(AdsCloseSQLStatement(hStmt) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}
