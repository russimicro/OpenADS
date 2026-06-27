// openads_remote_stress — concurrent multi-client load harness for the
// TCP wire server.
//
// Spins up an in-process Server on an ephemeral port over a seeded data
// directory, then launches C concurrent client threads. Each client opens
// its own RemoteConnection, opens the table, and repeatedly scans it
// (GoTop + Skip to EOF), verifying the record count on every pass. A
// sampler thread polls the server's live session count so we can see the
// thread-per-connection growth under load.
//
// This is a measurement baseline: it does NOT change the engine or server.
// It establishes throughput / latency / peak-session numbers so later
// server-hardening work (session reaping, connection limits, pooling) can
// be measured against it and proven not to regress.
//
// Usage:
//   openads_remote_stress [--clients C] [--seconds S] [--rows N]
//                         [--dir D] [--csv]
//   defaults: --clients 32 --seconds 5 --rows 2000, temp dir, human output

#include "openads/ace.h"
#include "network/server.h"
#include "network/client.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <mutex>
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

// Seed a DBF table with `rows` records via the local ABI, so the wire
// server has something to serve. ID is the 1-based record index.
void seed_table(const std::string& dir, const std::string& tname,
                int rows) {
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
    std::memcpy(nm, tname.c_str(), std::min(tname.size(), sizeof(nm) - 1));
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
        double v = static_cast<double>(i);
        AdsSetDouble(hTab, fld_id, v);
        UNSIGNED8 pad[] = "the quick brown fox jumps over the lazydog";
        AdsSetString(hTab, fld_pad, pad,
                     static_cast<UNSIGNED32>(std::strlen(
                         reinterpret_cast<const char*>(pad))));
    }
    AdsCloseTable(hTab);
    AdsDisconnect(hConn);
}

struct Stats {
    std::atomic<std::uint64_t> scans{0};      // completed full scans
    std::atomic<std::uint64_t> rows_read{0};   // records skipped over
    std::atomic<std::uint64_t> errors{0};      // any wire/op failure
    std::atomic<std::uint64_t> miscount{0};    // scan saw != expected rows
};

// Per-thread latency samples (ms per full scan), merged at the end.
struct ThreadResult {
    std::vector<double> scan_ms;
};

double percentile(std::vector<double>& v, double p) {
    if (v.empty()) return 0.0;
    std::sort(v.begin(), v.end());
    std::size_t idx = static_cast<std::size_t>(
        p / 100.0 * static_cast<double>(v.size() - 1) + 0.5);
    if (idx >= v.size()) idx = v.size() - 1;
    return v[idx];
}

}  // namespace

int main(int argc, char** argv) {
    std::setbuf(stderr, nullptr);

    int clients = 32;
    int seconds = 5;
    int rows = 2000;
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
        else if (a == "--dir") dir = next();
        else if (a == "--csv") csv = true;
    }
    if (clients < 1) clients = 1;
    if (seconds < 1) seconds = 1;
    if (rows < 1) rows = 1;

    if (dir.empty()) {
        dir = (fs::temp_directory_path() / "openads_remote_stress").string();
    }
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
    std::fprintf(stderr,
                 "server up on 127.0.0.1:%u | %d rows | %d clients | %ds\n",
                 port, rows, clients, seconds);

    Stats stats;
    std::atomic<bool> stop{false};
    std::atomic<std::size_t> peak_sessions{0};

    // Sampler: watch the live session count (== live session threads).
    std::thread sampler([&]() {
        while (!stop.load()) {
            std::size_t n = srv.sessions_snapshot().size();
            std::size_t cur = peak_sessions.load();
            while (n > cur && !peak_sessions.compare_exchange_weak(cur, n)) {}
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    });

    std::vector<ThreadResult> results(static_cast<std::size_t>(clients));
    std::vector<std::thread> workers;
    workers.reserve(static_cast<std::size_t>(clients));

    const double t_end = now_ms() + seconds * 1000.0;

    for (int c = 0; c < clients; ++c) {
        workers.emplace_back([&, c]() {
            RemoteConnection rc;
            if (!rc.connect("127.0.0.1", port, dir).has_value()) {
                stats.errors.fetch_add(1);
                return;
            }
            auto idr = rc.open_table(tname);
            if (!idr.has_value()) {
                stats.errors.fetch_add(1);
                return;
            }
            const std::uint32_t id = idr.value();
            auto& tr = results[static_cast<std::size_t>(c)];

            while (now_ms() < t_end) {
                double t0 = now_ms();
                if (!rc.goto_top(id).has_value()) {
                    stats.errors.fetch_add(1);
                    break;
                }
                std::uint64_t seen = 0;
                for (;;) {
                    auto eof = rc.at_eof(id);
                    if (!eof.has_value()) {
                        stats.errors.fetch_add(1);
                        break;
                    }
                    if (eof.value()) break;
                    ++seen;
                    if (!rc.skip(id, 1).has_value()) {
                        stats.errors.fetch_add(1);
                        break;
                    }
                }
                tr.scan_ms.push_back(now_ms() - t0);
                stats.scans.fetch_add(1);
                stats.rows_read.fetch_add(seen);
                if (seen != static_cast<std::uint64_t>(rows))
                    stats.miscount.fetch_add(1);
            }
            rc.close_table(id);
            rc.disconnect();
        });
    }

    for (auto& w : workers) w.join();
    stop.store(true);
    sampler.join();

    // Merge latency samples.
    std::vector<double> all;
    for (auto& r : results)
        all.insert(all.end(), r.scan_ms.begin(), r.scan_ms.end());

    const std::uint64_t scans = stats.scans.load();
    const double secs = static_cast<double>(seconds);
    const double scans_per_sec = static_cast<double>(scans) / secs;
    const double rows_per_sec =
        static_cast<double>(stats.rows_read.load()) / secs;
    const double p50 = percentile(all, 50);
    const double p95 = percentile(all, 95);
    const double p99 = percentile(all, 99);

    if (csv) {
        std::printf(
            "clients,seconds,rows,scans,scans_per_sec,rows_per_sec,"
            "p50_ms,p95_ms,p99_ms,errors,miscount,peak_sessions\n");
        std::printf("%d,%d,%d,%llu,%.1f,%.0f,%.3f,%.3f,%.3f,%llu,%llu,%zu\n",
                    clients, seconds, rows,
                    static_cast<unsigned long long>(scans), scans_per_sec,
                    rows_per_sec, p50, p95, p99,
                    static_cast<unsigned long long>(stats.errors.load()),
                    static_cast<unsigned long long>(stats.miscount.load()),
                    peak_sessions.load());
    } else {
        std::printf("\n=== openads_remote_stress baseline ===\n");
        std::printf("clients          : %d\n", clients);
        std::printf("duration         : %d s\n", seconds);
        std::printf("rows/table       : %d\n", rows);
        std::printf("full scans       : %llu (%.1f/s)\n",
                    static_cast<unsigned long long>(scans), scans_per_sec);
        std::printf("rows read        : %llu (%.0f/s)\n",
                    static_cast<unsigned long long>(stats.rows_read.load()),
                    rows_per_sec);
        std::printf("scan latency ms  : p50=%.2f  p95=%.2f  p99=%.2f\n",
                    p50, p95, p99);
        std::printf("peak sessions    : %zu (== live server threads)\n",
                    peak_sessions.load());
        std::printf("errors           : %llu\n",
                    static_cast<unsigned long long>(stats.errors.load()));
        std::printf("scan miscount    : %llu (should be 0)\n",
                    static_cast<unsigned long long>(stats.miscount.load()));
    }

    return (stats.errors.load() == 0 && stats.miscount.load() == 0) ? 0 : 2;
}
