// openads_remote_random — randomized soak harness for the TCP wire server.
//
// Like remote_concurrency, but each client thread runs a SEEDED-random mix of
// ADS-flavoured operations instead of a plain scan loop, so the server's
// per-connection state machine gets exercised under concurrency + churn:
//
//   SCAN          GoTop + Skip-to-EOF, verify record count
//   SELECT *      server-side SQL cursor (abi_conn/abi_stmt), verify count
//   SELECT COUNT  single-row aggregate cursor
//   AOF           server-side optimized filter set/clear (ADS signature)
//   BATCH         fetch_batch walk to EOF (prefetch path), verify total
//   RECONNECT     disconnect + reconnect + reopen (pool add/remove churn)
//
// Run it with OPENADS_SERVER_POOL=0 then =1: correctness must be identical
// (errors=0, miscount=0). Exit code 0 iff clean.
//
// Usage: openads_remote_random [--clients C] [--seconds S] [--rows N]
//                              [--seed K] [--dir D] [--csv]

#include "openads/ace.h"
#include "network/server.h"
#include "network/client.h"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <random>
#include <string>
#include <thread>
#include <vector>

namespace fs = std::filesystem;
using openads::network::RemoteConnection;
using openads::network::Server;

namespace {

double now_ms() {
    using clock = std::chrono::steady_clock;
    return std::chrono::duration<double, std::milli>(
               clock::now().time_since_epoch())
        .count();
}

void seed_table(const std::string& dir, const std::string& tname, int rows) {
    std::vector<UNSIGNED8> srv(dir.size() + 1);
    std::memcpy(srv.data(), dir.c_str(), dir.size() + 1);
    ADSHANDLE hConn = 0;
    if (AdsConnect60(srv.data(), ADS_LOCAL_SERVER, nullptr, nullptr, 0,
                     &hConn) != 0) {
        std::fprintf(stderr, "seed: AdsConnect60 failed\n");
        std::exit(1);
    }
    UNSIGNED8 def[] = "ID,N,10,0;PADDING,C,40,0";
    UNSIGNED8 nm[64] = {};
    std::memcpy(nm, tname.c_str(),
                std::min(tname.size(), sizeof(nm) - 1));
    ADSHANDLE hTab = 0;
    if (AdsCreateTable(hConn, nm, nullptr, ADS_CDX, ADS_ANSI, 0, 0, 0, def,
                       &hTab) != 0) {
        std::fprintf(stderr, "seed: AdsCreateTable failed\n");
        std::exit(1);
    }
    UNSIGNED8 fld_id[] = "ID";
    UNSIGNED8 fld_pad[] = "PADDING";
    for (int i = 1; i <= rows; ++i) {
        AdsAppendRecord(hTab);
        AdsSetDouble(hTab, fld_id, static_cast<double>(i));
        UNSIGNED8 pad[] = "the quick brown fox jumps over the lazydog";
        AdsSetString(hTab, fld_pad, pad,
                     static_cast<UNSIGNED32>(std::strlen(
                         reinterpret_cast<const char*>(pad))));
    }
    AdsCloseTable(hTab);
    AdsDisconnect(hConn);
}

struct Stats {
    std::atomic<std::uint64_t> ops{0};
    std::atomic<std::uint64_t> errors{0};
    std::atomic<std::uint64_t> miscount{0};
    std::atomic<std::uint64_t> reconnects{0};
};

}  // namespace

int main(int argc, char** argv) {
    std::setbuf(stderr, nullptr);

    int clients = 24;
    int seconds = 5;
    int rows = 1000;
    unsigned base_seed = 1234;
    bool csv = false;
    std::string dir;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        auto next = [&]() -> const char* {
            return (i + 1 < argc) ? argv[++i] : "";
        };
        if (a == "--clients") clients = std::atoi(next());
        else if (a == "--seconds") seconds = std::atoi(next());
        else if (a == "--rows") rows = std::atoi(next());
        else if (a == "--seed") base_seed =
            static_cast<unsigned>(std::strtoul(next(), nullptr, 10));
        else if (a == "--dir") dir = next();
        else if (a == "--csv") csv = true;
    }
    if (clients < 1) clients = 1;
    if (seconds < 1) seconds = 1;
    if (rows < 1) rows = 1;

    if (dir.empty())
        dir = (fs::temp_directory_path() / "openads_remote_random").string();
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir, ec);

    const std::string tname = "load.dbf";
    seed_table(dir, tname, rows);

    Server srv;
    if (!srv.start("127.0.0.1", 0).has_value()) {
        std::fprintf(stderr, "FATAL: server start failed\n");
        return 1;
    }
    const std::uint16_t port = srv.port();
    const bool pool = std::getenv("OPENADS_SERVER_POOL") != nullptr &&
                      std::strcmp(std::getenv("OPENADS_SERVER_POOL"), "0") != 0;
    std::fprintf(stderr,
                 "server up :%u | pool=%d | %d rows | %d clients | %ds | seed %u\n",
                 port, pool ? 1 : 0, rows, clients, seconds, base_seed);

    Stats stats;
    std::atomic<bool> stop{false};
    std::atomic<std::size_t> peak_sessions{0};

    std::thread sampler([&]() {
        while (!stop.load()) {
            std::size_t n = srv.sessions_snapshot().size();
            std::size_t cur = peak_sessions.load();
            while (n > cur && !peak_sessions.compare_exchange_weak(cur, n)) {}
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    });

    const double t_end = now_ms() + seconds * 1000.0;
    std::vector<std::thread> workers;
    workers.reserve(static_cast<std::size_t>(clients));

    for (int c = 0; c < clients; ++c) {
        workers.emplace_back([&, c]() {
            std::mt19937 rng(base_seed + static_cast<unsigned>(c) * 7919u);
            auto fail = [&]() { stats.errors.fetch_add(1); };

            RemoteConnection rc;
            if (!rc.connect("127.0.0.1", port, dir).has_value()) { fail(); return; }
            auto idr = rc.open_table(tname);
            if (!idr.has_value()) { fail(); return; }
            std::uint32_t id = idr.value();

            while (now_ms() < t_end) {
                int roll = static_cast<int>(rng() % 100);
                stats.ops.fetch_add(1);

                if (roll < 30) {                       // SCAN
                    if (!rc.goto_top(id).has_value()) { fail(); continue; }
                    std::uint64_t seen = 0;
                    bool ok = true;
                    for (;;) {
                        auto eof = rc.at_eof(id);
                        if (!eof.has_value()) { fail(); ok = false; break; }
                        if (eof.value()) break;
                        ++seen;
                        if (!rc.skip(id, 1).has_value()) { fail(); ok = false; break; }
                    }
                    if (ok && seen != static_cast<std::uint64_t>(rows))
                        stats.miscount.fetch_add(1);

                } else if (roll < 50) {                // SELECT *
                    auto cur = rc.execute_sql("SELECT * FROM load.dbf");
                    if (!cur.has_value()) { fail(); continue; }
                    std::uint32_t cid = cur.value();
                    auto rcnt = rc.record_count(cid);
                    if (!rcnt.has_value()) { fail(); }
                    else if (rcnt.value() != static_cast<std::uint32_t>(rows))
                        stats.miscount.fetch_add(1);
                    rc.close_table(cid);

                } else if (roll < 62) {                // SELECT COUNT(*)
                    auto cur = rc.execute_sql("SELECT COUNT(*) FROM load.dbf");
                    if (!cur.has_value()) { fail(); continue; }
                    std::uint32_t cid = cur.value();
                    auto rcnt = rc.record_count(cid);
                    if (!rcnt.has_value()) { fail(); }
                    else if (rcnt.value() != 1u) stats.miscount.fetch_add(1);
                    rc.close_table(cid);

                } else if (roll < 75) {                // AOF set/clear
                    std::uint32_t k = rng() % static_cast<unsigned>(rows);
                    std::string cond = "ID > " + std::to_string(k);
                    if (!rc.set_aof(id, cond).has_value()) { fail(); continue; }
                    // The optimizable subset varies; we only require that the
                    // path executes and a top/scan stays responsive + clears.
                    if (!rc.goto_top(id).has_value()) { fail(); }
                    if (!rc.clear_aof(id).has_value()) { fail(); }

                } else if (roll < 90) {                // BATCH fetch to EOF
                    if (!rc.goto_top(id).has_value()) { fail(); continue; }
                    std::uint64_t total = 0;
                    bool ok = true;
                    for (;;) {
                        auto b = rc.fetch_batch(id, 64, {"ID"});
                        if (!b.has_value()) { fail(); ok = false; break; }
                        if (b.value().empty()) break;
                        total += b.value().size();
                        if (b.value().size() < 64) break;
                    }
                    if (ok && total != static_cast<std::uint64_t>(rows))
                        stats.miscount.fetch_add(1);

                } else {                               // RECONNECT (churn)
                    rc.close_table(id);
                    rc.disconnect();
                    stats.reconnects.fetch_add(1);
                    if (!rc.connect("127.0.0.1", port, dir).has_value()) { fail(); return; }
                    auto r2 = rc.open_table(tname);
                    if (!r2.has_value()) { fail(); return; }
                    id = r2.value();
                }
            }
            rc.close_table(id);
            rc.disconnect();
        });
    }

    for (auto& w : workers) w.join();
    stop.store(true);
    sampler.join();

    const std::uint64_t ops = stats.ops.load();
    const std::uint64_t errs = stats.errors.load();
    const std::uint64_t mis = stats.miscount.load();
    const std::uint64_t rec = stats.reconnects.load();

    if (csv) {
        std::printf("pool,clients,seconds,rows,ops,reconnects,errors,miscount,peak_sessions\n");
        std::printf("%d,%d,%d,%d,%llu,%llu,%llu,%llu,%zu\n",
                    pool ? 1 : 0, clients, seconds, rows,
                    (unsigned long long)ops, (unsigned long long)rec,
                    (unsigned long long)errs, (unsigned long long)mis,
                    peak_sessions.load());
    } else {
        std::printf("\n=== openads_remote_random (pool=%d) ===\n", pool ? 1 : 0);
        std::printf("clients        : %d\n", clients);
        std::printf("ops            : %llu\n", (unsigned long long)ops);
        std::printf("reconnects     : %llu\n", (unsigned long long)rec);
        std::printf("peak sessions  : %zu\n", peak_sessions.load());
        std::printf("errors         : %llu  (must be 0)\n", (unsigned long long)errs);
        std::printf("miscount       : %llu  (must be 0)\n", (unsigned long long)mis);
    }

    srv.stop();
    fs::remove_all(dir, ec);
    return (errs == 0 && mis == 0) ? 0 : 2;
}
