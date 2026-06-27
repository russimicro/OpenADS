#include "doctest.h"
#include "network/socket.h"
#include "network/wire.h"

#include <chrono>
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
using openads::network::PollItem;
using openads::network::PollEvent;
using openads::network::socket_poll;

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

TEST_CASE("socket_poll reports a readable socket and times out cleanly") {
    auto l = listen_tcp({"127.0.0.1", 0, 16});
    REQUIRE(l.has_value());
    auto port = socket_local_port(l.value());
    REQUIRE(port.has_value());
    auto c = connect_tcp("127.0.0.1", port.value());
    REQUIRE(c.has_value());
    auto a = accept_one(l.value());
    REQUIRE(a.has_value());
    Socket cs = c.value();
    Socket as = a.value();

    // Nothing sent yet: poll on the accepted socket times out (0 ready).
    {
        std::vector<PollItem> items{
            {as, static_cast<std::uint8_t>(PollEvent::Readable)}};
        auto n = socket_poll(items, 100);
        REQUIRE(n.has_value());
        CHECK(n.value() == 0);
        CHECK((items[0].events &
               static_cast<std::uint8_t>(PollEvent::Readable)) == 0);
    }

    // Send a byte from the client; the accepted socket becomes readable.
    const std::uint8_t msg[1] = {42};
    auto sent = sock_send(cs, msg, 1);
    REQUIRE(sent.has_value());
    {
        std::vector<PollItem> items{
            {as, static_cast<std::uint8_t>(PollEvent::Readable)}};
        auto n = socket_poll(items, 1000);
        REQUIRE(n.has_value());
        CHECK(n.value() == 1);
        CHECK((items[0].events &
               static_cast<std::uint8_t>(PollEvent::Readable)) != 0);
    }

    sock_close(cs);
    sock_close(as);
    sock_close(l.value());
}

TEST_CASE("socket_set_nonblocking makes an idle recv report would-block") {
    ListenerOptions o;
    o.host = "127.0.0.1";
    o.port = 0;
    auto lis = listen_tcp(o);
    REQUIRE(lis.has_value());
    auto port = socket_local_port(lis.value());
    REQUIRE(port.has_value());
    auto p = port.value();

    // Client connects but stays quiet for a beat, then sends one byte.
    std::thread client([&]() {
        auto c = connect_tcp("127.0.0.1", p);
        if (!c) return;
        Socket cs = c.value();
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
        std::uint8_t b = 0x42;
        (void)sock_send(cs, &b, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
        sock_close(cs);
    });

    Socket listener = lis.value();
    auto acc = accept_one(listener);
    REQUIRE(acc.has_value());
    Socket s = acc.value();

    REQUIRE(openads::network::socket_set_nonblocking(s, true).has_value());

    // Nothing sent yet: recv must return a would-block error (not block, not a
    // fatal error). If set_nonblocking were a no-op this recv would hang.
    std::uint8_t buf[8];
    auto r = sock_recv(s, buf, sizeof(buf));
    CHECK_FALSE(r.has_value());
    CHECK(openads::network::socket_recv_would_block(r.error()));

    client.join();
    sock_close(s);
    sock_close(listener);
}
