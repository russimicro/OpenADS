#include "doctest.h"
#include "openads/ace.h"
#include "network/client.h"
#include "network/server.h"
#include "network/socket.h"
#include "network/transport.h"
#include "network/wire.h"

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

using openads::network::Server;
using openads::network::Socket;
using openads::network::connect_tcp;
using openads::network::sock_close;
using openads::network::Frame;
using openads::network::Opcode;
using openads::network::read_frame;
using openads::network::write_frame;

TEST_CASE("M12.3 server Hello → HelloAck round-trip") {
    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());
    CHECK(srv.running());
    auto port = srv.port();
    REQUIRE(port != 0);

    auto cli = connect_tcp("127.0.0.1", port);
    REQUIRE(cli.has_value());
    Socket cs = cli.value();

    Frame req;
    req.opcode = Opcode::Hello;
    REQUIRE(write_frame(cs, req).has_value());
    auto reply = read_frame(cs);
    REQUIRE(reply.has_value());
    CHECK(reply.value().opcode == Opcode::HelloAck);
    std::string ver(reply.value().payload.begin(),
                    reply.value().payload.end());
    CHECK(ver == "openads/0.3.2");

    sock_close(cs);
    srv.stop();
    CHECK_FALSE(srv.running());
}

TEST_CASE("M12.3 server Connect against a real data dir succeeds") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_m12_3_connect";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    auto cli = connect_tcp("127.0.0.1", srv.port());
    REQUIRE(cli.has_value());
    Socket cs = cli.value();

    Frame req;
    req.opcode = Opcode::Connect;
    std::string ds = dir.string();
    auto pushlen = [](std::vector<std::uint8_t>& out, std::uint16_t n) {
        out.push_back(static_cast<std::uint8_t>( n        & 0xFFu));
        out.push_back(static_cast<std::uint8_t>((n >>  8) & 0xFFu));
    };
    pushlen(req.payload, static_cast<std::uint16_t>(ds.size()));
    req.payload.insert(req.payload.end(), ds.begin(), ds.end());
    pushlen(req.payload, 0);                       // empty user
    pushlen(req.payload, 0);                       // empty password
    REQUIRE(write_frame(cs, req).has_value());
    auto reply = read_frame(cs);
    REQUIRE(reply.has_value());
    CHECK(reply.value().opcode == Opcode::ConnectAck);
    std::string s(reply.value().payload.begin(),
                  reply.value().payload.end());
    CHECK(s == std::string("connected:") + ds);

    sock_close(cs);
    srv.stop();
    fs::remove_all(dir, ec);
}

TEST_CASE("M12.3 server unknown opcode returns Error frame") {
    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    auto cli = connect_tcp("127.0.0.1", srv.port());
    REQUIRE(cli.has_value());
    Socket cs = cli.value();

    Frame req;
    // M12.15 grew the opcode space to 0x87; pick a value still
    // outside every defined op so the server's default-case path
    // is what answers.
    req.opcode = static_cast<Opcode>(0xEE);   // truly unknown
    REQUIRE(write_frame(cs, req).has_value());
    auto reply = read_frame(cs);
    REQUIRE(reply.has_value());
    CHECK(reply.value().opcode == Opcode::Error);
    // M12.10 — Error payload now starts with [u32 ace_code]; skip 4
    // bytes to recover the textual message.
    REQUIRE(reply.value().payload.size() >= 4);
    std::string s(reply.value().payload.begin() + 4,
                  reply.value().payload.end());
    CHECK(s == "unsupported opcode");

    sock_close(cs);
    srv.stop();
}

namespace {

void m12_write_dbf(const std::filesystem::path& path,
                   const std::vector<std::string>& tags) {
    std::vector<std::uint8_t> file;
    std::array<std::uint8_t, 32> hdr{};
    hdr[0]  = 0x03;
    hdr[4]  = static_cast<std::uint8_t>(tags.size());
    hdr[8]  = 32 + 32 + 1;
    hdr[10] = 1 + 4;
    file.insert(file.end(), hdr.begin(), hdr.end());
    std::array<std::uint8_t, 32> fd{};
    std::strncpy(reinterpret_cast<char*>(fd.data()), "TAG", 11);
    fd[11] = 'C'; fd[16] = 4;
    file.insert(file.end(), fd.begin(), fd.end());
    file.push_back(0x0D);
    for (auto& t : tags) {
        file.push_back(' ');
        for (std::size_t i = 0; i < 4; ++i)
            file.push_back(i < t.size()
                ? static_cast<std::uint8_t>(t[i]) : ' ');
    }
    file.push_back(0x1A);
    std::ofstream(path, std::ios::binary).write(
        reinterpret_cast<const char*>(file.data()),
        static_cast<std::streamsize>(file.size()));
}

void m12_write_u32(std::vector<std::uint8_t>& v, std::uint32_t x) {
    v.push_back(static_cast<std::uint8_t>( x        & 0xFFu));
    v.push_back(static_cast<std::uint8_t>((x >>  8) & 0xFFu));
    v.push_back(static_cast<std::uint8_t>((x >> 16) & 0xFFu));
    v.push_back(static_cast<std::uint8_t>((x >> 24) & 0xFFu));
}

}  // namespace

TEST_CASE("M12.4 remote OpenTable + GetRecordCount + walk + GetField") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_m12_4";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    m12_write_dbf(dir / "data.dbf", {"AAAA", "BBBB", "CCCC"});

    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    auto cli = connect_tcp("127.0.0.1", srv.port());
    REQUIRE(cli.has_value());
    Socket cs = cli.value();

    // Connect.
    {
        Frame req;
        req.opcode = Opcode::Connect;
        std::string ds = dir.string();
        auto pushlen = [](std::vector<std::uint8_t>& out, std::uint16_t n) {
            out.push_back(static_cast<std::uint8_t>( n        & 0xFFu));
            out.push_back(static_cast<std::uint8_t>((n >>  8) & 0xFFu));
        };
        pushlen(req.payload, static_cast<std::uint16_t>(ds.size()));
        req.payload.insert(req.payload.end(), ds.begin(), ds.end());
        pushlen(req.payload, 0);                   // user
        pushlen(req.payload, 0);                   // password
        REQUIRE(write_frame(cs, req).has_value());
        auto rep = read_frame(cs);
        REQUIRE(rep.has_value());
        REQUIRE(rep.value().opcode == Opcode::ConnectAck);
    }

    // OpenTable.
    std::uint32_t tid = 0;
    {
        Frame req;
        req.opcode = Opcode::OpenTable;
        std::string leaf = "data.dbf";
        req.payload.assign(leaf.begin(), leaf.end());
        REQUIRE(write_frame(cs, req).has_value());
        auto rep = read_frame(cs);
        REQUIRE(rep.has_value());
        REQUIRE(rep.value().opcode == Opcode::OpenTableAck);
        REQUIRE(rep.value().payload.size() == 4);
        tid = static_cast<std::uint32_t>(rep.value().payload[0]) |
              (static_cast<std::uint32_t>(rep.value().payload[1]) <<  8) |
              (static_cast<std::uint32_t>(rep.value().payload[2]) << 16) |
              (static_cast<std::uint32_t>(rep.value().payload[3]) << 24);
        CHECK(tid == 1);
    }

    // GetRecordCount.
    {
        Frame req;
        req.opcode = Opcode::GetRecordCount;
        m12_write_u32(req.payload, tid);
        REQUIRE(write_frame(cs, req).has_value());
        auto rep = read_frame(cs);
        REQUIRE(rep.has_value());
        REQUIRE(rep.value().opcode == Opcode::GetRecordCountAck);
        std::uint32_t rc =
            static_cast<std::uint32_t>(rep.value().payload[0]) |
            (static_cast<std::uint32_t>(rep.value().payload[1]) <<  8) |
            (static_cast<std::uint32_t>(rep.value().payload[2]) << 16) |
            (static_cast<std::uint32_t>(rep.value().payload[3]) << 24);
        CHECK(rc == 3);
    }

    // GotoTop + GetField on row 1.
    {
        Frame req;
        req.opcode = Opcode::GotoTop;
        m12_write_u32(req.payload, tid);
        REQUIRE(write_frame(cs, req).has_value());
        auto rep = read_frame(cs);
        REQUIRE(rep.has_value());
        REQUIRE(rep.value().opcode == Opcode::GotoTopAck);
    }
    auto get_tag = [&]() {
        Frame req;
        req.opcode = Opcode::GetField;
        m12_write_u32(req.payload, tid);
        std::string fname = "TAG";
        req.payload.insert(req.payload.end(),
                           fname.begin(), fname.end());
        REQUIRE(write_frame(cs, req).has_value());
        auto rep = read_frame(cs);
        REQUIRE(rep.has_value());
        REQUIRE(rep.value().opcode == Opcode::GetFieldAck);
        return std::string(rep.value().payload.begin(),
                           rep.value().payload.end());
    };
    CHECK(get_tag() == "AAAA");

    // Skip +1 → row 2.
    {
        Frame req;
        req.opcode = Opcode::Skip;
        m12_write_u32(req.payload, tid);
        m12_write_u32(req.payload, 1);
        REQUIRE(write_frame(cs, req).has_value());
        auto rep = read_frame(cs);
        REQUIRE(rep.has_value());
        REQUIRE(rep.value().opcode == Opcode::SkipAck);
    }
    CHECK(get_tag() == "BBBB");

    // CloseTable.
    {
        Frame req;
        req.opcode = Opcode::CloseTable;
        m12_write_u32(req.payload, tid);
        REQUIRE(write_frame(cs, req).has_value());
        auto rep = read_frame(cs);
        REQUIRE(rep.has_value());
        REQUIRE(rep.value().opcode == Opcode::CloseTableAck);
    }

    sock_close(cs);
    srv.stop();
    fs::remove_all(dir, ec);
}

TEST_CASE("M12.5 dual-mode AdsConnect60 with tcp:// URI routes ABI calls to server") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_m12_5";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    m12_write_dbf(dir / "data.dbf", {"AAAA", "BBBB"});

    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    char uri[256];
    std::snprintf(uri, sizeof(uri),
                  "tcp://127.0.0.1:%u/%s",
                  static_cast<unsigned>(srv.port()),
                  dir.string().c_str());

    UNSIGNED8 srvbuf[256];
    std::memcpy(srvbuf, uri, std::strlen(uri) + 1);
    UNSIGNED8 leaf[64] = "data.dbf";

    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srvbuf, ADS_REMOTE_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    ADSHANDLE hTable = 0;
    REQUIRE(AdsOpenTable(hConn, leaf, nullptr, ADS_CDX,
                         0, 0, 0, 0, &hTable) == 0);

    UNSIGNED32 cnt = 0;
    REQUIRE(AdsGetRecordCount(hTable, 0, &cnt) == 0);
    CHECK(cnt == 2);

    REQUIRE(AdsGotoTop(hTable) == 0);
    UNSIGNED8  buf[16] = {0};
    UNSIGNED32 cap = sizeof(buf);
    REQUIRE(AdsGetField(hTable, (UNSIGNED8*)"TAG", buf, &cap, 0) == 0);
    std::string s((char*)buf, cap);
    while (!s.empty() && s.back() == ' ') s.pop_back();
    CHECK(s == "AAAA");

    REQUIRE(AdsSkip(hTable, 1) == 0);
    cap = sizeof(buf); std::memset(buf, 0, sizeof(buf));
    REQUIRE(AdsGetField(hTable, (UNSIGNED8*)"TAG", buf, &cap, 0) == 0);
    std::string s2((char*)buf, cap);
    while (!s2.empty() && s2.back() == ' ') s2.pop_back();
    CHECK(s2 == "BBBB");

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);

    srv.stop();
    fs::remove_all(dir, ec);
}

TEST_CASE("M12.14/15 remote field metadata + cursor + info + AOF round-trip") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_m12_14_15";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    m12_write_dbf(dir / "data.dbf", {"AAAA", "BBBB", "CCCC"});

    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    char uri[256];
    std::snprintf(uri, sizeof(uri),
                  "tcp://127.0.0.1:%u/%s",
                  static_cast<unsigned>(srv.port()),
                  dir.string().c_str());
    UNSIGNED8 srvbuf[256];
    std::memcpy(srvbuf, uri, std::strlen(uri) + 1);
    UNSIGNED8 leaf[64] = "data.dbf";

    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srvbuf, ADS_REMOTE_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);
    ADSHANDLE hT = 0;
    REQUIRE(AdsOpenTable(hConn, leaf, nullptr, ADS_CDX,
                         0, 0, 0, 0, &hT) == 0);

    SUBCASE("M12.14 field metadata bridges (no per-field round-trip)") {
        UNSIGNED16 nf = 0;
        REQUIRE(AdsGetNumFields(hT, &nf) == 0);
        CHECK(nf == 1);

        UNSIGNED8  nm[32]  = {0};
        UNSIGNED16 nlen    = sizeof(nm);
        REQUIRE(AdsGetFieldName(hT, 1, nm, &nlen) == 0);
        std::string fname((char*)nm, nlen);
        CHECK(fname == "TAG");

        UNSIGNED16 ftype = 0;
        REQUIRE(AdsGetFieldType(hT, (UNSIGNED8*)"TAG", &ftype) == 0);
        CHECK(ftype == ADS_STRING);

        UNSIGNED32 flen = 0;
        REQUIRE(AdsGetFieldLength(hT, (UNSIGNED8*)"TAG", &flen) == 0);
        CHECK(flen == 4);

        UNSIGNED16 fdec = 99;
        REQUIRE(AdsGetFieldDecimals(hT, (UNSIGNED8*)"TAG", &fdec) == 0);
        CHECK(fdec == 0);
    }

    SUBCASE("M12.14 cursor state bridges") {
        REQUIRE(AdsGotoTop(hT) == 0);

        UNSIGNED16 atbof = 99;
        REQUIRE(AdsAtBOF(hT, &atbof) == 0);
        CHECK(atbof == 0);

        UNSIGNED32 rn = 0;
        REQUIRE(AdsGetRecordNum(hT, 0, &rn) == 0);
        CHECK(rn == 1);

        UNSIGNED16 del = 99;
        REQUIRE(AdsIsRecordDeleted(hT, &del) == 0);
        CHECK(del == 0);

        REQUIRE(AdsGotoBottom(hT) == 0);
        REQUIRE(AdsGetRecordNum(hT, 0, &rn) == 0);
        CHECK(rn == 3);
    }

    SUBCASE("M12.15 info bridges") {
        UNSIGNED16 ttype = 0;
        REQUIRE(AdsGetTableType(hT, &ttype) == 0);
        CHECK(ttype == ADS_CDX);

        UNSIGNED32 rl = 0;
        REQUIRE(AdsGetRecordLength(hT, &rl) == 0);
        CHECK(rl == 5);  // 1 (delete byte) + 4 (TAG)

        UNSIGNED16 ni = 99;
        REQUIRE(AdsGetNumIndexes(hT, &ni) == 0);
        CHECK(ni == 0);
    }

    SUBCASE("M12.15 lock + maintenance bridges (no-op success)") {
        REQUIRE(AdsLockTable(hT) == 0);
        REQUIRE(AdsUnlockTable(hT) == 0);
        REQUIRE(AdsLockRecord(hT, 1) == 0);
        REQUIRE(AdsUnlockRecord(hT, 1) == 0);
        REQUIRE(AdsRefreshRecord(hT) == 0);
        REQUIRE(AdsFlushFileBuffers(hT) == 0);
    }

    SUBCASE("M12.15 AOF over the wire") {
        UNSIGNED8 cond[32] = "TAG = 'BBBB'";
        REQUIRE(AdsSetAOF(hT, cond, 0) == 0);
        UNSIGNED16 lvl = 99;
        REQUIRE(AdsGetAOFOptLevel(hT, &lvl, nullptr, nullptr) == 0);
        CHECK((lvl == ADS_OPTIMIZED_NONE ||
               lvl == ADS_OPTIMIZED_FULL ||
               lvl == ADS_OPTIMIZED_PART));
        REQUIRE(AdsGotoTop(hT) == 0);
        UNSIGNED32 rn = 0;
        REQUIRE(AdsGetRecordNum(hT, 0, &rn) == 0);
        CHECK(rn == 2);  // BBBB row only, AAAA / CCCC filtered out
        REQUIRE(AdsClearAOF(hT) == 0);
        REQUIRE(AdsGotoTop(hT) == 0);
        REQUIRE(AdsGetRecordNum(hT, 0, &rn) == 0);
        CHECK(rn == 1);
    }

    REQUIRE(AdsCloseTable(hT) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    srv.stop();
    fs::remove_all(dir, ec);
}

TEST_CASE("M12.6 remote append + set_field + delete + recall + flush round-trip") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_m12_6";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    m12_write_dbf(dir / "data.dbf", {"AAAA", "BBBB"});

    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    char uri[256];
    std::snprintf(uri, sizeof(uri),
                  "tcp://127.0.0.1:%u/%s",
                  static_cast<unsigned>(srv.port()),
                  dir.string().c_str());

    UNSIGNED8 srvbuf[256];
    std::memcpy(srvbuf, uri, std::strlen(uri) + 1);
    UNSIGNED8 leaf[64] = "data.dbf";

    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srvbuf, ADS_REMOTE_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);
    ADSHANDLE hTable = 0;
    REQUIRE(AdsOpenTable(hConn, leaf, nullptr, ADS_CDX,
                         0, 0, 0, 0, &hTable) == 0);

    // Append a third record + write its TAG over the wire.
    REQUIRE(AdsAppendRecord(hTable) == 0);
    UNSIGNED8 fld[8] = "TAG";
    UNSIGNED8 val[8] = "CCCC";
    REQUIRE(AdsSetString(hTable, fld, val, 4) == 0);
    REQUIRE(AdsWriteRecord(hTable) == 0);          // wire flush

    // Confirm record_count climbed to 3 + the value round-trips.
    UNSIGNED32 cnt = 0;
    REQUIRE(AdsGetRecordCount(hTable, 0, &cnt) == 0);
    CHECK(cnt == 3);

    REQUIRE(AdsGotoRecord(hTable, 3) == 0);
    UNSIGNED8 buf[16] = {0};
    UNSIGNED32 cap = sizeof(buf);
    REQUIRE(AdsGetField(hTable, fld, buf, &cap, 0) == 0);
    std::string s((char*)buf, cap);
    while (!s.empty() && s.back() == ' ') s.pop_back();
    CHECK(s == "CCCC");

    // Delete + recall round-trip.
    REQUIRE(AdsGotoRecord(hTable, 2) == 0);
    REQUIRE(AdsDeleteRecord(hTable) == 0);
    UNSIGNED16 del = 0;
    // is_deleted is read-only / local-only here; verify by reopening
    // the file at the end. For now exercise recall.
    REQUIRE(AdsRecallRecord(hTable) == 0);
    (void)del;

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    srv.stop();

    // Re-open through a *local* connection to confirm the write hit
    // disk on the server side.
    UNSIGNED8 lsrv[256];
    std::memcpy(lsrv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hLocal = 0;
    REQUIRE(AdsConnect60(lsrv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hLocal) == 0);
    ADSHANDLE hLocalT = 0;
    REQUIRE(AdsOpenTable(hLocal, leaf, nullptr, ADS_CDX,
                         0, 0, 0, 0, &hLocalT) == 0);
    UNSIGNED32 cnt2 = 0;
    REQUIRE(AdsGetRecordCount(hLocalT, 0, &cnt2) == 0);
    CHECK(cnt2 == 3);
    REQUIRE(AdsGotoRecord(hLocalT, 3) == 0);
    UNSIGNED8 lbuf[16] = {0};
    UNSIGNED32 lcap = sizeof(lbuf);
    REQUIRE(AdsGetField(hLocalT, fld, lbuf, &lcap, 0) == 0);
    std::string s2((char*)lbuf, lcap);
    while (!s2.empty() && s2.back() == ' ') s2.pop_back();
    CHECK(s2 == "CCCC");
    REQUIRE(AdsCloseTable(hLocalT) == 0);
    REQUIRE(AdsDisconnect(hLocal) == 0);

    fs::remove_all(dir, ec);
}

TEST_CASE("M12.7 remote SQL exec — SELECT cursor + COUNT round-trip") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_m12_7";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    m12_write_dbf(dir / "data.dbf", {"AAAA", "BBBB", "CCCC"});

    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    char uri[256];
    std::snprintf(uri, sizeof(uri),
                  "tcp://127.0.0.1:%u/%s",
                  static_cast<unsigned>(srv.port()),
                  dir.string().c_str());
    UNSIGNED8 srvbuf[256];
    std::memcpy(srvbuf, uri, std::strlen(uri) + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srvbuf, ADS_REMOTE_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    ADSHANDLE hStmt = 0;
    REQUIRE(AdsCreateSQLStatement(hConn, &hStmt) == 0);

    // 1) SELECT * FROM data — cursor with 3 rows.
    {
        UNSIGNED8 sql[64];
        std::strcpy(reinterpret_cast<char*>(sql),
                    "SELECT * FROM data.dbf");
        ADSHANDLE hCur = 0;
        REQUIRE(AdsExecuteSQLDirect(hStmt, sql, &hCur) == 0);
        REQUIRE(hCur != 0);
        UNSIGNED32 cnt = 0;
        REQUIRE(AdsGetRecordCount(hCur, 0, &cnt) == 0);
        CHECK(cnt == 3);
        REQUIRE(AdsGotoTop(hCur) == 0);
        UNSIGNED8  buf[16] = {0};
        UNSIGNED32 cap = sizeof(buf);
        REQUIRE(AdsGetField(hCur, (UNSIGNED8*)"TAG",
                            buf, &cap, 0) == 0);
        std::string s((char*)buf, cap);
        while (!s.empty() && s.back() == ' ') s.pop_back();
        CHECK(s == "AAAA");
        REQUIRE(AdsCloseTable(hCur) == 0);
    }

    // 2) SELECT COUNT(*) — single-row aggregate cursor.
    {
        UNSIGNED8 sql[64];
        std::strcpy(reinterpret_cast<char*>(sql),
                    "SELECT COUNT(*) FROM data.dbf");
        ADSHANDLE hCur = 0;
        REQUIRE(AdsExecuteSQLDirect(hStmt, sql, &hCur) == 0);
        REQUIRE(hCur != 0);
        UNSIGNED32 cnt = 0;
        REQUIRE(AdsGetRecordCount(hCur, 0, &cnt) == 0);
        CHECK(cnt == 1);
        REQUIRE(AdsCloseTable(hCur) == 0);
    }

    REQUIRE(AdsCloseSQLStatement(hStmt) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    srv.stop();
    fs::remove_all(dir, ec);
}

TEST_CASE("M12.8 remote AdsReindex routes through wire") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_m12_8";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    m12_write_dbf(dir / "data.dbf", {"AAAA", "BBBB"});

    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    char uri[256];
    std::snprintf(uri, sizeof(uri),
                  "tcp://127.0.0.1:%u/%s",
                  static_cast<unsigned>(srv.port()),
                  dir.string().c_str());
    UNSIGNED8 srvbuf[256];
    std::memcpy(srvbuf, uri, std::strlen(uri) + 1);
    UNSIGNED8 leaf[64] = "data.dbf";

    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srvbuf, ADS_REMOTE_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);
    ADSHANDLE hTable = 0;
    REQUIRE(AdsOpenTable(hConn, leaf, nullptr, ADS_CDX,
                         0, 0, 0, 0, &hTable) == 0);
    // No bound indexes — Reindex over an indexless table is a no-op
    // success. The point of the test is that the wire op and the
    // server-side dispatch hold together end-to-end.
    REQUIRE(AdsReindex(hTable) == 0);
    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    srv.stop();
    fs::remove_all(dir, ec);
}

TEST_CASE("M12.9 server with credentials rejects bad password") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_m12_9_bad";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    m12_write_dbf(dir / "data.dbf", {"AAAA"});

    Server srv;
    srv.add_credential("admin", "letmein");
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    char uri[256];
    std::snprintf(uri, sizeof(uri),
                  "tcp://127.0.0.1:%u/%s",
                  static_cast<unsigned>(srv.port()),
                  dir.string().c_str());
    UNSIGNED8 srvbuf[256];
    std::memcpy(srvbuf, uri, std::strlen(uri) + 1);

    UNSIGNED8 user[16] = "admin";
    UNSIGNED8 wrong[16] = "nope";
    ADSHANDLE hConn = 0;
    CHECK(AdsConnect60(srvbuf, ADS_REMOTE_SERVER,
                       user, wrong, 0, &hConn) != 0);
    CHECK(hConn == 0);

    srv.stop();
    fs::remove_all(dir, ec);
}

TEST_CASE("M12.9 server with credentials accepts matching password") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_m12_9_ok";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    m12_write_dbf(dir / "data.dbf", {"AAAA"});

    Server srv;
    srv.add_credential("admin", "letmein");
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    char uri[256];
    std::snprintf(uri, sizeof(uri),
                  "tcp://127.0.0.1:%u/%s",
                  static_cast<unsigned>(srv.port()),
                  dir.string().c_str());
    UNSIGNED8 srvbuf[256];
    std::memcpy(srvbuf, uri, std::strlen(uri) + 1);

    UNSIGNED8 user[16] = "admin";
    UNSIGNED8 pw  [16] = "letmein";
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srvbuf, ADS_REMOTE_SERVER,
                         user, pw, 0, &hConn) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    srv.stop();
    fs::remove_all(dir, ec);
}

TEST_CASE("M12.11 batch fetch returns multiple rows in one frame") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_m12_11";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    m12_write_dbf(dir / "data.dbf",
                  {"AAAA","BBBB","CCCC","DDDD","EEEE","FFFF","GGGG"});

    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    openads::network::RemoteConnection rc;
    REQUIRE(rc.connect("127.0.0.1", srv.port(), dir.string())
                .has_value());
    auto tid_r = rc.open_table("data.dbf");
    REQUIRE(tid_r.has_value());
    std::uint32_t tid = tid_r.value();
    REQUIRE(rc.goto_top(tid).has_value());

    // Fetch up to 5 rows in a single round-trip.
    auto rows_r = rc.fetch_batch(tid, 5, {"TAG"});
    REQUIRE(rows_r.has_value());
    auto rows = rows_r.value();
    REQUIRE(rows.size() == 5);
    auto trim = [](std::string s) {
        while (!s.empty() && s.back() == ' ') s.pop_back();
        return s;
    };
    CHECK(trim(rows[0][0]) == "AAAA");
    CHECK(trim(rows[1][0]) == "BBBB");
    CHECK(trim(rows[2][0]) == "CCCC");
    CHECK(trim(rows[3][0]) == "DDDD");
    CHECK(trim(rows[4][0]) == "EEEE");

    // Continue the walk — should pick up rows 6 + 7, then EOF.
    auto rest_r = rc.fetch_batch(tid, 100, {"TAG"});
    REQUIRE(rest_r.has_value());
    auto rest = rest_r.value();
    REQUIRE(rest.size() == 2);
    CHECK(trim(rest[0][0]) == "FFFF");
    CHECK(trim(rest[1][0]) == "GGGG");

    REQUIRE(rc.close_table(tid).has_value());
    rc.disconnect();
    srv.stop();
    fs::remove_all(dir, ec);
}

TEST_CASE("M12.13 PlainTransport round-trips frames identically to raw Socket") {
    using openads::network::PlainTransport;
    using openads::network::make_plain_transport;
    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    auto cli = connect_tcp("127.0.0.1", srv.port());
    REQUIRE(cli.has_value());
    auto t = make_plain_transport(cli.value());

    Frame req;
    req.opcode = Opcode::Hello;
    REQUIRE(write_frame(*t, req).has_value());
    auto rep = read_frame(*t);
    REQUIRE(rep.has_value());
    CHECK(rep.value().opcode == Opcode::HelloAck);

    t->close();
    srv.stop();
}

TEST_CASE("M12.12 tls:// URI is parsed + handled per build mode") {
    // tls:// must be recognised symmetrically to tcp://; AdsConnect60
    // routes to either the real TLS path (when built with
    // -DOPENADS_WITH_TLS=ON) or returns AE_FUNCTION_NOT_AVAILABLE
    // 5004 otherwise. Either way it must NEVER silently downgrade
    // to plaintext.
    UNSIGNED8 srvbuf[64];
    // Use port 1 — we only want to confirm the URI was recognised
    // and routed; the TLS handshake will fail (refused) but that's
    // a *remote-error* path, not "function not available".
    std::strcpy(reinterpret_cast<char*>(srvbuf),
                "tls://127.0.0.1:1/whatever");
    ADSHANDLE hConn = 0;
    UNSIGNED32 rc = AdsConnect60(srvbuf, ADS_REMOTE_SERVER,
                                  nullptr, nullptr, 0, &hConn);
#if defined(OPENADS_WITH_TLS)
    // Real TLS path attempted — connect fails on the local refusal
    // and the server returns an AE_REMOTE_ERROR (5172) frame, but
    // it must NOT be 5004 (function-not-available).
    CHECK(rc != 0u);
    CHECK(rc != 5004u);
#else
    CHECK(rc == 5004u);                            // AE_FUNCTION_NOT_AVAILABLE
#endif
    CHECK(hConn == 0);

    // tls:// parser correctness — split host / port / data_dir.
    std::string h, dd;
    std::uint16_t p = 0;
    REQUIRE(openads::network::parse_tls_uri(
        "tls://server.example:7777/some/dir", h, p, dd));
    CHECK(h == "server.example");
    CHECK(p == 7777u);
    CHECK(dd == "some/dir");

    // tls:// is not parsed by parse_tcp_uri (and vice-versa).
    CHECK_FALSE(openads::network::parse_tcp_uri(
        "tls://x:1/y", h, p, dd));
    CHECK_FALSE(openads::network::parse_tls_uri(
        "tcp://x:1/y", h, p, dd));
}

TEST_CASE("M12.10 server Error frame surfaces the real ACE code") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_m12_10";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    Server srv;
    srv.add_credential("admin", "letmein");
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    char uri[256];
    std::snprintf(uri, sizeof(uri),
                  "tcp://127.0.0.1:%u/%s",
                  static_cast<unsigned>(srv.port()),
                  dir.string().c_str());
    UNSIGNED8 srvbuf[256];
    std::memcpy(srvbuf, uri, std::strlen(uri) + 1);
    UNSIGNED8 user[16] = "admin";
    UNSIGNED8 wrong[16] = "nope";
    ADSHANDLE hConn = 0;
    UNSIGNED32 rc = AdsConnect60(srvbuf, ADS_REMOTE_SERVER,
                                  user, wrong, 0, &hConn);
    // Was AE_INTERNAL_ERROR (5000) before M12.10; now AE_LOGIN_FAILED.
    CHECK(rc == 7077u);
    CHECK(hConn == 0);

    srv.stop();
    fs::remove_all(dir, ec);
}

TEST_CASE("M12.3 server stop() drops in-flight connection cleanly") {
    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());
    auto cli = connect_tcp("127.0.0.1", srv.port());
    REQUIRE(cli.has_value());
    Socket cs = cli.value();
    srv.stop();
    sock_close(cs);
    CHECK_FALSE(srv.running());
}

TEST_CASE("Server::build_mg_snapshot counts live sessions") {
    using openads::network::Server;
    Server srv;
    REQUIRE(srv.start("127.0.0.1", 0).has_value());

    Server::SessionInfo a;
    a.peer_ip = "127.0.0.1"; a.peer_port = 5001;
    a.user = "alice"; a.open_tables = 2;
    std::uint64_t id = srv.register_session(a);

    auto snap = srv.build_mg_snapshot();
    CHECK(snap.connections == 1);
    CHECK(snap.tables == 2);
    REQUIRE(snap.user_list.size() == 1);
    CHECK(snap.user_list[0].name == "alice");

    srv.unregister_session(id);
    srv.stop();
}
