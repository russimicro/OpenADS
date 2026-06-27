// openads_daily_ops — local (in-process) "real business day" soak harness.
//
// Pure read/write loops do not resemble a real application's day. A line-of-
// business app interleaves, under concurrency:
//
//   SCAN        full walk of an order (every tag must reach EOF seeing all rows)
//   SEEK        keyed lookup (must find a key known to exist)
//   FILTER/AOF  Advantage Optimized Filter set/clear (must only see matches)
//   ORDER SWAP  switch the active index order mid-session
//   SCOPE       range-limit an order, walk it (must not leak out-of-range rows)
//   TEMP INDEX  build an ad-hoc index for a report, use it, discard it
//   REINDEX     rebuild a table's indexes
//   APPEND      add rows
//
// Each worker thread owns its own ABI connection. The seeded CUSTOMERS table is
// shared read-only across all threads (scan / seek / filter / order-swap / scope
// are read paths and need no cross-thread coordination). The mutating patterns
// (temp index / reindex / append) run on a PRIVATE per-thread table, so the run
// has a deterministic integrity oracle and no lock contention masks real bugs.
//
// Every operation is checked: an ABI failure bumps "errors"; a wrong result
// (short walk, scope leak, filter leak, lost append) bumps "miscount". The
// process exits 0 iff errors == 0 AND miscount == 0, so it drops straight into
// CI or a `ctest` smoke tier.
//
// Usage: openads_daily_ops [--threads C] [--seconds S] [--rows N]
//                          [--seed K] [--dir D] [--csv]

#include "openads/ace.h"

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

namespace {

double now_ms() {
    using clock = std::chrono::steady_clock;
    return std::chrono::duration<double, std::milli>(
               clock::now().time_since_epoch())
        .count();
}

// Borrow a NUL-terminated UNSIGNED8* from a std::string via a scratch buffer.
UNSIGNED8* S(const std::string& s, std::vector<UNSIGNED8>& b) {
    b.assign(s.begin(), s.end());
    b.push_back(0);
    return b.data();
}

// Twelve cities, assigned round-robin at seed time so every city is guaranteed
// to exist and its row count is N/NCITY (+/-1) — a known oracle for filter/scope.
const char* CITIES[] = {
    "Sao Paulo", "Rio",      "Belo Horizonte", "Curitiba",
    "Porto Alegre", "Salvador", "Recife",       "Fortaleza",
    "Brasilia", "Manaus",   "Goiania",        "Belem"};
constexpr int NCITY = 12;
// A single-word city (no spaces) so a prefix seek/scope/AOF key isolates exactly
// its rows and no other city shares the prefix.
const char* PROBE_CITY = "Curitiba";

const char* NAMES[] = {"Ana", "Bruno", "Carla", "Diego", "Elena", "Felipe",
                       "Gabi", "Hugo",  "Ines",  "Joao",  "Kelly", "Lucas"};
constexpr int NNAME = 12;

enum Op { OP_SCAN, OP_SEEK, OP_FILTER, OP_SWAP, OP_SCOPE, OP_TEMPIDX,
          OP_REINDEX, OP_APPEND, OP_FINAL, OP_N };
const char* OP_NAME[OP_N] = {"scan", "seek", "filter", "swap", "scope",
                             "tempidx", "reindex", "append", "final"};

// When true, every iteration picks only the mutating private-table ops
// (temp index / reindex / append) — used to isolate concurrent index/table
// creation from the shared read paths.
bool g_mutate_only = false;

struct Stats {
    std::atomic<std::uint64_t> ops{0};
    std::atomic<std::uint64_t> errors{0};
    std::atomic<std::uint64_t> miscount{0};
    std::atomic<std::uint64_t> appends{0};
    std::atomic<std::uint64_t> temp_indexes{0};
    std::atomic<std::uint64_t> reindexes{0};
    std::atomic<std::uint64_t> err_by[OP_N]{};
    std::atomic<std::uint64_t> mis_by[OP_N]{};
    std::atomic<std::uint32_t> first_rc[OP_N]{};
};

std::string trim(const char* buf, std::uint32_t len) {
    std::string s(buf, len);
    while (!s.empty() && (s.back() == ' ' || s.back() == '\0')) s.pop_back();
    return s;
}

// Read a CHARACTER field of the current record into a trimmed std::string.
bool get_field(ADSHANDLE h, const char* field, std::string& out) {
    std::vector<UNSIGNED8> fb;
    UNSIGNED8 buf[256];
    UNSIGNED32 len = sizeof(buf);
    std::vector<UNSIGNED8> nb;
    if (AdsGetField(h, S(field, nb), buf, &len, 0) != 0) return false;
    out = trim(reinterpret_cast<const char*>(buf), len);
    return true;
}

// Build the shared CUSTOMERS table with a 3-tag production CDX.
int seed_shared(const std::string& dir, std::uint32_t rows) {
    std::vector<UNSIGNED8> b;
    ADSHANDLE hConn = 0;
    if (AdsConnect60(S(dir, b), ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn))
        return 1;
    ADSHANDLE hC = 0;
    std::string def = "CUSTID,N,8,0;NAME,C,30,0;CITY,C,20,0;BALANCE,N,14,2";
    std::vector<UNSIGNED8> bn, bd;
    if (AdsCreateTable(hConn, S(std::string("CUSTOMERS"), bn), nullptr, ADS_CDX,
                       0, 0, 0, 0, S(def, bd), &hC)) {
        AdsDisconnect(hConn);
        return 2;
    }
    std::vector<UNSIGNED8> fb;
    UNSIGNED32 seed_rows_rc = 0;
    for (std::uint32_t i = 1; i <= rows && !seed_rows_rc; ++i) {
        seed_rows_rc |= AdsAppendRecord(hC);
        seed_rows_rc |= AdsSetDouble(hC, S("CUSTID", fb), static_cast<double>(i));
        std::string nm = std::string(NAMES[i % NNAME]) + " #" + std::to_string(i);
        std::vector<UNSIGNED8> vb;
        seed_rows_rc |= AdsSetString(hC, S("NAME", fb), S(nm, vb), static_cast<UNSIGNED32>(nm.size()));
        std::string ci = CITIES[i % NCITY];
        std::vector<UNSIGNED8> cb;
        seed_rows_rc |= AdsSetString(hC, S("CITY", fb), S(ci, cb), static_cast<UNSIGNED32>(ci.size()));
        seed_rows_rc |= AdsSetDouble(hC, S("BALANCE", fb), static_cast<double>(i) * 1.5);
        seed_rows_rc |= AdsWriteRecord(hC);
    }
    if (seed_rows_rc) {
        AdsCloseTable(hC);
        AdsDisconnect(hConn);
        return 4;
    }
    // Production CDX (named after the table) so AdsOpenTable auto-binds it.
    // Every AdsCreateIndex61 needs a non-null phIndex out-param — the ABI
    // rejects a null one with AE_INTERNAL_ERROR (so the tag would be skipped).
    int seed_rc = 0;
    ADSHANDLE h = 0;
    { std::vector<UNSIGNED8> a, c, d;
      seed_rc |= AdsCreateIndex61(hC, S("CUSTOMERS.CDX", a), S("TID", c),
                   S("CUSTID", d), nullptr, nullptr, 0, 0, &h); }
    { std::vector<UNSIGNED8> a, c, d;
      seed_rc |= AdsCreateIndex61(hC, S("CUSTOMERS.CDX", a), S("TNAME", c),
                   S("NAME", d), nullptr, nullptr, 0, 0, &h); }
    { std::vector<UNSIGNED8> a, c, d;
      seed_rc |= AdsCreateIndex61(hC, S("CUSTOMERS.CDX", a), S("TCITY", c),
                   S("CITY", d), nullptr, nullptr, 0, 0, &h); }
    AdsCloseTable(hC);
    if (seed_rc) {
        AdsDisconnect(hConn);
        return 3;
    }
    AdsDisconnect(hConn);
    return 0;
}

void worker(int tid, const std::string& dir, int seconds, unsigned seed,
            std::uint32_t rows, Stats* g) {
    Op cur = OP_SCAN;
    UNSIGNED32 last_rc = 0;
    auto fail = [&]() {
        g->errors.fetch_add(1);
        g->err_by[cur].fetch_add(1);
        UNSIGNED32 z = 0;
        g->first_rc[cur].compare_exchange_strong(z, last_rc ? last_rc : 0xFFFFFFFFu);
    };
    auto miss = [&]() {
        g->miscount.fetch_add(1);
        g->mis_by[cur].fetch_add(1);
    };
    auto EL = [&](UNSIGNED32 rc, const char* /*label*/) -> bool {
        if (rc) { last_rc = rc; fail(); return true; }
        return false;
    };
    auto E = [&](UNSIGNED32 rc) -> bool { return EL(rc, "?"); };
    (void)E;

    std::vector<UNSIGNED8> b;
    ADSHANDLE hConn = 0;
    if (AdsConnect60(S(dir, b), ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn)) {
        fail();
        return;
    }
    ADSHANDLE hS = 0;  // shared CUSTOMERS
    if (AdsOpenTable(hConn, S(std::string("CUSTOMERS"), b), nullptr, ADS_CDX, 0,
                     0, 0, 0, &hS)) {
        fail();
        AdsDisconnect(hConn);
        return;
    }
    ADSHANDLE hTID = 0, hTCITY = 0;
    AdsGetIndexHandle(hS, S(std::string("TID"), b), &hTID);
    AdsGetIndexHandle(hS, S(std::string("TCITY"), b), &hTCITY);

    // Private per-thread table for the mutating daily patterns.
    const std::string ptab = "priv" + std::to_string(tid);
    const std::string tmp_cdx = (fs::path(dir) / (ptab + "_t.cdx")).string();
    ADSHANDLE hP = 0;
    {
        std::string def = "PID,N,10,0;VAL,C,16,0";
        std::vector<UNSIGNED8> a, c;
        if (AdsCreateTable(hConn, S(ptab, a), nullptr, ADS_CDX, 0, 0, 0, 0,
                           S(def, c), &hP)) {
            fail();
            AdsCloseTable(hS);
            AdsDisconnect(hConn);
            return;
        }
    }
    std::uint32_t priv_seed = 64;  // private rows present before the loop
    {
        std::vector<UNSIGNED8> fb;
        for (std::uint32_t i = 1; i <= priv_seed; ++i) {
            AdsAppendRecord(hP);
            AdsSetDouble(hP, S("PID", fb), static_cast<double>(i));
            std::string v = "v" + std::to_string(i);
            std::vector<UNSIGNED8> vb;
            AdsSetString(hP, S("VAL", fb), S(v, vb), static_cast<UNSIGNED32>(v.size()));
            AdsWriteRecord(hP);
        }
        std::vector<UNSIGNED8> a, c, d;
        ADSHANDLE hpidx = 0;  // phIndex must be non-null (ABI rejects null)
        AdsCreateIndex61(hP, S(ptab + ".CDX", a), S("TPID", c), S("PID", d),
                         nullptr, nullptr, 0, 0, &hpidx);
    }
    std::uint64_t priv_count = priv_seed;
    std::uint32_t priv_next = priv_seed + 1;

    std::mt19937 rng(seed + static_cast<unsigned>(tid) * 7919u);
    const double t_end = now_ms() + seconds * 1000.0;

    while (now_ms() < t_end) {
        int roll = static_cast<int>(rng() % 100);
        if (g_mutate_only) roll = 76 + static_cast<int>(rng() % 24);  // temp/reindex/append
        g->ops.fetch_add(1);
        std::vector<UNSIGNED8> fb;

        if (roll < 22) {  // ---- SCAN: full walk of one order, must see all rows
            cur = OP_SCAN;
            const char* tag = (roll % 3 == 0) ? "TID"
                              : (roll % 3 == 1) ? "TNAME"
                                                : "TCITY";
            std::vector<UNSIGNED8> tb;
            if (EL(AdsSetIndexOrder(hS, S(tag, tb)), "scan.setorder")) continue;
            if (EL(AdsGotoTop(hS), "scan.gototop")) continue;
            std::uint64_t seen = 0;
            UNSIGNED16 eof = 0;
            bool ok = true;
            while (true) {
                if (EL(AdsAtEOF(hS, &eof), "scan.ateof")) { ok = false; break; }
                if (eof) break;
                ++seen;
                if (EL(AdsSkip(hS, 1), "scan.skip")) { ok = false; break; }
            }
            if (ok && seen != rows) miss();

        } else if (roll < 40) {  // ---- SEEK: key known to exist must be found
            cur = OP_SEEK;
            std::vector<UNSIGNED8> kb;
            UNSIGNED16 found = 0;
            std::string key = PROBE_CITY;
            if (E(AdsSetIndexOrder(hS, S(std::string("TCITY"), fb)))) continue;
            if (E(AdsSeek(hTCITY, S(key, kb), static_cast<UNSIGNED16>(key.size()),
                          ADS_STRINGKEY, ADS_SOFTSEEK, &found))) continue;
            if (!found) { miss(); continue; }
            std::string city;
            if (get_field(hS, "CITY", city) && city.rfind(PROBE_CITY, 0) != 0)
                miss();

        } else if (roll < 56) {  // ---- FILTER (AOF): only matching rows visible
            cur = OP_FILTER;
            std::string cond = std::string("CITY = '") + PROBE_CITY + "'";
            std::vector<UNSIGNED8> cb;
            if (E(AdsSetAOF(hS, S(cond, cb), 0))) continue;
            AdsGotoTop(hS);
            UNSIGNED16 eof = 0;
            int checked = 0;
            bool leak = false;
            while (checked < 32) {
                if (AdsAtEOF(hS, &eof) || eof) break;
                std::string city;
                if (get_field(hS, "CITY", city) && city.rfind(PROBE_CITY, 0) != 0) {
                    leak = true;
                    break;
                }
                ++checked;
                if (AdsSkip(hS, 1)) break;
            }
            if (E(AdsClearAOF(hS))) {}
            if (leak) miss();

        } else if (roll < 66) {  // ---- ORDER SWAP: switch active order mid-session
            cur = OP_SWAP;
            const char* tag = (roll & 1) ? "TNAME" : "TID";
            std::vector<UNSIGNED8> tb;
            if (EL(AdsSetIndexOrder(hS, S(tag, tb)), "swap.setorder")) continue;
            if (EL(AdsGotoTop(hS), "swap.gototop")) {}

        } else if (roll < 76) {  // ---- SCOPE: range must not leak out-of-range rows
            cur = OP_SCOPE;
            // Fixed-width char keys are space-padded on disk, so a bare prefix
            // is the lower bound but the upper bound must out-rank every padded
            // key sharing the prefix — append high bytes ("Curitiba\xFF...").
            std::vector<UNSIGNED8> ktb, kbb;
            std::string ktop = PROBE_CITY;
            std::string kbot = std::string(PROBE_CITY) + std::string(8, '\xFF');
            if (EL(AdsSetIndexOrder(hS, S(std::string("TCITY"), fb)), "scope.setorder")) continue;
            if (EL(AdsSetScope(hTCITY, ADS_TOP, S(ktop, ktb),
                               static_cast<UNSIGNED16>(ktop.size()), ADS_STRINGKEY),
                   "scope.settop")) continue;
            if (EL(AdsSetScope(hTCITY, ADS_BOTTOM, S(kbot, kbb),
                               static_cast<UNSIGNED16>(kbot.size()), ADS_STRINGKEY),
                   "scope.setbot")) continue;
            AdsGotoTop(hS);
            UNSIGNED16 eof = 0;
            int walked = 0;
            bool leak = false;
            while (walked < 5000) {
                if (AdsAtEOF(hS, &eof) || eof) break;
                std::string city;
                if (get_field(hS, "CITY", city) && city.rfind(PROBE_CITY, 0) != 0) {
                    leak = true;
                    break;
                }
                ++walked;
                if (AdsSkip(hS, 1)) break;
            }
            AdsClearScope(hTCITY, ADS_TOP);
            AdsClearScope(hTCITY, ADS_BOTTOM);
            if (leak || walked == 0) miss();

        } else if (roll < 84) {  // ---- TEMP INDEX: ad-hoc report index, then discard
            cur = OP_TEMPIDX;
            ADSHANDLE hTmp = 0;
            std::vector<UNSIGNED8> a, c, d;
            if (E(AdsCreateIndex61(hP, S(tmp_cdx, a), S("TMP", c), S("VAL", d),
                                   nullptr, nullptr, 0, 0, &hTmp))) continue;
            std::vector<UNSIGNED8> kb;
            UNSIGNED16 found = 0;
            std::string key = "v1";
            if (E(AdsSeek(hTmp, S(key, kb), static_cast<UNSIGNED16>(key.size()),
                          ADS_STRINGKEY, ADS_SOFTSEEK, &found))) {}
            else if (!found) { miss(); }
            AdsCloseIndex(hTmp);
            g->temp_indexes.fetch_add(1);

        } else if (roll < 92) {  // ---- REINDEX: rebuild private-table indexes
            cur = OP_REINDEX;
            if (E(AdsReindex(hP))) {}
            else g->reindexes.fetch_add(1);

        } else {  // ---- APPEND: grow the private table
            cur = OP_APPEND;
            std::vector<UNSIGNED8> vb2;
            if (E(AdsAppendRecord(hP))) continue;
            AdsSetDouble(hP, S("PID", fb), static_cast<double>(priv_next));
            std::string v = "v" + std::to_string(priv_next);
            std::vector<UNSIGNED8> vb;
            AdsSetString(hP, S("VAL", fb), S(v, vb), static_cast<UNSIGNED32>(v.size()));
            if (E(AdsWriteRecord(hP))) continue;
            ++priv_next;
            ++priv_count;
            g->appends.fetch_add(1);
        }
    }

    // Integrity oracle: the private table must hold exactly the rows we appended.
    cur = OP_FINAL;
    UNSIGNED32 final_cnt = 0;
    if (AdsGetRecordCount(hP, 0, &final_cnt) == 0) {
        if (final_cnt != priv_count) miss();
    } else {
        fail();
    }

    AdsCloseTable(hP);
    AdsCloseTable(hS);
    AdsDisconnect(hConn);
    std::error_code ec;
    fs::remove(tmp_cdx, ec);
}

}  // namespace

int main(int argc, char** argv) {
    std::setbuf(stderr, nullptr);

    int threads = 16;
    int seconds = 6;
    int rows = 5000;
    unsigned seed = 1234;
    bool csv = false;
    std::string dir;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        auto next = [&]() -> const char* { return (i + 1 < argc) ? argv[++i] : ""; };
        if (a == "--threads") threads = std::atoi(next());
        else if (a == "--seconds") seconds = std::atoi(next());
        else if (a == "--rows") rows = std::atoi(next());
        else if (a == "--seed") seed = static_cast<unsigned>(std::strtoul(next(), nullptr, 10));
        else if (a == "--dir") dir = next();
        else if (a == "--csv") csv = true;
        else if (a == "--mutate") g_mutate_only = true;
    }
    if (threads < 1) threads = 1;
    if (seconds < 1) seconds = 1;
    if (rows < 1) rows = 1;

    if (dir.empty())
        dir = (fs::temp_directory_path() / "openads_daily_ops").string();
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir, ec);

    if (int rc = seed_shared(dir, static_cast<std::uint32_t>(rows))) {
        std::fprintf(stderr, "FATAL: seed_shared failed (%d)\n", rc);
        return 1;
    }
    std::fprintf(stderr,
                 "daily-ops | %d rows | %d threads | %ds | seed %u | dir %s\n",
                 rows, threads, seconds, seed, dir.c_str());

    Stats g;
    const double t0 = now_ms();
    std::vector<std::thread> ws;
    ws.reserve(static_cast<std::size_t>(threads));
    for (int t = 0; t < threads; ++t)
        ws.emplace_back(worker, t, dir, seconds, seed,
                        static_cast<std::uint32_t>(rows), &g);
    for (auto& w : ws) w.join();
    const double secs = (now_ms() - t0) / 1000.0;

    const std::uint64_t ops = g.ops.load(), errs = g.errors.load(),
                        mis = g.miscount.load();
    const double tput = secs > 0 ? static_cast<double>(ops) / secs : 0;

    if (csv) {
        std::printf("threads,seconds,rows,ops,throughput,appends,temp_indexes,"
                    "reindexes,errors,miscount\n");
        std::printf("%d,%d,%d,%llu,%.0f,%llu,%llu,%llu,%llu,%llu\n", threads,
                    seconds, rows, (unsigned long long)ops, tput,
                    (unsigned long long)g.appends.load(),
                    (unsigned long long)g.temp_indexes.load(),
                    (unsigned long long)g.reindexes.load(),
                    (unsigned long long)errs, (unsigned long long)mis);
    } else {
        std::printf("\n=== openads_daily_ops ===\n");
        std::printf("threads        : %d\n", threads);
        std::printf("ops            : %llu  (%.0f/s)\n", (unsigned long long)ops, tput);
        std::printf("appends        : %llu\n", (unsigned long long)g.appends.load());
        std::printf("temp indexes   : %llu\n", (unsigned long long)g.temp_indexes.load());
        std::printf("reindexes      : %llu\n", (unsigned long long)g.reindexes.load());
        std::printf("errors         : %llu  (must be 0)\n", (unsigned long long)errs);
        std::printf("miscount       : %llu  (must be 0)\n", (unsigned long long)mis);
        if (errs || mis) {
            std::printf("  per-op (err/mis, first rc):\n");
            for (int o = 0; o < OP_N; ++o) {
                std::uint64_t e = g.err_by[o].load(), m = g.mis_by[o].load();
                if (e || m)
                    std::printf("    %-8s err=%llu mis=%llu rc=%u\n", OP_NAME[o],
                                (unsigned long long)e, (unsigned long long)m,
                                g.first_rc[o].load());
            }
        }
    }

    fs::remove_all(dir, ec);
    return (errs == 0 && mis == 0) ? 0 : 2;
}
