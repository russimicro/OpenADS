#include "doctest.h"

#include "network/server.h"
#include "openads/ace.h"

#include <cstdint>
#include <string>
#include <vector>

extern "C" {
UNSIGNED32 AdsMgConnect(UNSIGNED8*, UNSIGNED8*, UNSIGNED8*, ADSHANDLE*);
UNSIGNED32 AdsMgDisconnect(ADSHANDLE);
UNSIGNED32 AdsMgGetActivityInfo(ADSHANDLE, void*, UNSIGNED16*);
}

TEST_CASE("M9.25 AdsMgGetActivityInfo over the wire sees a live session") {
    using openads::network::Server;
    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());
    std::uint16_t port = srv.port();

    Server::SessionInfo a;
    a.peer_ip = "127.0.0.1";
    a.peer_port = 6001;
    a.user = "alice";
    a.open_tables = 1;
    std::uint64_t id = srv.register_session(a);

    std::string server = "127.0.0.1:" + std::to_string(port);
    std::vector<UNSIGNED8> srvbuf(server.begin(), server.end());
    srvbuf.push_back(0);
    UNSIGNED8 usr[2] = "u";
    UNSIGNED8 pwd[2] = "p";

    ADSHANDLE h = 0;
    REQUIRE(AdsMgConnect(srvbuf.data(), usr, pwd, &h) == 0);

    ADS_MGMT_ACTIVITY_INFO act;
    UNSIGNED16 sz = sizeof(act);
    REQUIRE(AdsMgGetActivityInfo(h, &act, &sz) == 0);
    // At least the manually-registered "alice" session must show; the
    // mgmt client's own socket may or may not also be counted.
    CHECK(act.stConnections.ulInUse >= 1);

    REQUIRE(AdsMgDisconnect(h) == 0);
    srv.unregister_session(id);
    srv.stop();
}
