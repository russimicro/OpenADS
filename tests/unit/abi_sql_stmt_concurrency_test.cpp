#include "doctest.h"
#include "openads/ace.h"

#include <atomic>
#include <cstring>
#include <filesystem>
#include <set>
#include <thread>
#include <vector>

// Regression for the stmt_map() data race: AdsCreateSQLStatement inserted
// under the global lock while AdsCloseSQLStatement erased with no lock, so a
// rehash during a concurrent walk corrupted the table and the process crashed
// or hung once enough threads churned statements (observed at >= 8 threads).
// Both tests run a real connection through the public ABI.

namespace {

ADSHANDLE connect_local(const std::filesystem::path& dir) {
    UNSIGNED8 srv[512]{};
    std::memcpy(srv, dir.string().c_str(), dir.string().size());
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn)
            == AE_SUCCESS);
    return hConn;
}

}  // namespace

TEST_CASE("AdsCreateSQLStatement/Close survive concurrent churn") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_stmt_churn";
    std::error_code ec;
    fs::create_directories(dir, ec);
    ADSHANDLE hConn = connect_local(dir);

    const int kThreads = 8;          // the observed crash/hang threshold
    const int kIters   = 3000;
    std::atomic<int> fails{0};

    std::vector<std::thread> ts;
    for (int t = 0; t < kThreads; ++t) {
        ts.emplace_back([&]() {
            for (int i = 0; i < kIters; ++i) {
                ADSHANDLE h = 0;
                if (AdsCreateSQLStatement(hConn, &h) != AE_SUCCESS || h == 0) {
                    ++fails; return;
                }
                if (AdsCloseSQLStatement(h) != AE_SUCCESS) { ++fails; return; }
            }
        });
    }
    for (auto& th : ts) th.join();

    CHECK(fails.load() == 0);        // completes without crash/hang/error

    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

TEST_CASE("concurrent AdsCreateSQLStatement hands out unique handles") {
    namespace fs = std::filesystem;
    auto dir = fs::temp_directory_path() / "openads_stmt_unique";
    std::error_code ec;
    fs::create_directories(dir, ec);
    ADSHANDLE hConn = connect_local(dir);

    const int kThreads = 8;
    const int kPer     = 200;
    std::vector<std::vector<ADSHANDLE>> handles(kThreads);
    std::atomic<int> fails{0};

    std::vector<std::thread> ts;
    for (int t = 0; t < kThreads; ++t) {
        ts.emplace_back([&, t]() {
            handles[t].reserve(kPer);
            for (int i = 0; i < kPer; ++i) {
                ADSHANDLE h = 0;
                if (AdsCreateSQLStatement(hConn, &h) != AE_SUCCESS || h == 0) {
                    ++fails; return;
                }
                handles[t].push_back(h);
            }
        });
    }
    for (auto& th : ts) th.join();

    CHECK(fails.load() == 0);

    std::set<ADSHANDLE> all;
    int total = 0;
    for (auto& v : handles) for (auto h : v) { all.insert(h); ++total; }
    CHECK(total == kThreads * kPer);
    CHECK(static_cast<int>(all.size()) == total);   // no duplicates

    for (auto& v : handles) for (auto h : v) AdsCloseSQLStatement(h);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}
