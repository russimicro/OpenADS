#include "doctest.h"

#include "sql_backend/uri.h"

using openads::sql_backend::SqliteUri;
using openads::sql_backend::parse_sqlite_uri;

TEST_CASE("sqlite URI: basic path extraction") {
    SqliteUri u;
    REQUIRE(parse_sqlite_uri("sqlite:///tmp/data.db", u));
    CHECK(u.path == "/tmp/data.db");
    CHECK(u.cipher_key.empty());
}

TEST_CASE("sqlite URI: Windows-style path") {
    SqliteUri u;
    REQUIRE(parse_sqlite_uri("sqlite://C:/Users/test/mydb.sqlite", u));
    CHECK(u.path == "C:/Users/test/mydb.sqlite");
    CHECK(u.cipher_key.empty());
}

TEST_CASE("sqlite URI: relative path") {
    SqliteUri u;
    REQUIRE(parse_sqlite_uri("sqlite://data/local.db", u));
    CHECK(u.path == "data/local.db");
}

TEST_CASE("sqlite URI: with cipher key") {
    SqliteUri u;
    REQUIRE(parse_sqlite_uri("sqlite:///tmp/db.sqlite?key=secret123", u));
    CHECK(u.path == "/tmp/db.sqlite");
    CHECK(u.cipher_key == "secret123");
}

TEST_CASE("sqlite URI: cipher key with URL-encoded characters") {
    SqliteUri u;
    REQUIRE(parse_sqlite_uri("sqlite:///tmp/db.sqlite?key=p%40ss%3Aword", u));
    CHECK(u.path == "/tmp/db.sqlite");
    CHECK(u.cipher_key == "p@ss:word");
}

TEST_CASE("sqlite URI: cipher key with plus for spaces") {
    SqliteUri u;
    REQUIRE(parse_sqlite_uri("sqlite:///tmp/db.sqlite?key=hello+world", u));
    CHECK(u.path == "/tmp/db.sqlite");
    CHECK(u.cipher_key == "hello world");
}

TEST_CASE("sqlite URI: multiple query params, only key is captured") {
    SqliteUri u;
    REQUIRE(parse_sqlite_uri("sqlite:///tmp/db.sqlite?key=abc&mode=ro", u));
    CHECK(u.path == "/tmp/db.sqlite");
    CHECK(u.cipher_key == "abc");
}

TEST_CASE("sqlite URI: query param without equals is skipped") {
    SqliteUri u;
    REQUIRE(parse_sqlite_uri("sqlite:///tmp/db.sqlite?nokey", u));
    CHECK(u.path == "/tmp/db.sqlite");
    CHECK(u.cipher_key.empty());
}

TEST_CASE("sqlite URI: rejects non-sqlite schemes") {
    SqliteUri u;
    CHECK_FALSE(parse_sqlite_uri("postgresql://host/db", u));
    CHECK_FALSE(parse_sqlite_uri("mariadb://host/db", u));
    CHECK_FALSE(parse_sqlite_uri("mysql://host/db", u));
    CHECK_FALSE(parse_sqlite_uri("odbc://Driver={x}", u));
    CHECK_FALSE(parse_sqlite_uri("/local/path", u));
}

TEST_CASE("sqlite URI: rejects empty string") {
    SqliteUri u;
    CHECK_FALSE(parse_sqlite_uri("", u));
}

TEST_CASE("sqlite URI: rejects empty path") {
    SqliteUri u;
    CHECK_FALSE(parse_sqlite_uri("sqlite://", u));
}

TEST_CASE("sqlite URI: rejects empty path with query") {
    SqliteUri u;
    CHECK_FALSE(parse_sqlite_uri("sqlite://?key=abc", u));
}

TEST_CASE("sqlite URI: hex-encoded special chars in key") {
    SqliteUri u;
    // %00 = null byte, %FF = 0xFF
    REQUIRE(parse_sqlite_uri("sqlite:///tmp/db?key=%00%FF", u));
    CHECK(u.path == "/tmp/db");
    CHECK(u.cipher_key.size() == 2);
    CHECK(static_cast<std::uint8_t>(u.cipher_key[0]) == 0x00);
    CHECK(static_cast<std::uint8_t>(u.cipher_key[1]) == 0xFF);
}

TEST_CASE("sqlite URI: percent-encoded slash in path stays as-is") {
    SqliteUri u;
    // parse_sqlite_uri does NOT decode the path, only the ?key= param
    REQUIRE(parse_sqlite_uri("sqlite:///tmp%2Fdata%2Fdb.sqlite", u));
    CHECK(u.path == "/tmp%2Fdata%2Fdb.sqlite");
}

TEST_CASE("sqlite URI: short prefix sqlite: is rejected") {
    SqliteUri u;
    CHECK_FALSE(parse_sqlite_uri("sqlite:", u));
}
