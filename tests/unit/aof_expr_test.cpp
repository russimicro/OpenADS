// M-AOF.1 — parser + AST round-trip tests for the AOF filter
// expression subset. Each case parses a source string, formats the
// AST back via to_string() and asserts the textual round-trip is the
// canonical form. Anything outside the documented V1 grammar must
// return an error so the AOF surface can degrade to a full scan.

#include "doctest.h"
#include "engine/aof_expr.h"

#include <string>

using openads::engine::aof::parse;
using openads::engine::aof::to_string;

namespace {

std::string round_trip(const std::string& src) {
    auto r = parse(src);
    REQUIRE(r);
    return to_string(*r.value());
}

} // namespace

TEST_CASE("aof_expr parses a single equality leaf") {
    CHECK(round_trip("AGE = 30")    == "AGE = 30");
    CHECK(round_trip("age == 30")   == "age = 30");
    CHECK(round_trip("NAME = 'Bob'") == "NAME = 'Bob'");
    CHECK(round_trip("ACTIVE = .T.") == "ACTIVE = 1");
}

TEST_CASE("aof_expr accepts every comparison operator") {
    CHECK(round_trip("AGE != 30")  == "AGE != 30");
    CHECK(round_trip("AGE <> 30")  == "AGE != 30");
    CHECK(round_trip("AGE #  30")  == "AGE != 30");
    CHECK(round_trip("AGE <  30")  == "AGE < 30");
    CHECK(round_trip("AGE <= 30")  == "AGE <= 30");
    CHECK(round_trip("AGE >  30")  == "AGE > 30");
    CHECK(round_trip("AGE >= 30")  == "AGE >= 30");
}

TEST_CASE("aof_expr handles BETWEEN and IN leaves") {
    CHECK(round_trip("AGE BETWEEN 18 AND 65")
          == "AGE BETWEEN 18 AND 65");
    CHECK(round_trip("CITY IN ('NYC','LON','TOK')")
          == "CITY IN ('NYC', 'LON', 'TOK')");
}

TEST_CASE("aof_expr LIKE support") {
    CHECK(round_trip("NAME LIKE 'A%'")    == "NAME LIKE 'A%'");
    CHECK(round_trip("NAME LIKE '%bob%'") == "NAME LIKE '%bob%'");
    CHECK(round_trip("NAME LIKE 'A_B'")   == "NAME LIKE 'A_B'");
}

TEST_CASE("aof_expr IS NULL / IS NOT NULL") {
    CHECK(round_trip("FLAG IS NULL")          == "FLAG IS NULL");
    CHECK(round_trip("FLAG IS NOT NULL")      == "FLAG IS NOT NULL");
    CHECK(round_trip("NAME IS NULL .AND. AGE > 18")
          == "(NAME IS NULL AND AGE > 18)");
}

TEST_CASE("aof_expr combines leaves with AND / OR / NOT") {
    CHECK(round_trip("AGE > 18 .AND. AGE < 65")
          == "(AGE > 18 AND AGE < 65)");
    CHECK(round_trip("CITY = 'NYC' OR CITY = 'LON'")
          == "(CITY = 'NYC' OR CITY = 'LON')");
    CHECK(round_trip(".NOT. ACTIVE = .T.")
          == "NOT ACTIVE = 1");
    CHECK(round_trip("(A = 1 OR B = 2) AND C = 3")
          == "((A = 1 OR B = 2) AND C = 3)");
}

TEST_CASE("aof_expr rejects unsupported constructs") {
    // Function calls must fail so the AOF layer falls back to a
    // full-table scan; today V1 does not optimise function calls.
    CHECK_FALSE(parse("UPPER(NAME) = 'BOB'"));
    // Arithmetic on the field side is also out of scope V1.
    CHECK_FALSE(parse("AGE + 1 > 30"));
    // Trailing junk.
    CHECK_FALSE(parse("AGE = 30 GARBAGE"));
    // Missing operand.
    CHECK_FALSE(parse("AGE ="));
    CHECK_FALSE(parse(""));
    // BETWEEN without AND.
    CHECK_FALSE(parse("AGE BETWEEN 1 OR 5"));
    // Unterminated string.
    CHECK_FALSE(parse("NAME = 'oops"));
}

TEST_CASE("aof_expr precedence: AND binds tighter than OR") {
    // A=1 OR B=2 AND C=3 must parse as A=1 OR (B=2 AND C=3).
    CHECK(round_trip("A = 1 OR B = 2 AND C = 3")
          == "(A = 1 OR (B = 2 AND C = 3))");
}
