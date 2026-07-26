#include "doctest.h"
#include "platform/path.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;
using openads::platform::resolve_case_insensitive;

TEST_CASE("Case-insensitive resolve returns existing path verbatim") {
    const auto dir = fs::temp_directory_path() / "openads_path_t1";
    fs::create_directories(dir);
    const auto file = dir / "Clientes.dbf";
    { std::ofstream(file) << "x"; }

    auto resolved = resolve_case_insensitive((dir / "Clientes.dbf").string());
    CHECK(resolved == file.string());

    fs::remove_all(dir);
}

TEST_CASE("Case-insensitive resolve matches by case-folded leaf") {
    const auto dir = fs::temp_directory_path() / "openads_path_t2";
    fs::create_directories(dir);
    const auto file = dir / "Clientes.DBF";
    { std::ofstream(file) << "x"; }

    auto resolved = resolve_case_insensitive((dir / "clientes.dbf").string());
    CHECK(resolved == file.string());

    fs::remove_all(dir);
}

TEST_CASE("Case-insensitive resolve returns input on miss") {
    const auto dir = fs::temp_directory_path() / "openads_path_t3";
    fs::create_directories(dir);
    const auto missing = (dir / "Nope.dbf").string();

    auto resolved = resolve_case_insensitive(missing);
    CHECK(resolved == missing);

    fs::remove_all(dir);
}
