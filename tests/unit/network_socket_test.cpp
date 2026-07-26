#include "doctest.h"
#include "network/socket.h"
#include "network/wire.h"

#include <cstdint>
#include <cstring>
#include <string>
#include <thread>
#include <vector>

using openads::network::Socket;
using openads::network::ListenerOptions;
using openads::network::listen_tcp;
using openads::network::socket_local_port;
using openads::network::accept_one;
using openads::network::connect_tcp;
using openads::network::sock_send;
using openads::network::sock_recv;
using openads::network::sock_close;
using openads::network::Frame;
using openads::network::Opcode;
using openads::network::encode_frame;
using openads::network::decode_frame;

TEST_CASE("M12.2 listen / connect / send / recv round-trip a frame") {
    ListenerOptions opts;
    opts.host = "127.0.0.1";
    opts.port = 0;                 // ephemeral
    auto listener = listen_tcp(opts);
    REQUIRE(listener.has_value());

    auto port = socket_local_port(listener.value());
    REQUIRE(port.has_value());
    auto p = port.value();

    // Server thread: accept one + read a frame.
    std::vector<std::uint8_t> received;
    Socket server_listener = listener.value();
    std::thread server([&]() {
        auto cli = accept_one(server_listener);
        if (!cli) return;
        Socket s = cli.value();
        std::uint8_t buf[256];
        auto n = sock_recv(s, buf, sizeof(buf));
        if (n.has_value()) {
            received.assign(buf, buf + n.value());
        }
        sock_close(s);
    });

    auto cli = connect_tcp("127.0.0.1", p);
    REQUIRE(cli.has_value());
    Socket cs = cli.value();

    Frame f;
    f.opcode = Opcode::Hello;
    std::string hello = "openads-0.3";
    f.payload.assign(hello.begin(), hello.end());
    auto enc = encode_frame(f);
    REQUIRE(enc.has_value());
    auto sent = sock_send(cs, enc.value().data(), enc.value().size());
    REQUIRE(sent.has_value());
    CHECK(sent.value() == enc.value().size());

    sock_close(cs);
    server.join();
    sock_close(server_listener);

    REQUIRE(received.size() >= 5);
    auto dec = decode_frame(received.data(), received.size());
    REQUIRE(dec.has_value());
    CHECK(dec.value().opcode == Opcode::Hello);
    std::string got(dec.value().payload.begin(),
                    dec.value().payload.end());
    CHECK(got == hello);
}
