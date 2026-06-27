#include "doctest.h"
#include "openads/ace.h"

#include <cstdint>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

// Walk the active order from top to EOF, collecting the CODIGO value of
// every entry the index yields. The FOR clause is meant to keep only
// CODIGO>100 in the tag, so a correct conditional index returns exactly
// 100 entries with the minimum CODIGO == 101.
std::vector<int> walk_codigo(ADSHANDLE hTable) {
    std::vector<int> out;
    REQUIRE(AdsGotoTop(hTable) == 0);
    UNSIGNED16 at_eof = 0;
    REQUIRE(AdsAtEOF(hTable, &at_eof) == 0);
    while (!at_eof) {
        double dv = 0.0;
        UNSIGNED8 f[8] = "CODIGO";
        REQUIRE(AdsGetDouble(hTable, f, &dv) == 0);
        out.push_back(static_cast<int>(dv + 0.5));
        REQUIRE(AdsSkip(hTable, 1) == 0);
        REQUIRE(AdsAtEOF(hTable, &at_eof) == 0);
    }
    return out;
}

} // namespace

TEST_CASE("CDX conditional tag (FOR) survives AdsReindex") {
    auto dir = fs::temp_directory_path() / "openads_cdx_cond_for";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    UNSIGNED8 srv[256];
    std::memcpy(srv, dir.string().c_str(), dir.string().size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER,
                         nullptr, nullptr, 0, &hConn) == 0);

    UNSIGNED8 name[64]    = "cond";
    UNSIGNED8 alias[64]   = "cond";
    UNSIGNED8 fields[128] = "CODIGO,Numeric,6,0;NOME,Character,20";
    ADSHANDLE hTable = 0;
    REQUIRE(AdsCreateTable(hConn, name, alias, ADS_CDX, 0, 0, 0, 64,
                           fields, &hTable) == 0);

    // Append CODIGO = 1..200.
    for (int i = 1; i <= 200; ++i) {
        REQUIRE(AdsAppendRecord(hTable) == 0);
        UNSIGNED8 fCod[8] = "CODIGO";
        REQUIRE(AdsSetDouble(hTable, fCod, static_cast<double>(i)) == 0);
        UNSIGNED8 fNome[8] = "NOME";
        std::string v = "n" + std::to_string(i);
        REQUIRE(AdsSetString(hTable, fNome,
                             reinterpret_cast<UNSIGNED8*>(v.data()),
                             static_cast<UNSIGNED32>(v.size())) == 0);
        REQUIRE(AdsWriteRecord(hTable) == 0);
    }

    // Conditional tag: key CODIGO, FOR CODIGO>100.
    UNSIGNED8 idxfile[16] = "cond";
    UNSIGNED8 idxname[16] = "TF";
    UNSIGNED8 expr[16]    = "CODIGO";
    UNSIGNED8 cond[16]    = "CODIGO>100";
    ADSHANDLE hIdx = 0;
    REQUIRE(AdsCreateIndex61(hTable, idxfile, idxname, expr,
                             cond, nullptr, 0, 512, &hIdx) == 0);

    REQUIRE(AdsSetIndexOrderByHandle(hTable, hIdx) == 0);

    // Before reindex: tag must hold only CODIGO>100.
    {
        auto vals = walk_codigo(hTable);
        CHECK(vals.size() == 100u);
        if (!vals.empty()) {
            int mn = vals[0];
            for (int v : vals) if (v < mn) mn = v;
            CHECK(mn == 101);
        }
    }

    // Reindex must NOT lose the FOR predicate.
    REQUIRE(AdsReindex(hTable) == 0);

    // After reindex: tag must STILL hold only CODIGO>100.
    {
        auto vals = walk_codigo(hTable);
        CHECK(vals.size() == 100u);
        if (!vals.empty()) {
            int mn = vals[0];
            for (int v : vals) if (v < mn) mn = v;
            CHECK(mn == 101);
        }
    }

    REQUIRE(AdsCloseIndex(hIdx) == 0);
    REQUIRE(AdsCloseTable(hTable) == 0);
    REQUIRE(AdsDisconnect(hConn) == 0);
    fs::remove_all(dir, ec);
}
