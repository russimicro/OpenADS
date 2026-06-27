// tests/unit/index_expr_sql_test.cpp
// Tier-2 SQL push-down spike: try_emit_sql_where() translates the
// safely-translatable subset of an xBase FOR / SET FILTER predicate into a
// SQL WHERE fragment, and returns nullopt for anything it can't model (so the
// caller falls back to the row interpreter — push-down never changes results).
#include "doctest.h"
#include "engine/index_expr.h"

#include <optional>
#include <string>

using openads::engine::SqlDialect;
using openads::engine::try_emit_sql_where;

namespace {
std::string emit(const std::string& e) {
    auto r = try_emit_sql_where(e);
    REQUIRE(r.has_value());
    return *r;
}
}

TEST_CASE("SQL push-down: simple comparisons map to SQL operators") {
    CHECK(emit("QTY >= 100")        == "QTY >= 100");
    CHECK(emit("QTY > 5")           == "QTY > 5");
    CHECK(emit("X == 'a'")          == "X = 'a'");      // == -> =
    CHECK(emit("X = 'a'")           == "X = 'a'");
    CHECK(emit("QTY # 5")           == "QTY <> 5");     // xBase # -> <>
    CHECK(emit("QTY != 5")          == "QTY <> 5");     // xHarbour != -> <>
    CHECK(emit("QTY <> 5")          == "QTY <> 5");
    CHECK(emit("QTY <= 9")          == "QTY <= 9");
}

TEST_CASE("SQL push-down: logical operators and grouping") {
    CHECK(emit("QTY >= 100 .AND. NM = 'X'") == "(QTY >= 100 AND NM = 'X')");
    CHECK(emit("A = 1 .OR. B = 2")          == "(A = 1 OR B = 2)");
    CHECK(emit("(A = 1 .OR. B = 2) .AND. C = 3")
          == "((A = 1 OR B = 2) AND C = 3)");
    CHECK(emit(".NOT. A = 1")               == "(NOT A = 1)");
    CHECK(emit("! (A = 1)")                 == "(NOT A = 1)");
}

TEST_CASE("SQL push-down: scalar functions") {
    CHECK(emit("UPPER(NM) = 'ABC'")       == "UPPER(NM) = 'ABC'");
    CHECK(emit("LOWER(NM) = 'abc'")       == "LOWER(NM) = 'abc'");
    CHECK(emit("ALLTRIM(NM) = 'x'")       == "TRIM(NM) = 'x'");
    CHECK(emit("LTRIM(NM) = 'x'")         == "LTRIM(NM) = 'x'");
    CHECK(emit("SUBSTR(NM, 1, 3) = 'abc'") == "SUBSTR(NM, 1, 3) = 'abc'");
    CHECK(emit("SUBS(NM, 2) = 'b'")        == "SUBSTR(NM, 2) = 'b'");
    CHECK(emit("UPPER(ALLTRIM(NM)) = 'X'") == "UPPER(TRIM(NM)) = 'X'");
}

TEST_CASE("SQL push-down: '$' contains operator maps to LIKE") {
    CHECK(emit("'abc' $ NM")        == "NM LIKE '%abc%'");
    CHECK(emit("\"O'Brien\" $ NM")  == "NM LIKE '%O''Brien%'");  // quote escaped
    // Combined with other predicates.
    CHECK(emit("'x' $ NM .AND. QTY > 0") == "(NM LIKE '%x%' AND QTY > 0)");
}

TEST_CASE("SQL push-down: LEFT maps to SUBSTR") {
    CHECK(emit("LEFT(NM, 3) = 'abc'") == "SUBSTR(NM, 1, 3) = 'abc'");
}

TEST_CASE("SQL push-down: string literals are re-quoted and escaped") {
    // Double-quoted xBase string with an embedded single quote -> SQL
    // single-quoted with the quote doubled.
    CHECK(emit("NM = \"O'Brien\"") == "NM = 'O''Brien'");
    CHECK(emit("NM = 'plain'")     == "NM = 'plain'");
}

TEST_CASE("SQL push-down: string concatenation") {
    CHECK(emit("FIRST + LAST = 'AB'") == "(FIRST || LAST) = 'AB'");

    SqlDialect mysql;
    mysql.use_concat_fn = true;
    auto r = try_emit_sql_where("FIRST + LAST = 'AB'", mysql);
    REQUIRE(r.has_value());
    CHECK(*r == "CONCAT(FIRST, LAST) = 'AB'");
}

TEST_CASE("SQL push-down: dialect overrides") {
    SqlDialect ansi;
    ansi.substr_fn = "SUBSTRING";
    auto r = try_emit_sql_where("SUBSTR(NM, 1, 3) = 'x'", ansi);
    REQUIRE(r.has_value());
    CHECK(*r == "SUBSTRING(NM, 1, 3) = 'x'");
}

TEST_CASE("SQL push-down: ALIAS-> qualifiers are stripped") {
    CHECK(emit("CUST->QTY >= 100") == "QTY >= 100");
}

TEST_CASE("SQL push-down: untranslatable predicates decline to nullopt") {
    // RECNO()/DELETED(): no portable column form here -> fall back.
    CHECK_FALSE(try_emit_sql_where("RECNO() > 5").has_value());
    CHECK_FALSE(try_emit_sql_where("DELETED()").has_value());
    CHECK_FALSE(try_emit_sql_where(".NOT. DELETED()").has_value());
    // Unknown function.
    CHECK_FALSE(try_emit_sql_where("FOO(X) = 1").has_value());
    // '$' with a LIKE wildcard in the needle: xBase treats % as literal,
    // LIKE treats it as wildcard -> decline to avoid wrong results.
    CHECK_FALSE(try_emit_sql_where("'a%b' $ NM").has_value());
    CHECK_FALSE(try_emit_sql_where("'a_b' $ NM").has_value());
    // A bare scalar with no comparison can't be coerced to a SQL boolean.
    CHECK_FALSE(try_emit_sql_where("QTY").has_value());
    // Empty / whitespace.
    CHECK_FALSE(try_emit_sql_where("").has_value());
    CHECK_FALSE(try_emit_sql_where("   ").has_value());
}

TEST_CASE("SQL push-down: STR/VAL/DTOS now translatable") {
    // STR(n) -> CAST(n AS VARCHAR)
    CHECK(emit("STR(QTY) = '1'")       == "CAST(QTY AS VARCHAR) = '1'");
    CHECK(emit("STR(QTY,10) = 'x'")    == "CAST(QTY AS VARCHAR) = 'x'");
    CHECK(emit("STR(QTY,10,2) = 'x'")  == "CAST(QTY AS VARCHAR) = 'x'");
    // VAL(c) -> CAST(c AS DECIMAL)
    CHECK(emit("VAL(CODE) > 3")        == "CAST(CODE AS DECIMAL) > 3");
    CHECK(emit("VAL(AMT) >= 100")      == "CAST(AMT AS DECIMAL) >= 100");
    // DTOS(d) -> REPLACE(CAST(d AS VARCHAR),'-','')
    CHECK(emit("DTOS(DT) = '20260101'")
          == "REPLACE(CAST(DT AS VARCHAR), '-', '') = '20260101'");
}

TEST_CASE("SQL push-down: date/time functions") {
    CHECK(emit("YEAR(DT) = 2026")       == "EXTRACT(YEAR FROM DT) = 2026");
    CHECK(emit("MONTH(DT) = 6")         == "EXTRACT(MONTH FROM DT) = 6");
    CHECK(emit("DAY(DT) = 15")          == "EXTRACT(DAY FROM DT) = 15");
    CHECK(emit("DATE() = DT")           == "NOW() = DT");
    CHECK(emit("TIME() = TM")           == "CURRENT_TIME = TM");
    CHECK(emit("DATETIME() = DT")       == "NOW() = DT");
}

TEST_CASE("SQL push-down: conditional / type functions") {
    // Functions must appear in a comparison context (emit_cmp requires an operator)
    CHECK(emit("IIF(A,B,C) = 'X'")
          == "(CASE WHEN A THEN B ELSE C END) = 'X'");
    CHECK(emit("IF(X,Y,Z) = 'hello'")
          == "(CASE WHEN X THEN Y ELSE Z END) = 'hello'");
    CHECK(emit("ISNULL(A) = 1")
          == "(A IS NULL) = 1");
    CHECK(emit("EMPTY(A) = 1")
          == "(A IS NULL OR A = '' OR A = 0) = 1");
    CHECK(emit("LEN(NM) > 0")
          == "LENGTH(NM) > 0");
    CHECK(emit("ABS(QTY) > 5")
          == "ABS(QTY) > 5");
    CHECK(emit("INT(AMT) > 3")
          == "FLOOR(AMT) > 3");
}

TEST_CASE("SQL push-down: '$' field-to-field") {
    CHECK(emit("NM $ DESC")
          == "DESC LIKE ('%' || NM || '%')");
    // With MySQL CONCAT dialect
    SqlDialect mysql;
    mysql.use_concat_fn = true;
    auto r = try_emit_sql_where("NM $ DESC", mysql);
    REQUIRE(r.has_value());
    CHECK(*r == "DESC LIKE CONCAT('%', NM, '%')");
}

TEST_CASE("SQL push-down: $ with dialect LIKE overrides") {
    SqlDialect pg;
    pg.length_fn = "CHAR_LENGTH";
    pg.now_fn    = "NOW()";
    pg.true_literal  = "TRUE";
    pg.false_literal = "FALSE";
    auto r = try_emit_sql_where("DTOS(DT) = '20260101'", pg);
    REQUIRE(r.has_value());
    CHECK(*r == "REPLACE(CAST(DT AS VARCHAR), '-', '') = '20260101'");
    CHECK(emit("EMPTY(A) = 1")
          == "(A IS NULL OR A = '' OR A = 0) = 1");
    CHECK(emit("LEN(NM) > 0")   == "LENGTH(NM) > 0");
}
