// CDX on an empty table: a bare character field index must still use the
// declared field width as the on-disk key length (not the 32-byte fallback
// used when no field can be resolved and no first record is available).

#include "doctest.h"
#include "openads/ace.h"
#include "drivers/cdx/cdx_index.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;
using openads::drivers::IndexOpenMode;
using openads::drivers::cdx::CdxIndex;

namespace {

constexpr std::uint16_t kNomeLen = 30;

fs::path stage_empty_char_dbf(const fs::path& dir) {
    fs::create_directories(dir);
    auto p = dir / "empty.dbf";
    std::error_code ec;
    fs::remove(p, ec);

    std::vector<std::uint8_t> file;
    auto push = [&](const void* d, std::size_t n) {
        const auto* b = static_cast<const std::uint8_t*>(d);
        file.insert(file.end(), b, b + n);
    };

    const std::uint16_t rec_len = 1 + kNomeLen;
    const std::uint16_t hdr_len = 32 + 32 + 1;

    std::array<std::uint8_t, 32> hdr{};
    hdr[0]  = 0x03;
    hdr[4]  = 0;
    hdr[8]  = hdr_len & 0xFF; hdr[9]  = (hdr_len >> 8) & 0xFF;
    hdr[10] = rec_len & 0xFF; hdr[11] = (rec_len >> 8) & 0xFF;
    push(hdr.data(), hdr.size());

    std::array<std::uint8_t, 32> fnome{};
    std::strncpy(reinterpret_cast<char*>(fnome.data()), "NOME", 11);
    fnome[11] = 'C';
    fnome[16] = static_cast<std::uint8_t>(kNomeLen);
    fnome[17] = 0;
    push(fnome.data(), fnome.size());

    file.push_back(0x0D);
    file.push_back(0x1A);

    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(file.data()),
        static_cast<std::streamsize>(file.size()));
    return p;
}

struct TempDirGuard {
    fs::path path;
    explicit TempDirGuard(fs::path p) : path(std::move(p)) {}
    ~TempDirGuard() {
        std::error_code ec;
        fs::remove_all(path, ec);
    }
    TempDirGuard(const TempDirGuard&) = delete;
    TempDirGuard& operator=(const TempDirGuard&) = delete;
};

} // namespace

TEST_CASE("CDX empty table: bare CHAR index uses declared field width") {
    auto dir = fs::temp_directory_path() / "openads_cdx_empty_keylen";
    std::error_code ec;
    fs::remove_all(dir, ec);
    TempDirGuard cleanup{dir};
    stage_empty_char_dbf(dir);

    const auto dir_str = dir.string();
    UNSIGNED8 srv[512];
    std::memset(srv, 0, sizeof(srv));
    REQUIRE(dir_str.size() < sizeof(srv));
    std::memcpy(srv, dir_str.c_str(), dir_str.size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    ADSHANDLE hTable = 0;
    UNSIGNED8 tname[16] = "empty";
    REQUIRE(AdsOpenTable(hConn, tname, tname, ADS_CDX, 1, 1, 0, 1, &hTable) == 0);

    UNSIGNED8 idxfile[16] = "empty.cdx";
    UNSIGNED8 idxname[16] = "NOME_IDX";
    UNSIGNED8 expr[16]    = "NOME";
    ADSHANDLE hIdx = 0;
    REQUIRE(AdsCreateIndex61(hTable, idxfile, idxname, expr,
                             nullptr, nullptr, 0, 512, &hIdx) == 0);
    REQUIRE(AdsCloseIndex(hIdx) == 0);

    auto bag = (dir / "empty.cdx").string();
    REQUIRE(fs::exists(bag));

    CdxIndex ix;
    REQUIRE(ix.open_named(bag, IndexOpenMode::Shared, "NOME_IDX").has_value());
    CHECK(ix.key_length() == kNomeLen);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
}