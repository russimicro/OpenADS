#include "doctest.h"
#include "openads/ace.h"
#include "openads/error.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

// Write a minimal text-format DD that requires login.
// Creates one user "alice" with password "secret" (prop_1101).
fs::path make_auth_add(const fs::path& dir, bool require_login) {
    auto p = dir / "test.add";
    std::ofstream f(p);
    f << "# OpenADS Data Dictionary v1\n"
      << "TABLE tbl=tbl.dbf\n"
      << "USER alice\n"
      << "USERPROP alice;prop_1101=secret\n";
    if (require_login)
        f << "DBPROP prop_5=1\n";
    return p;
}

// Write a minimal 1-field DBF so the table open doesn't fail.
void make_tbl_dbf(const fs::path& dir) {
    fs::create_directories(dir);
    auto p = dir / "tbl.dbf";
    std::vector<std::uint8_t> file;
    auto push = [&](const void* d, std::size_t n) {
        const auto* b = static_cast<const std::uint8_t*>(d);
        file.insert(file.end(), b, b + n);
    };
    std::array<std::uint8_t, 32> hdr{};
    hdr[0] = 0x03;
    hdr[8] = 32 + 32 + 1;
    hdr[10] = 1 + 4;
    push(hdr.data(), hdr.size());
    std::array<std::uint8_t, 32> f1{};
    std::strncpy(reinterpret_cast<char*>(f1.data()), "ID", 11);
    f1[11] = 'C'; f1[16] = 4;
    push(f1.data(), f1.size());
    file.push_back(0x0D);
    file.push_back(0x1A);
    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(file.data()),
        static_cast<std::streamsize>(file.size()));
}

ADSHANDLE connect_dd(const fs::path& add_path,
                     const char* user = nullptr,
                     const char* pwd  = nullptr) {
    ADSHANDLE h = 0;
    UNSIGNED8 srv[512];
    auto s = add_path.string();
    std::memcpy(srv, s.c_str(), s.size() + 1);

    auto to_u8 = [](const char* src, UNSIGNED8* dst, std::size_t cap) {
        if (!src) return (UNSIGNED8*)nullptr;
        std::size_t n = std::strlen(src);
        if (n >= cap) n = cap - 1;
        std::memcpy(dst, src, n);
        dst[n] = '\0';
        return dst;
    };
    UNSIGNED8 ubuf[64]{};
    UNSIGNED8 pbuf[64]{};
    UNSIGNED8* pu = to_u8(user, ubuf, sizeof(ubuf));
    UNSIGNED8* pp = to_u8(pwd,  pbuf, sizeof(pbuf));

    AdsConnect60(srv, ADS_LOCAL_SERVER, pu, pp, 0, &h);
    return h;
}

}  // namespace

// ---------------------------------------------------------------------------

TEST_CASE("DD auth: valid credentials accepted") {
    auto dir = fs::temp_directory_path() / "openads_auth_ok";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_tbl_dbf(dir);
    make_auth_add(dir, /*require_login=*/true);

    ADSHANDLE h = connect_dd(dir / "test.add", "alice", "secret");
    CHECK(h != 0);
    if (h) REQUIRE(AdsDisconnect(h) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("DD auth: wrong password rejected") {
    auto dir = fs::temp_directory_path() / "openads_auth_badpwd";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_tbl_dbf(dir);
    make_auth_add(dir, true);

    ADSHANDLE h = 0;
    UNSIGNED8 srv[512];
    auto s = (dir / "test.add").string();
    std::memcpy(srv, s.c_str(), s.size() + 1);
    UNSIGNED8 u[8] = "alice";
    UNSIGNED8 p[8] = "wrong";
    UNSIGNED32 rc = AdsConnect60(srv, ADS_LOCAL_SERVER, u, p, 0, &h);
    CHECK(rc == openads::AE_LOGIN_FAILED);
    CHECK(h == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("DD auth: unknown user rejected") {
    auto dir = fs::temp_directory_path() / "openads_auth_baduser";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_tbl_dbf(dir);
    make_auth_add(dir, true);

    ADSHANDLE h = 0;
    UNSIGNED8 srv[512];
    auto s = (dir / "test.add").string();
    std::memcpy(srv, s.c_str(), s.size() + 1);
    UNSIGNED8 u[8] = "bob";
    UNSIGNED8 p[8] = "secret";
    UNSIGNED32 rc = AdsConnect60(srv, ADS_LOCAL_SERVER, u, p, 0, &h);
    CHECK(rc == openads::AE_LOGIN_FAILED);
    CHECK(h == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("DD auth: missing username rejected when login required") {
    auto dir = fs::temp_directory_path() / "openads_auth_nouser";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_tbl_dbf(dir);
    make_auth_add(dir, true);

    ADSHANDLE h = 0;
    UNSIGNED8 srv[512];
    auto s = (dir / "test.add").string();
    std::memcpy(srv, s.c_str(), s.size() + 1);
    UNSIGNED32 rc = AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &h);
    CHECK(rc == openads::AE_LOGIN_FAILED);
    CHECK(h == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("DD auth: no login required — anonymous connect succeeds") {
    auto dir = fs::temp_directory_path() / "openads_auth_anon";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    make_tbl_dbf(dir);
    make_auth_add(dir, /*require_login=*/false);

    ADSHANDLE h = connect_dd(dir / "test.add");
    CHECK(h != 0);
    if (h) REQUIRE(AdsDisconnect(h) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("DD auth: no DD — plain directory connect always succeeds") {
    auto dir = fs::temp_directory_path() / "openads_auth_nodd";
    std::error_code ec;
    fs::remove_all(dir, ec);
    make_tbl_dbf(dir);

    ADSHANDLE h = 0;
    UNSIGNED8 srv[512];
    auto s = dir.string();
    std::memcpy(srv, s.c_str(), s.size() + 1);
    UNSIGNED32 rc = AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &h);
    CHECK(rc == 0);
    CHECK(h != 0);
    if (h) REQUIRE(AdsDisconnect(h) == 0);
    fs::remove_all(dir, ec);
}
