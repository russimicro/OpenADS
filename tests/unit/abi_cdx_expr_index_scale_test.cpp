#include "doctest.h"
#include "openads/ace.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <numeric>
#include <string>
#include <vector>

// Repro for the ERP bug: after reindexing ARTICULO (34k rows) with OpenADS,
// the index whose key is an EXPRESSION (UPPER(cNombreArt)) came out with ~46%
// of its keys pinned to the WRONG recno (the key value is right and ordered,
// but the recno it points to holds a different record). The plain-field index
// (cCodigoArt) was fine. A 600-row test built the expression index correctly,
// so the corruption only shows at scale (multi-page fast build). This pins it:
// navigating an UPPER() index over many rows must visit recnos in key order.

namespace fs = std::filesystem;

namespace {

void set_str_(ADSHANDLE h, const char* field, const std::string& val) {
    UNSIGNED8 f[16];
    std::memcpy(f, field, std::strlen(field) + 1);
    std::vector<UNSIGNED8> v(val.begin(), val.end());
    v.push_back(0);
    AdsSetString(h, f, v.data(), static_cast<UNSIGNED32>(val.size()));
}

std::string up(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::toupper((unsigned char)c));
    return s;
}

} // namespace

TEST_CASE("CDX expression index (UPPER) at scale: navigation matches key order") {
    auto dir = fs::temp_directory_path() / "openads_expr_scale";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "COD,C,8,0;NOM,C,40,0";
    UNSIGNED8 tname[] = "data";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hTable) == 0);

    const int N = 30000;
    // Long SHARED prefixes (like the real catalog: "*EPS ACIDO ...",
    // "*EPS ALOPURINOL ...") exercise the compact-leaf prefix compression,
    // plus a leading space / '*' so byte order isn't the trivial digit order.
    const char* fam[] = {
        "*EPS ACIDO ", "*EPS AMINO ", "*EPS BETA BLOQ ",
        " STREPSILS LINEA ", "*CLENOX SERIE ", "item generico "
    };
    const int NF = static_cast<int>(sizeof(fam) / sizeof(fam[0]));
    std::vector<std::string> nom(N);
    for (int i = 0; i < N; ++i) {
        int perm = static_cast<int>((static_cast<long long>(i) * 53) % N);
        char b[80];
        // unique suffix (perm) keeps keys distinct; mixed case for UPPER().
        std::snprintf(b, sizeof(b), "%sprod %05d tail", fam[perm % NF], perm);
        nom[i] = b;
        REQUIRE(AdsAppendRecord(hTable) == 0);
        set_str_(hTable, "NOM", nom[i]);
    }
    REQUIRE(AdsWriteRecord(hTable) == 0);

    UNSIGNED8 fn[16]   = "data";
    UNSIGNED8 tag[16]  = "TNOM";
    UNSIGNED8 expr[32] = "UPPER(NOM)";
    ADSHANDLE hIdx = 0;
    REQUIRE(AdsCreateIndex61(hTable, fn, tag, expr, nullptr, nullptr,
                             0, 512, &hIdx) == 0);

    // Expected recno order = rows sorted by UPPER(NOM).
    std::vector<int> idx(N);
    std::iota(idx.begin(), idx.end(), 0);
    std::stable_sort(idx.begin(), idx.end(),
                     [&](int a, int b) { return up(nom[a]) < up(nom[b]); });

    // Navigate the UPPER() order; every visited recno must match the
    // by-name sort. A truncated key_size or a recno masked by too-small
    // RNBits scrambles recno<->key and trips this.
    REQUIRE(AdsGotoTop(hTable) == 0);
    int bad = 0;
    for (int k = 0; k < N; ++k) {
        UNSIGNED32 rn = 0;
        AdsGetRecordNum(hTable, 0, &rn);
        if (rn != static_cast<UNSIGNED32>(idx[k] + 1)) ++bad;
        if (k < N - 1) {
            if (AdsSkip(hTable, 1) != 0) { ++bad; break; }
        }
    }
    INFO("rows out of key order = " << bad);
    CHECK(bad == 0);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}
