#include "doctest.h"
#include "openads/ace.h"

#include <array>
#include <cstdint>

extern "C" {

UNSIGNED32 AdsDDAddIndexFile      (ADSHANDLE, UNSIGNED8*, UNSIGNED8*, UNSIGNED8*);
UNSIGNED32 AdsDDRemoveIndexFile   (ADSHANDLE, UNSIGNED8*, UNSIGNED8*, UNSIGNED16);
UNSIGNED32 AdsDDCreateLink        (ADSHANDLE, UNSIGNED8*, UNSIGNED8*,
                                   UNSIGNED8*, UNSIGNED8*, UNSIGNED16);
UNSIGNED32 AdsDDDropLink          (ADSHANDLE, UNSIGNED8*, UNSIGNED16);
UNSIGNED32 AdsDDCreateUser        (ADSHANDLE, UNSIGNED8*, UNSIGNED8*,
                                   UNSIGNED8*, UNSIGNED8*);
UNSIGNED32 AdsDDDeleteUser        (ADSHANDLE, UNSIGNED8*);
UNSIGNED32 AdsDDAddUserToGroup    (ADSHANDLE, UNSIGNED8*, UNSIGNED8*);
UNSIGNED32 AdsDDRemoveUserFromGroup(ADSHANDLE, UNSIGNED8*, UNSIGNED8*);
UNSIGNED32 AdsDDCreateRefIntegrity (ADSHANDLE, UNSIGNED8*, UNSIGNED8*,
                                    UNSIGNED8*, UNSIGNED8*, UNSIGNED8*,
                                    UNSIGNED8*,
                                    UNSIGNED16, UNSIGNED16);
UNSIGNED32 AdsDDRemoveRefIntegrity (ADSHANDLE, UNSIGNED8*);
UNSIGNED32 AdsDDGetDatabaseProperty(ADSHANDLE, UNSIGNED16, void*, UNSIGNED16*);
UNSIGNED32 AdsDDSetDatabaseProperty(ADSHANDLE, UNSIGNED16, void*, UNSIGNED16);
UNSIGNED32 AdsDDGetUserProperty    (ADSHANDLE, UNSIGNED8*, UNSIGNED16,
                                    void*, UNSIGNED16*);

}  // extern "C"

TEST_CASE("M9.25 DD CRUD calls accept silently and return AE_SUCCESS") {
    UNSIGNED8 alias[16] = "remote";
    UNSIGNED8 path[32]  = "C:/data/remote.add";
    UNSIGNED8 user[16]  = "alice";
    UNSIGNED8 group[16] = "admins";
    UNSIGNED8 pwd[16]   = "secret";
    UNSIGNED8 desc[32]  = "test user";
    UNSIGNED8 tbl[16]   = "data";
    UNSIGNED8 idx[16]   = "data.cdx";
    UNSIGNED8 cmt[16]   = "ix";
    UNSIGNED8 ri[16]    = "ri1";
    UNSIGNED8 fail[16]  = "fail.dbf";
    UNSIGNED8 par[16]   = "parent";
    UNSIGNED8 chi[16]   = "child";
    UNSIGNED8 tag[16]   = "T";

    REQUIRE(AdsDDAddIndexFile(0, tbl, idx, cmt) == 0);
    REQUIRE(AdsDDRemoveIndexFile(0, tbl, idx, 0) == 0);

    REQUIRE(AdsDDCreateLink(0, alias, path, user, pwd, 0) == 0);
    REQUIRE(AdsDDDropLink(0, alias, 0) == 0);

    REQUIRE(AdsDDCreateUser(0, group, user, pwd, desc) == 0);
    REQUIRE(AdsDDAddUserToGroup(0, group, user) == 0);
    REQUIRE(AdsDDRemoveUserFromGroup(0, group, user) == 0);
    REQUIRE(AdsDDDeleteUser(0, user) == 0);

    REQUIRE(AdsDDCreateRefIntegrity(0, ri, fail, par, tag, chi, tag,
                                    0, 0) == 0);
    REQUIRE(AdsDDRemoveRefIntegrity(0, ri) == 0);
}

TEST_CASE("M9.25 DD property getters zero-fill caller buffer") {
    std::array<std::uint8_t, 32> buf;
    buf.fill(0xCD);
    UNSIGNED16 sz = static_cast<UNSIGNED16>(buf.size());
    REQUIRE(AdsDDGetDatabaseProperty(0, /*usProp=*/1, buf.data(), &sz) == 0);
    for (auto b : buf) CHECK(b == 0);
    CHECK(sz == 0);

    buf.fill(0xEE);
    sz = static_cast<UNSIGNED16>(buf.size());
    UNSIGNED8 user[8] = "alice";
    REQUIRE(AdsDDGetUserProperty(0, user, 1, buf.data(), &sz) == 0);
    for (auto b : buf) CHECK(b == 0);
    CHECK(sz == 0);
}

TEST_CASE("M9.25 DD property setter accepts silently") {
    UNSIGNED8 buf[16] = "anything";
    REQUIRE(AdsDDSetDatabaseProperty(0, 1, buf, 8) == 0);
}
