#include "doctest.h"
#include "openads/ace.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <numeric>
#include <string>
#include <vector>

// Repro del bug "ordenamiento por columna" del ERP (mantenimiento de
// articulos, ARTICULO.CDX con 22 tags): al hacer DBSETORDER(n) para cambiar
// el tag activo y luego navegar, goto_top/skip NO recorren por la clave del
// order activo. Sintoma real (log GUI): con el order NOMBRE activo, goto_top
// cae en RECNO=1367 (no la primera clave) y skip(1) cae en RECNO fisico 2.
// INDEXORD()/ORDNAME() reportan el order correcto y order_ se reemplaza bien
// (parked_was=SET, order_now=SET) -> la navegacion del indice multi-tag esta
// desincronizada. El caso pequeno (1 pagina) pasa; el bug aparece con indice
// MULTI-PAGINA (miles de registros, nodos internos en el B-tree).

namespace fs = std::filesystem;

namespace {

void set_str(ADSHANDLE h, const char* field, const char* val) {
    UNSIGNED8 f[16];
    std::memcpy(f, field, std::strlen(field) + 1);
    UNSIGNED8 v[32];
    std::memcpy(v, val, std::strlen(val) + 1);
    AdsSetString(h, f, v, static_cast<UNSIGNED32>(std::strlen(val)));
}

void make_tag(ADSHANDLE hTable, const char* tag, const char* expr) {
    UNSIGNED8 fn[16];
    std::memcpy(fn, "data", 5);
    UNSIGNED8 t[64];
    std::memcpy(t, tag, std::strlen(tag) + 1);
    UNSIGNED8 e[64];
    std::memcpy(e, expr, std::strlen(expr) + 1);
    ADSHANDLE hIdx = 0;
    REQUIRE(AdsCreateIndex61(hTable, fn, t, e, nullptr, nullptr,
                             0, 512, &hIdx) == 0);
}

void make_tag_cond(ADSHANDLE hTable, const char* tag, const char* expr,
                   const char* cond) {
    UNSIGNED8 fn[16];
    std::memcpy(fn, "data", 5);
    UNSIGNED8 t[64];
    std::memcpy(t, tag, std::strlen(tag) + 1);
    UNSIGNED8 e[64];
    std::memcpy(e, expr, std::strlen(expr) + 1);
    UNSIGNED8 c[64];
    std::memcpy(c, cond, std::strlen(cond) + 1);
    ADSHANDLE hIdx = 0;
    REQUIRE(AdsCreateIndex61(hTable, fn, t, e, c, nullptr, 0, 512, &hIdx) == 0);
}

UNSIGNED32 recno_now(ADSHANDLE h) {
    UNSIGNED32 r = 0;
    AdsGetRecordNum(h, 0, &r);
    return r;
}

void set_order(ADSHANDLE h, const char* tag) {
    UNSIGNED8 t[64];
    std::memcpy(t, tag, std::strlen(tag) + 1);
    REQUIRE(AdsSetIndexOrder(h, t) == 0);
}

} // namespace

TEST_CASE("multi-tag CDX (single page): nav follows active order") {
    auto dir = fs::temp_directory_path() / "openads_multitag_small";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "COD,C,3,0;NOM,C,8,0";
    UNSIGNED8 tname[] = "data";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hTable) == 0);

    struct Row { const char* cod; const char* nom; };
    const Row rows[5] = {
        {"003", "MANGO"}, {"001", "ZEBRA"}, {"005", "ALFA"},
        {"002", "PERA"},  {"004", "KIWI"},
    };
    for (const auto& r : rows) {
        REQUIRE(AdsAppendRecord(hTable) == 0);
        set_str(hTable, "COD", r.cod);
        set_str(hTable, "NOM", r.nom);
    }
    REQUIRE(AdsWriteRecord(hTable) == 0);

    make_tag(hTable, "TCOD", "COD");
    make_tag(hTable, "TNOM", "UPPER(NOM)");
    make_tag(hTable, "TCN",  "COD+NOM");
    make_tag_cond(hTable, "TF1", "COD", "NOM > 'M'");

    const UNSIGNED32 byName[5] = {3, 5, 1, 4, 2};
    const UNSIGNED32 byCode[5] = {2, 4, 1, 5, 3};

    auto walk = [&](const char* tag, const UNSIGNED32* expect) {
        set_order(hTable, tag);
        REQUIRE(AdsGotoTop(hTable) == 0);
        CHECK(recno_now(hTable) == expect[0]);
        for (int i = 1; i < 5; ++i) {
            REQUIRE(AdsSkip(hTable, 1) == 0);
            CHECK(recno_now(hTable) == expect[i]);
        }
    };

    walk("TNOM", byName);
    walk("TCOD", byCode);
    walk("TNOM", byName);

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("multi-tag CDX (multi-page): nav follows active order across B-tree pages") {
    auto dir = fs::temp_directory_path() / "openads_multitag_big";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "COD,C,6,0;NOM,C,6,0";
    UNSIGNED8 tname[] = "data";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hTable) == 0);

    // ~600 rows -> the index spans several leaf pages + internal nodes.
    // COD and NOM are independent permutations of the physical order, so
    // "by code" and "by name" both differ from each other and from recno.
    const int N = 600;
    std::vector<std::string> cod(N), nom(N);
    for (int i = 0; i < N; ++i) {
        char b[16];
        std::snprintf(b, sizeof(b), "C%05d", (i * 53) % N);
        cod[i] = b;
        std::snprintf(b, sizeof(b), "N%05d", (i * 37) % N);
        nom[i] = b;
        REQUIRE(AdsAppendRecord(hTable) == 0);
        set_str(hTable, "COD", cod[i].c_str());
        set_str(hTable, "NOM", nom[i].c_str());
    }
    REQUIRE(AdsWriteRecord(hTable) == 0);

    make_tag(hTable, "TCOD", "COD");
    make_tag(hTable, "TNOM", "UPPER(NOM)");
    make_tag(hTable, "TCN",  "COD+NOM");
    make_tag(hTable, "TNC",  "NOM+COD");
    make_tag_cond(hTable, "TF1", "COD", "NOM > 'N00300'");
    make_tag_cond(hTable, "TF2", "UPPER(NOM)", "COD > 'C00300'");

    // Expected recno order for a given key vector (1-based recnos).
    auto order_by = [&](const std::vector<std::string>& key) {
        std::vector<int> idx(N);
        std::iota(idx.begin(), idx.end(), 0);
        std::stable_sort(idx.begin(), idx.end(),
                         [&](int a, int b) { return key[a] < key[b]; });
        std::vector<UNSIGNED32> recnos(N);
        for (int k = 0; k < N; ++k)
            recnos[k] = static_cast<UNSIGNED32>(idx[k] + 1);
        return recnos;
    };
    const std::vector<UNSIGNED32> byName = order_by(nom);
    const std::vector<UNSIGNED32> byCode = order_by(cod);

    // Reopen with AdsOpenTable so the production CDX auto-attaches ALL tags
    // (M-AOF.6) exactly like the ERP's lUsaTab/AdsOpenTable, and drive the
    // active order BY NUMBER (AdsGetIndexHandleByOrder + SetIndexOrderByHandle)
    // — the rddads DBSETORDER(n) path the ERP actually uses (bindings created
    // on the fly), instead of AdsSetIndexOrder by tag name.
    REQUIRE(AdsCloseTable(hTable) == 0);
    UNSIGNED8 onm[16] = "data";
    hTable = 0;
    REQUIRE(AdsOpenTable(hConn, onm, onm, ADS_CDX, 1, 1, 0, 1, &hTable) == 0);

    auto seq_for_order = [&](UNSIGNED16 ord) {
        ADSHANDLE hIdx = 0;
        REQUIRE(AdsGetIndexHandleByOrder(hTable, ord, &hIdx) == 0);
        REQUIRE(AdsSetIndexOrderByHandle(hTable, hIdx) == 0);
        // Navigate passing the INDEX handle (hOrdCurrent) as the operand,
        // EXACTLY as the Harbour rddads RDD does for DBGOTOP/DBSKIP — not the
        // table handle. This is the path the GUI uses and where the bug shows.
        std::vector<UNSIGNED32> seq;
        REQUIRE(AdsGotoTop(hIdx) == 0);
        UNSIGNED32 r0 = 0;
        AdsGetRecordNum(hIdx, 0, &r0);
        seq.push_back(r0);
        for (int i = 1; i < N; ++i) {
            if (AdsSkip(hIdx, 1) != 0) break;
            UNSIGNED32 r = 0;
            AdsGetRecordNum(hIdx, 0, &r);
            seq.push_back(r);
        }
        return seq;
    };
    auto is_perm = [&](const std::vector<UNSIGNED32>& s) {
        if (static_cast<int>(s.size()) != N) return false;
        std::vector<char> seen(N + 1, 0);
        for (auto r : s) {
            if (r < 1 || r > static_cast<UNSIGNED32>(N) || seen[r]) return false;
            seen[r] = 1;
        }
        return true;
    };

    // Orders 1 and 2 are the non-conditional code/name tags. Each must yield a
    // FULL permutation of the N recnos in its key's order. The bug (skip stuck
    // on physical recno 2) breaks is_perm and the byCode/byName match.
    bool matched_name = false, matched_code = false;
    for (UNSIGNED16 ord = 1; ord <= 2; ++ord) {
        auto seq = seq_for_order(ord);
        CHECK(is_perm(seq));
        if (seq == byName) matched_name = true;
        if (seq == byCode) matched_code = true;
    }
    CHECK(matched_name);
    CHECK(matched_code);

    // Re-activate each order a 2nd time (the failing "2nd click" alternation).
    {
        auto s2 = seq_for_order(2);
        CHECK(is_perm(s2));
        auto s1 = seq_for_order(1);
        CHECK(is_perm(s1));
        auto s2b = seq_for_order(2);
        CHECK(is_perm(s2b));
    }

    // POSITION primitives the XBrowse uses to PAINT and SCROLL over the active
    // order (AdsGetKeyNum = ordinal; AdsSetRelKeyPos = scrollbar thumb). The
    // skip-only walks above pass, but the grid paints via these — if they
    // return physical-recno positions instead of the active order's, the grid
    // does NOT reorder even though skip/INDEXORD changed. Find the name order.
    UNSIGNED16 nameOrd = 0;
    for (UNSIGNED16 ord = 1; ord <= 2; ++ord)
        if (seq_for_order(ord) == byName) nameOrd = ord;
    REQUIRE(nameOrd != 0);

    {
        ADSHANDLE hIdx = 0;
        REQUIRE(AdsGetIndexHandleByOrder(hTable, nameOrd, &hIdx) == 0);
        REQUIRE(AdsSetIndexOrderByHandle(hTable, hIdx) == 0);

        UNSIGNED32 kc = 0;
        REQUIRE(AdsGetKeyCount(hTable, 0, &kc) == 0);
        CHECK(kc == static_cast<UNSIGNED32>(N));

        // OrdKeyNo after landing on a row by recno (bookmark restore) must be
        // that row's SORTED position in the name order, not its physical recno.
        for (int k = 0; k < N; k += (N / 7)) {
            REQUIRE(AdsGotoRecord(hTable, byName[k]) == 0);
            UNSIGNED32 kn = 0;
            REQUIRE(AdsGetKeyNum(hTable, 0, &kn) == 0);
            CHECK(kn == static_cast<UNSIGNED32>(k + 1));
        }

        // Scrollbar thumb: fraction -> sorted position in the name order.
        REQUIRE(AdsSetRelKeyPos(hTable, 0.0) == 0);
        CHECK(recno_now(hTable) == byName[0]);
        REQUIRE(AdsSetRelKeyPos(hTable, 1.0) == 0);
        CHECK(recno_now(hTable) == byName[N - 1]);
    }

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}

TEST_CASE("multi-tag CDX via AdsOpenIndex array handles (full rddads path)") {
    auto dir = fs::temp_directory_path() / "openads_multitag_openidx";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 def[]   = "COD,C,6,0;NOM,C,6,0";
    UNSIGNED8 tname[] = "data";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsCreateTable(hConn, tname, nullptr, ADS_CDX,
                           0, 0, 0, 0, def, &hTable) == 0);

    const int N = 600;
    std::vector<std::string> cod(N), nom(N);
    for (int i = 0; i < N; ++i) {
        char b[16];
        std::snprintf(b, sizeof(b), "C%05d", (i * 53) % N);
        cod[i] = b;
        std::snprintf(b, sizeof(b), "N%05d", (i * 37) % N);
        nom[i] = b;
        REQUIRE(AdsAppendRecord(hTable) == 0);
        set_str(hTable, "COD", cod[i].c_str());
        set_str(hTable, "NOM", nom[i].c_str());
    }
    REQUIRE(AdsWriteRecord(hTable) == 0);
    make_tag(hTable, "TCOD", "COD");
    make_tag(hTable, "TNOM", "UPPER(NOM)");
    make_tag(hTable, "TCN",  "COD+NOM");
    make_tag_cond(hTable, "TF1", "COD", "NOM > 'N00300'");
    REQUIRE(AdsCloseTable(hTable) == 0);

    UNSIGNED8 onm[16] = "data";
    hTable = 0;
    REQUIRE(AdsOpenTable(hConn, onm, onm, ADS_CDX, 1, 1, 0, 1, &hTable) == 0);

    // rddads path: open the index bag -> array of per-tag handles; navigation
    // is then driven through those handles (pArea->hOrdCurrent).
    ADSHANDLE ah[64] = {0};
    UNSIGNED16 acount = 64;
    UNSIGNED8 idxf[16];
    std::memcpy(idxf, "data", 5);
    REQUIRE(AdsOpenIndex(hTable, idxf, ah, &acount) == 0);
    REQUIRE(acount >= 2);

    auto order_by = [&](const std::vector<std::string>& key) {
        std::vector<int> idx(N);
        std::iota(idx.begin(), idx.end(), 0);
        std::stable_sort(idx.begin(), idx.end(),
                         [&](int a, int b) { return key[a] < key[b]; });
        std::vector<UNSIGNED32> r(N);
        for (int k = 0; k < N; ++k) r[k] = static_cast<UNSIGNED32>(idx[k] + 1);
        return r;
    };
    const std::vector<UNSIGNED32> byName = order_by(nom);
    const std::vector<UNSIGNED32> byCode = order_by(cod);

    auto seq_h = [&](ADSHANDLE h) {
        REQUIRE(AdsSetIndexOrderByHandle(hTable, h) == 0);
        std::vector<UNSIGNED32> seq;
        REQUIRE(AdsGotoTop(h) == 0);
        UNSIGNED32 r = 0;
        AdsGetRecordNum(h, 0, &r);
        seq.push_back(r);
        for (int i = 1; i < N; ++i) {
            if (AdsSkip(h, 1) != 0) break;
            r = 0;
            AdsGetRecordNum(h, 0, &r);
            seq.push_back(r);
        }
        return seq;
    };

    bool found_name = false, found_code = false;
    for (UNSIGNED16 k = 0; k < acount; ++k) {
        if (ah[k] == 0) continue;
        auto seq = seq_h(ah[k]);
        if (seq == byName) found_name = true;
        if (seq == byCode) found_code = true;
    }
    CHECK(found_name);   // the name tag handle must navigate in name order
    CHECK(found_code);   // the code tag handle must navigate in code order

    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}
