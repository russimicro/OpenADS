#include "doctest.h"
#include "openads/ace.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
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
    hdr[4]  = 3;
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
    rec("BBBB"); rec("AAAA"); rec("CCCC");
    file.push_back(0x1A);
    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(file.data()),
        static_cast<std::streamsize>(file.size()));
    return p;
}

} // namespace

TEST_CASE("M9.7 AdsCreateIndex61 builds CDX dynamically + dbSeek works") {
    auto dir = fs::temp_directory_path() / "openads_m97_ci61";
    std::error_code ec;
    fs::remove_all(dir, ec);
    stage_dbf(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    ADSHANDLE hTable = 0;
    UNSIGNED8 name[16] = "data";
    REQUIRE(AdsOpenTable(hConn, name, name, ADS_CDX, 1, 1, 0, 1, &hTable) == 0);

    UNSIGNED8 idxfile[16] = "data";
    UNSIGNED8 idxname[16] = "TAG";
    UNSIGNED8 expr[16]    = "TAG";
    ADSHANDLE hIdx = 0;
    REQUIRE(AdsCreateIndex61(hTable, idxfile, idxname, expr,
                             nullptr, nullptr,
                             0, 512, &hIdx) == 0);

    // Records arrived BBBB, AAAA, CCCC (recnos 1,2,3). After the
    // index, AdsGotoTop should land on AAAA at recno 2.
    REQUIRE(AdsGotoTop(hTable) == 0);
    UNSIGNED32 recno = 0;
    REQUIRE(AdsGetRecordNum(hTable, 0, &recno) == 0);
    CHECK(recno == 2);

    // Seek by key.
    UNSIGNED8 key[8] = "BBBB";
    UNSIGNED16 found = 0;
    REQUIRE(AdsSeek(hIdx, key, 4, 0, 0, &found) == 0);
    CHECK(found == 1);
    REQUIRE(AdsGetRecordNum(hTable, 0, &recno) == 0);
    CHECK(recno == 1);

    REQUIRE(AdsCloseIndex(hIdx) == 0);
    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("M9.7 AdsCreateIndex61 supports compound expressions (UPPER)") {
    auto dir = fs::temp_directory_path() / "openads_m97_upper";
    std::error_code ec;
    fs::remove_all(dir, ec);
    // DBF with mixed-case names.
    fs::create_directories(dir);
    auto p = dir / "data.dbf";
    std::vector<std::uint8_t> file;
    auto push = [&](const void* d, std::size_t n) {
        const auto* b = static_cast<const std::uint8_t*>(d);
        file.insert(file.end(), b, b + n);
    };
    std::array<std::uint8_t, 32> hdr{};
    hdr[0]  = 0x03; hdr[4]  = 2;
    hdr[8]  = 32 + 32 + 1; hdr[10] = 1 + 5;
    push(hdr.data(), hdr.size());
    std::array<std::uint8_t, 32> fd{};
    std::strncpy(reinterpret_cast<char*>(fd.data()), "NAME", 11);
    fd[11] = 'C'; fd[16] = 5;
    push(fd.data(), fd.size());
    file.push_back(0x0D);
    file.push_back(' '); push("alpha", 5);
    file.push_back(' '); push("BeTa ", 5);
    file.push_back(0x1A);
    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(file.data()),
        static_cast<std::streamsize>(file.size()));

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    ADSHANDLE hTable = 0;
    UNSIGNED8 nm[16] = "data";
    REQUIRE(AdsOpenTable(hConn, nm, nm, ADS_CDX, 1, 1, 0, 1, &hTable) == 0);

    UNSIGNED8 fn[16]   = "data";
    UNSIGNED8 tag[16]  = "UPNAME";
    UNSIGNED8 expr[32] = "UPPER(NAME)";
    ADSHANDLE hIdx = 0;
    REQUIRE(AdsCreateIndex61(hTable, fn, tag, expr,
                             nullptr, nullptr,
                             0, 512, &hIdx) == 0);

    // Seek 'ALPHA' (upper) — should hit the 'alpha' record (recno 1).
    UNSIGNED8 key[8] = "ALPHA";
    UNSIGNED16 found = 0;
    REQUIRE(AdsSeek(hIdx, key, 5, 0, 0, &found) == 0);
    CHECK(found == 1);
    UNSIGNED32 recno = 0;
    REQUIRE(AdsGetRecordNum(hTable, 0, &recno) == 0);
    CHECK(recno == 1);

    // Seek 'BETA ' (upper) — recno 2.
    UNSIGNED8 key2[8] = "BETA ";
    found = 0;
    REQUIRE(AdsSeek(hIdx, key2, 5, 0, 0, &found) == 0);
    CHECK(found == 1);
    REQUIRE(AdsGetRecordNum(hTable, 0, &recno) == 0);
    CHECK(recno == 2);

    REQUIRE(AdsCloseIndex(hIdx) == 0);
    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("M12.23 AdsCreateIndex61 option bits: ADS_COMPOUND is not ADS_DESCENDING") {
    // rddads / X#'s ADSRDD always pass ADS_COMPOUND when creating a CDX
    // order. That bit must be ignored, not treated as ADS_DESCENDING —
    // otherwise every order is built descending and DbGoTop lands on the
    // last key. Real option-bit values (verified against rddads, in
    // include/openads/ace.h): ADS_COMPOUND 0x02, ADS_DESCENDING 0x08.
    // The literal-0x02 case below pins what rddads actually sends —
    // testing only the symbol ADS_COMPOUND let the 0x02/0x08 swap ship.
    // Fixture records: BBBB(1), AAAA(2), CCCC(3).
    auto dir = fs::temp_directory_path() / "openads_m1223_ci_opts";
    std::error_code ec;

    auto run = [&](UNSIGNED32 opts, UNSIGNED32 expectTopRecno) {
        fs::remove_all(dir, ec);
        stage_dbf(dir);
        UNSIGNED8 srv[256];
        std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
        ADSHANDLE hConn = 0;
        REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);
        ADSHANDLE hTable = 0;
        UNSIGNED8 name[16] = "data";
        REQUIRE(AdsOpenTable(hConn, name, name, ADS_CDX, 1, 1, 0, 1, &hTable) == 0);
        UNSIGNED8 idxfile[16] = "data";
        UNSIGNED8 idxname[16] = "TAG";
        UNSIGNED8 expr[16]    = "TAG";
        ADSHANDLE hIdx = 0;
        REQUIRE(AdsCreateIndex61(hTable, idxfile, idxname, expr,
                                 nullptr, nullptr, opts, 512, &hIdx) == 0);
        REQUIRE(AdsGotoTop(hTable) == 0);
        UNSIGNED32 recno = 0;
        REQUIRE(AdsGetRecordNum(hTable, 0, &recno) == 0);
        CHECK(recno == expectTopRecno);
        REQUIRE(AdsCloseIndex(hIdx) == 0);
        REQUIRE(AdsCloseTable(hTable) == 0);
        REQUIRE(AdsDisconnect(hConn) == 0);
    };

    run(ADS_COMPOUND, 2u);                        // ascending → AAAA (rec 2)
    run(ADS_COMPOUND | ADS_DESCENDING, 3u);       // descending → CCCC (rec 3)
    run(ADS_DESCENDING, 3u);                      // descending → CCCC (rec 3)
    // Regression: the EXACT raw bits rddads sends, as literals — these
    // must not depend on the symbol values staying correct.
    run(0x02u, 2u);                               // rddads `INDEX ON` → ascending
    run(0x0Au, 3u);                               // rddads `... DESCENDING` → descending
    fs::remove_all(dir, ec);
}
