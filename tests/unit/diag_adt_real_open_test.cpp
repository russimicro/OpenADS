// TEMPORARY diagnostic: open the REAL ERP ADT table + .ADI (reroute) the way the
// rddads RDD does, and report the rc of every step to find what "ERROR ADS #0"
// is when opening an indexed ADT table. Skips cleanly if the prod files are
// absent. DELETE after diagnosis.
#include "doctest.h"
#include "openads/ace.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace {
struct EnvGuard {
    const char* n_;
    EnvGuard(const char* n, const char* v) : n_(n) {
#ifdef _WIN32
        _putenv_s(n, v);
#else
        setenv(n, v, 1);
#endif
    }
    ~EnvGuard() {
#ifdef _WIN32
        _putenv_s(n_, "");
#else
        unsetenv(n_);
#endif
    }
};
} // namespace

// Try one table the way the ERP RDD does. Returns "" on full success, else a
// short reason naming the failing step + rc.
static std::string try_open_indexed(ADSHANDLE hConn, const fs::path& dat,
                                    const fs::path& adi) {
    // Pass the FULL path as table name so subdir tables (MVTOS/, CONTABIL/)
    // resolve regardless of the connection's base dir.
    std::string nm = dat.string();
    UNSIGNED8 tbl[512]{}; std::memcpy(tbl, nm.c_str(), nm.size());
    ADSHANDLE hT = 0;
    // Read-WRITE shared, like the ERP's lUsaTab(...,lShared) for the
    // login-time MVTOS validation/update pass.
    UNSIGNED32 rcO = AdsOpenTable(hConn, tbl, nullptr, ADS_ADT, ADS_ANSI,
                                  ADS_SHARED, 0, ADS_DEFAULT, &hT);
    if (rcO != AE_SUCCESS) return "AdsOpenTable(rw) rc=" + std::to_string(rcO);

    std::string adis = adi.string();
    std::vector<UNSIGNED8> b(adis.size() + 1, 0);
    std::memcpy(b.data(), adis.data(), adis.size());
    ADSHANDLE arr[64] = {0};
    UNSIGNED16 alen = 64;
    UNSIGNED32 rcI = AdsOpenIndex(hT, b.data(), arr, &alen);
    if (rcI != AE_SUCCESS) { AdsCloseTable(hT);
        return "AdsOpenIndex rc=" + std::to_string(rcI); }

    UNSIGNED16 nIdx = 0;
    AdsGetNumIndexes(hT, &nIdx);
    if (nIdx == 0) { AdsCloseTable(hT); return "0 indexes bound"; }

    std::string reason;
    if (alen > 0 && arr[0] != 0) {
        UNSIGNED32 rcS = AdsSetIndexOrderByHandle(hT, arr[0]);
        UNSIGNED32 rcG = AdsGotoTop(hT);
        UNSIGNED32 rn = 0;
        UNSIGNED32 rcR = AdsGetRecordNum(hT, 0, &rn);
        if (rcS || rcG || rcR)
            reason = "nav setOrder=" + std::to_string(rcS) +
                     " gotop=" + std::to_string(rcG) +
                     " recnum=" + std::to_string(rcR);
    }
    AdsCloseTable(hT);
    return reason;
}

TEST_CASE("DIAG: MOVABR01 with reroute OFF (simulate missing env flag)") {
    const char* base = "F:/zerus64/0X/bases";
    fs::path dat = fs::path(base) / "MVTOS" / "MOVABR01.DAT";
    fs::path adi = fs::path(base) / "MVTOS" / "MOVABR01.ADI";
    std::error_code ec;
    if (!fs::exists(dat, ec) || !fs::exists(adi, ec)) { MESSAGE("absent -> skip"); return; }
    // NO EnvGuard: OPENADS_ADT_CDX_INDEX unset -> native AdiIndex path, which
    // must read the CDX-format .adi the reroute wrote.
    UNSIGNED8 srv[260]{}; std::memcpy(srv, base, std::strlen(base));
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == AE_SUCCESS);
    std::string dn = dat.string();
    UNSIGNED8 tbl[512]{}; std::memcpy(tbl, dn.c_str(), dn.size());
    ADSHANDLE hT = 0;
    UNSIGNED32 rcO = AdsOpenTable(hConn, tbl, nullptr, ADS_ADT, ADS_ANSI, ADS_SHARED, 0, ADS_DEFAULT, &hT);
    MESSAGE("[FLAG OFF] AdsOpenTable rc=", rcO);
    if (rcO != AE_SUCCESS) { MESSAGE("[FLAG OFF] open FAILED rc=", rcO); AdsDisconnect(hConn); return; }
    std::string adis = adi.string();
    std::vector<UNSIGNED8> b(adis.size()+1, 0); std::memcpy(b.data(), adis.data(), adis.size());
    ADSHANDLE arr[128] = {0}; UNSIGNED16 alen = 128;
    UNSIGNED32 rcI = AdsOpenIndex(hT, b.data(), arr, &alen);
    MESSAGE("[FLAG OFF] AdsOpenIndex rc=", rcI, " ntags=", alen);
    UNSIGNED16 n = 0; AdsGetNumIndexes(hT, &n);
    MESSAGE("[FLAG OFF] num indexes=", n);
    if (alen > 0) {
        UNSIGNED32 rcS = AdsSetIndexOrderByHandle(hT, arr[0]);
        UNSIGNED32 rcG = AdsGotoTop(hT);
        UNSIGNED32 rn = 0; AdsGetRecordNum(hT, 0, &rn);
        MESSAGE("[FLAG OFF] setord=", rcS, " gotop=", rcG, " recno=", rn);
    }
    AdsCloseTable(hT);
    AdsDisconnect(hConn);
}

TEST_CASE("DIAG: MOVABR01 detailed — every tag + seek") {
    const char* base = "F:/zerus64/0X/bases";
    fs::path dat = fs::path(base) / "MVTOS" / "MOVABR01.DAT";
    fs::path adi = fs::path(base) / "MVTOS" / "MOVABR01.ADI";
    std::error_code ec;
    if (!fs::exists(dat, ec) || !fs::exists(adi, ec)) {
        MESSAGE("MOVABR01 absent -> skip"); return;
    }
    EnvGuard _g("OPENADS_ADT_CDX_INDEX", "1");
    UNSIGNED8 srv[260]{}; std::memcpy(srv, base, std::strlen(base));
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == AE_SUCCESS);

    std::string dn = dat.string();
    UNSIGNED8 tbl[512]{}; std::memcpy(tbl, dn.c_str(), dn.size());
    ADSHANDLE hT = 0;
    UNSIGNED32 rcO = AdsOpenTable(hConn, tbl, nullptr, ADS_ADT, ADS_ANSI, ADS_SHARED, 0, ADS_DEFAULT, &hT);
    MESSAGE("AdsOpenTable(rw) rc=", rcO, " hT=", hT);
    REQUIRE(rcO == AE_SUCCESS);

    UNSIGNED32 nrec = 0; AdsGetRecordCount(hT, ADS_IGNOREFILTERS, &nrec);
    MESSAGE("record count=", nrec);

    std::string adis = adi.string();
    std::vector<UNSIGNED8> b(adis.size()+1, 0); std::memcpy(b.data(), adis.data(), adis.size());
    ADSHANDLE arr[128] = {0}; UNSIGNED16 alen = 128;
    UNSIGNED32 rcI = AdsOpenIndex(hT, b.data(), arr, &alen);
    MESSAGE("AdsOpenIndex rc=", rcI, " ntags=", alen);

    for (UNSIGNED16 i = 0; i < alen; ++i) {
        UNSIGNED8 nm[64]{}; UNSIGNED16 nl = 64;
        UNSIGNED32 rcNm = AdsGetIndexName(arr[i], nm, &nl);
        UNSIGNED8 ex[256]{}; UNSIGNED16 el = 256;
        UNSIGNED32 rcEx = AdsGetIndexExpr(arr[i], ex, &el);
        UNSIGNED32 rcS = AdsSetIndexOrderByHandle(hT, arr[i]);
        UNSIGNED32 rcG = AdsGotoTop(hT);
        UNSIGNED32 rn = 0; UNSIGNED32 rcR = AdsGetRecordNum(hT, 0, &rn);
        UNSIGNED32 rcSk = AdsSkip(hT, 1); UNSIGNED32 rn2 = 0; AdsGetRecordNum(hT, 0, &rn2);
        std::string sname(reinterpret_cast<const char*>(nm));
        std::string sexpr(reinterpret_cast<const char*>(ex));
        MESSAGE("tag[", i, "] name='", sname,
                "' expr='", sexpr,
                "' setord=", rcS, " gotop=", rcG, " recno=", rn,
                " skip=", rcSk, " recno2=", rn2,
                " (rcNm=", rcNm, " rcEx=", rcEx, " rcR=", rcR, ")");
    }
    AdsCloseTable(hT);
    AdsDisconnect(hConn);
}

TEST_CASE("DIAG: scan all real ADT tables, report open/index failures") {
    const char* base = "F:/zerus64/0X/bases";
    std::error_code ec;
    if (!fs::exists(base, ec)) { MESSAGE("base absent -> skip"); return; }

    EnvGuard _g("OPENADS_ADT_CDX_INDEX", "1");

    UNSIGNED8 srv[260]{};
    std::memcpy(srv, base, std::strlen(base));
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn)
            == AE_SUCCESS);

    int ok = 0, fail = 0;
    for (auto& de : fs::recursive_directory_iterator(base, ec)) {
        if (!de.is_regular_file()) continue;
        fs::path p = de.path();
        std::string e = p.extension().string();
        for (auto& c : e) c = (char)std::tolower((unsigned char)c);
        if (e != ".adi") continue;
        fs::path dat = p; dat.replace_extension(".DAT");
        if (!fs::exists(dat, ec)) { dat = p; dat.replace_extension(".dat"); }
        if (!fs::exists(dat, ec)) continue;
        std::string r = try_open_indexed(hConn, dat, p);
        if (r.empty()) { ++ok; }
        else { ++fail; MESSAGE("FAIL ", dat.filename().string(), " -> ", r); }
    }
    MESSAGE("ADT tables OK=", ok, " FAIL=", fail);
    AdsDisconnect(hConn);
}
