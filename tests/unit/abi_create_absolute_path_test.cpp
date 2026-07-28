// Creating a table by its ABSOLUTE path, when that path already points into
// the connection's data directory, must write exactly there.
//
// Regression: Connection::resolve_table_file() honoured an absolute path only
// on OPEN; on CREATE it always stripped the root and re-joined the remainder
// under data_dir_, so `F:\z\BASES\` + `F:\z\BASES\T.DAT` resolved to
// `F:\z\BASES\z\BASES\T.DAT` -- a directory that does not exist. The create
// then failed with "ADT open for write failed" while AdsGetLastError() stayed
// 0, because no ACE call had failed. Every Harbour rddads caller doing
// COPY TO <full path> VIA "ADS" hit this.
//
// An absolute path is honoured on CREATE when the directory it names exists,
// which is what the real ACE engine does. When the directory does NOT exist --
// a client path that means nothing on this machine, which is the case the
// original guard was written for -- the fold under data_dir_ still applies, so
// the engine never writes outside the directory the server owns on the
// strength of a caller-supplied path.
#include "doctest.h"
#include "openads/ace.h"

#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

ADSHANDLE connect_to(const fs::path& dir) {
    UNSIGNED8 srv[260]{};
    const std::string s = dir.string();
    std::memcpy(srv, s.c_str(), s.size());
    ADSHANDLE h = 0;
    REQUIRE(AdsConnect60(srv, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &h)
            == AE_SUCCESS);
    return h;
}

void create_at(ADSHANDLE hConn, const std::string& name, UNSIGNED16 type) {
    std::vector<UNSIGNED8> tbl(name.begin(), name.end());
    tbl.push_back(0);
    UNSIGNED8 flddef[] = "Code,Character,10;Amount,Numeric,12";
    ADSHANDLE hTable   = 0;
    REQUIRE(AdsCreateTable(hConn, tbl.data(), nullptr, type, ADS_ANSI, 0, 0, 0,
                           flddef, &hTable) == AE_SUCCESS);
    CHECK(hTable != 0);
    AdsCloseTable(hTable);
}

}  // namespace

TEST_CASE("create by absolute path inside the data dir lands there (ADT)") {
    fs::path tmp = fs::temp_directory_path() / "openads_abs_create_adt";
    { std::error_code ec; fs::remove_all(tmp, ec); fs::create_directories(tmp, ec); }

    ADSHANDLE hConn = connect_to(tmp);

    // The ERP names its own company directory: the path is absolute AND it is
    // the very directory the connection was opened on.
    const fs::path target = tmp / "CONSE.DAT";
    create_at(hConn, target.string(), ADS_ADT);

    std::error_code ec;
    CHECK(fs::exists(target, ec));
    CHECK(fs::file_size(target, ec) > 0);

    // Nothing was written to a duplicated sub-path.
    const fs::path folded = tmp / target.relative_path();
    CHECK(!fs::exists(folded, ec));

    AdsDisconnect(hConn);
}

TEST_CASE("create by absolute path inside the data dir lands there (CDX)") {
    fs::path tmp = fs::temp_directory_path() / "openads_abs_create_cdx";
    { std::error_code ec; fs::remove_all(tmp, ec); fs::create_directories(tmp, ec); }

    ADSHANDLE hConn = connect_to(tmp);

    const fs::path target = tmp / "CONSE.DBF";
    create_at(hConn, target.string(), ADS_CDX);

    std::error_code ec;
    CHECK(fs::exists(target, ec));
    CHECK(fs::file_size(target, ec) > 0);

    AdsDisconnect(hConn);
}

TEST_CASE("create in a SUBDIRECTORY of the data dir by absolute path") {
    fs::path tmp = fs::temp_directory_path() / "openads_abs_create_sub";
    { std::error_code ec; fs::remove_all(tmp, ec);
      fs::create_directories(tmp / "DOC", ec); }

    ADSHANDLE hConn = connect_to(tmp);

    const fs::path target = tmp / "DOC" / "MOVI.DAT";
    create_at(hConn, target.string(), ADS_ADT);

    std::error_code ec;
    CHECK(fs::exists(target, ec));

    AdsDisconnect(hConn);
}

TEST_CASE("create by absolute path OUTSIDE the data dir, directory exists") {
    fs::path base = fs::temp_directory_path() / "openads_abs_create_out";
    fs::path data = base / "data";
    fs::path away = base / "elsewhere";
    { std::error_code ec; fs::remove_all(base, ec);
      fs::create_directories(data, ec);
      fs::create_directories(away, ec); }

    ADSHANDLE hConn = connect_to(data);

    // An application staging a work table outside the data directory names a
    // real directory, and the real ACE engine writes exactly there. Folding it
    // under data_dir_ produced intermediate directories nobody creates, so the
    // table the caller went on to open by that same absolute name did not
    // exist. Honour the path.
    const fs::path target = away / "SCRATCH.DBF";
    create_at(hConn, target.string(), ADS_CDX);

    std::error_code ec;
    CHECK(fs::exists(target, ec));
    CHECK(!fs::exists(data / target.relative_path(), ec));

    AdsDisconnect(hConn);
}

// A name rooted at the drive itself ("C:\STRAY.DBF") is also folded, even
// though the drive root exists -- see abi_create_table_test.cpp, which pins
// that case down. The root is never a deliberate destination for a table.

TEST_CASE("create by absolute path whose directory does NOT exist is folded") {
    fs::path base = fs::temp_directory_path() / "openads_abs_create_nodir";
    fs::path data = base / "data";
    { std::error_code ec; fs::remove_all(base, ec);
      fs::create_directories(data, ec); }

    ADSHANDLE hConn = connect_to(data);

    // A path that means nothing on this machine keeps the original guard: it
    // is re-rooted under the directory the server owns rather than written
    // wherever the client happened to point. Nothing creates the intermediate
    // directories, so the create does not succeed -- what matters is that the
    // engine never writes outside data_dir_ on the strength of a client path.
    const fs::path phantom = base / "no_such_dir" / "GHOST.DBF";
    std::vector<UNSIGNED8> tbl;
    { const std::string s = phantom.string();
      tbl.assign(s.begin(), s.end());
      tbl.push_back(0); }
    UNSIGNED8 flddef[] = "Code,Character,10;Amount,Numeric,12";
    ADSHANDLE hTable   = 0;
    CHECK(AdsCreateTable(hConn, tbl.data(), nullptr, ADS_CDX, ADS_ANSI, 0, 0, 0,
                         flddef, &hTable) != AE_SUCCESS);

    std::error_code ec;
    CHECK(!fs::exists(phantom, ec));

    AdsDisconnect(hConn);
}
