#include "doctest.h"
#include "openads/ace.h"
#include "engine/data_dict.h"

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

extern "C" {
UNSIGNED32 AdsDDCreateUser        (ADSHANDLE, UNSIGNED8*, UNSIGNED8*,
                                   UNSIGNED8*, UNSIGNED8*);
UNSIGNED32 AdsDDDeleteUser        (ADSHANDLE, UNSIGNED8*);
UNSIGNED32 AdsDDAddUserToGroup    (ADSHANDLE, UNSIGNED8*, UNSIGNED8*);
UNSIGNED32 AdsDDCreateLink        (ADSHANDLE, UNSIGNED8*, UNSIGNED8*,
                                   UNSIGNED8*, UNSIGNED8*, UNSIGNED16);
UNSIGNED32 AdsDDAddIndexFile      (ADSHANDLE, UNSIGNED8*, UNSIGNED8*, UNSIGNED8*);
UNSIGNED32 AdsDDCreateRefIntegrity(ADSHANDLE, UNSIGNED8*, UNSIGNED8*,
                                   UNSIGNED8*, UNSIGNED8*, UNSIGNED8*,
                                   UNSIGNED8*,
                                   UNSIGNED16, UNSIGNED16);
UNSIGNED32 AdsDDSetDatabaseProperty(ADSHANDLE, UNSIGNED16, void*, UNSIGNED16);
UNSIGNED32 AdsDDGetDatabaseProperty(ADSHANDLE, UNSIGNED16, void*, UNSIGNED16*);
}  // extern "C"

namespace {

fs::path stage_dd(const fs::path& dir) {
    fs::create_directories(dir);
    auto add_path = dir / "openads.add";
    auto created = openads::engine::DataDict::create(add_path.string());
    REQUIRE(created.has_value());
    return add_path;
}

}  // namespace

TEST_CASE("M10.1 DD CRUD round-trips through .add reopen") {
    auto dir = fs::temp_directory_path() / "openads_m10_1_dd";
    std::error_code ec;
    fs::remove_all(dir, ec);
    auto add_path = stage_dd(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, add_path.string().c_str(), add_path.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 user[16]  = "alice";
    UNSIGNED8 group[16] = "admins";
    UNSIGNED8 pwd[16]   = "secret";
    UNSIGNED8 desc[16]  = "test";
    UNSIGNED8 alias[16] = "remote";
    UNSIGNED8 path[64]  = "C:/data/remote.add";
    UNSIGNED8 tbl[16]   = "data";
    UNSIGNED8 idx[16]   = "data.cdx";
    UNSIGNED8 cmt[16]   = "main idx";
    UNSIGNED8 ri[16]    = "ri1";
    UNSIGNED8 fail[16]  = "fail.dbf";
    UNSIGNED8 par[16]   = "parent";
    UNSIGNED8 chi[16]   = "child";
    UNSIGNED8 tag[16]   = "T";

    REQUIRE(AdsDDCreateUser(hConn, group, user, pwd, desc) == 0);
    REQUIRE(AdsDDAddUserToGroup(hConn, group, user) == 0);
    REQUIRE(AdsDDCreateLink(hConn, alias, path, user, pwd, 0) == 0);
    REQUIRE(AdsDDAddIndexFile(hConn, tbl, idx, cmt) == 0);
    REQUIRE(AdsDDCreateRefIntegrity(hConn, ri, fail, par, tag, chi, tag,
                                    1, 2) == 0);
    UNSIGNED8 propval[16] = "hello";
    REQUIRE(AdsDDSetDatabaseProperty(hConn, /*usProp=*/42,
                                     propval, 5) == 0);

    REQUIRE(AdsDisconnect(hConn) == 0);

    // Reopen — every CRUD should be visible.
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED16 plen = 16;
    UNSIGNED8 pbuf[16] = {0};
    REQUIRE(AdsDDGetDatabaseProperty(hConn, 42, pbuf, &plen) == 0);
    CHECK(plen == 5);
    CHECK(std::string(reinterpret_cast<const char*>(pbuf), plen) == "hello");

    // Direct dump for the rest — verify file content has each row.
    REQUIRE(AdsDisconnect(hConn) == 0);

    std::ifstream in(add_path);
    std::string line;
    bool saw_user = false, saw_member = false, saw_link = false,
         saw_index = false, saw_ri = false, saw_dbprop = false;
    while (std::getline(in, line)) {
        if (line.find("USER alice") != std::string::npos) saw_user = true;
        if (line.find("MEMBER alice=admins") != std::string::npos) saw_member = true;
        if (line.find("LINK remote=") != std::string::npos) saw_link = true;
        if (line.find("INDEX data=") != std::string::npos) saw_index = true;
        if (line.find("RI ri1=") != std::string::npos) saw_ri = true;
        if (line.find("DBPROP prop_42=hello") != std::string::npos)
            saw_dbprop = true;
    }
    CHECK(saw_user);
    CHECK(saw_member);
    CHECK(saw_link);
    CHECK(saw_index);
    CHECK(saw_ri);
    CHECK(saw_dbprop);

    fs::remove_all(dir, ec);
}

TEST_CASE("M10.1 DD CRUD without .add is silent-success no-op") {
    auto dir = fs::temp_directory_path() / "openads_m10_1_no_dd";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 user[16]  = "alice";
    UNSIGNED8 group[16] = "admins";
    UNSIGNED8 pwd[16]   = "secret";
    UNSIGNED8 desc[16]  = "test";

    // Without a `.add` connection these CRUD calls report success but
    // don't persist anything — matches M9.25 behaviour.
    REQUIRE(AdsDDCreateUser(hConn, group, user, pwd, desc) == 0);

    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("M10.1 DD remove-user clears membership + props") {
    auto dir = fs::temp_directory_path() / "openads_m10_1_dd_rm";
    std::error_code ec;
    fs::remove_all(dir, ec);
    auto add_path = stage_dd(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, add_path.string().c_str(), add_path.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 group[16] = "g";
    UNSIGNED8 user[16]  = "alice";
    UNSIGNED8 nullbuf[1] = {0};
    REQUIRE(AdsDDCreateUser(hConn, group, user, nullbuf, nullbuf) == 0);
    REQUIRE(AdsDDAddUserToGroup(hConn, group, user) == 0);
    REQUIRE(AdsDDDeleteUser(hConn, user) == 0);

    REQUIRE(AdsDisconnect(hConn) == 0);

    std::ifstream in(add_path);
    std::string content((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    CHECK(content.find("USER alice") == std::string::npos);
    CHECK(content.find("MEMBER alice=") == std::string::npos);

    fs::remove_all(dir, ec);
}
