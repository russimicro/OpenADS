#include "doctest.h"
#include "platform/path.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;
using openads::platform::resolve_case_insensitive;
using openads::platform::resolve_under_root;

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

TEST_CASE("resolve_under_root keeps relative paths inside jail") {
    const auto root = fs::temp_directory_path() / "openads_path_jail";
    const auto sub  = root / "data";
    fs::create_directories(sub);

    auto resolved = resolve_under_root(root.string(), "data");
    REQUIRE(resolved.has_value());
    CHECK(fs::path(*resolved).filename() == "data");

    fs::remove_all(root);
}

TEST_CASE("resolve_under_root rejects parent traversal") {
    const auto root = fs::temp_directory_path() / "openads_path_escape";
    fs::create_directories(root);

    auto resolved = resolve_under_root(root.string(), "..");
    CHECK_FALSE(resolved.has_value());

    fs::remove_all(root);
}
