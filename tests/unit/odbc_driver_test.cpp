// Tests for the OpenADS ODBC driver (bindings/odbc/openads_odbc.cpp).
// Drives the driver's exported SQL* entry points directly (no Driver Manager,
// no registry) to prove a full SELECT round-trip against the native engine:
// connect via connection string -> CREATE/INSERT -> SELECT -> describe ->
// fetch -> SQLGetData.
#include "doctest.h"

#ifdef _WIN32
#  include <windows.h>
#endif
#include <sql.h>
#include <sqlext.h>

#include <cctype>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

static void exec_ok(SQLHDBC dbc, const char* sql) {
    SQLHSTMT st = SQL_NULL_HSTMT;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &st) == SQL_SUCCESS);
    REQUIRE(SQLExecDirect(st, reinterpret_cast<SQLCHAR*>(const_cast<char*>(sql)),
                          SQL_NTS) == SQL_SUCCESS);
    SQLFreeHandle(SQL_HANDLE_STMT, st);
}

TEST_CASE("openads ODBC driver: SELECT round-trip") {
    auto dir = fs::temp_directory_path() / "openads_odbc_test";
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    SQLHENV env = SQL_NULL_HENV;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env) == SQL_SUCCESS);
    REQUIRE(SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION,
                          reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0)
            == SQL_SUCCESS);

    SQLHDBC dbc = SQL_NULL_HDBC;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc) == SQL_SUCCESS);

    std::string cs = "DRIVER={OpenADS};DataDir=" + dir.string()
                   + ";ServerType=local";
    REQUIRE(SQLDriverConnect(dbc, nullptr,
                             reinterpret_cast<SQLCHAR*>(const_cast<char*>(cs.c_str())),
                             SQL_NTS, nullptr, 0, nullptr, SQL_DRIVER_NOPROMPT)
            == SQL_SUCCESS);

    exec_ok(dbc, "CREATE TABLE people (NAME Character(20), AGE Numeric(3,0))");
    exec_ok(dbc, "INSERT INTO people (NAME, AGE) VALUES ('alice', 30)");
    exec_ok(dbc, "INSERT INTO people (NAME, AGE) VALUES ('bob', 41)");

    SQLHSTMT st = SQL_NULL_HSTMT;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &st) == SQL_SUCCESS);
    REQUIRE(SQLExecDirect(st,
            reinterpret_cast<SQLCHAR*>(const_cast<char*>("SELECT NAME, AGE FROM people")),
            SQL_NTS) == SQL_SUCCESS);

    SQLSMALLINT ncols = 0;
    REQUIRE(SQLNumResultCols(st, &ncols) == SQL_SUCCESS);
    CHECK(ncols == 2);

    SQLCHAR cname[64] = {0};
    SQLSMALLINT nlen = 0, dtype = 0, ddec = 0, dnull = 0;
    SQLULEN dsize = 0;
    REQUIRE(SQLDescribeCol(st, 1, cname, sizeof(cname), &nlen,
                           &dtype, &dsize, &ddec, &dnull) == SQL_SUCCESS);
    CHECK(std::string(reinterpret_cast<char*>(cname)) == "NAME");

    int rows = 0;
    while (SQLFetch(st) == SQL_SUCCESS) {
        SQLCHAR val[64] = {0};
        SQLLEN ind = 0;
        REQUIRE(SQLGetData(st, 1, SQL_C_CHAR, val, sizeof(val), &ind)
                == SQL_SUCCESS);
        CHECK(ind > 0);
        ++rows;
    }
    CHECK(rows == 2);
    CHECK(SQLFetch(st) == SQL_NO_DATA);

    SQLFreeHandle(SQL_HANDLE_STMT, st);

    // --- catalog: SQLTables lists our table ---
    SQLHSTMT cat = SQL_NULL_HSTMT;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &cat) == SQL_SUCCESS);
    REQUIRE(SQLTables(cat, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0)
            == SQL_SUCCESS);
    bool found_people = false;
    while (SQLFetch(cat) == SQL_SUCCESS) {
        SQLCHAR tn[128] = {0};
        SQLLEN ind = 0;
        REQUIRE(SQLGetData(cat, 3, SQL_C_CHAR, tn, sizeof(tn), &ind)
                == SQL_SUCCESS);
        std::string name(reinterpret_cast<char*>(tn));
        for (char& c : name) c = static_cast<char>(std::tolower((unsigned char)c));
        if (name.find("people") != std::string::npos) found_people = true;
    }
    CHECK(found_people);
    SQLFreeHandle(SQL_HANDLE_STMT, cat);

    // --- catalog: SQLColumns lists NAME and AGE for 'people' ---
    SQLHSTMT colh = SQL_NULL_HSTMT;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &colh) == SQL_SUCCESS);
    REQUIRE(SQLColumns(colh, nullptr, 0, nullptr, 0,
                       reinterpret_cast<SQLCHAR*>(const_cast<char*>("people")),
                       SQL_NTS, nullptr, 0) == SQL_SUCCESS);
    int colcount = 0;
    bool has_name = false;
    while (SQLFetch(colh) == SQL_SUCCESS) {
        SQLCHAR cn[128] = {0};
        SQLLEN ind = 0;
        REQUIRE(SQLGetData(colh, 4, SQL_C_CHAR, cn, sizeof(cn), &ind)
                == SQL_SUCCESS);
        if (std::string(reinterpret_cast<char*>(cn)) == "NAME") has_name = true;
        ++colcount;
    }
    CHECK(colcount == 2);
    CHECK(has_name);
    SQLFreeHandle(SQL_HANDLE_STMT, colh);

    REQUIRE(SQLDisconnect(dbc) == SQL_SUCCESS);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
}

// Connect helper for the typed/scroll cases: returns a live env+dbc on a fresh
// data dir. Mirrors the round-trip case's connect sequence.
static void connect_fresh(const char* sub, SQLHENV* env, SQLHDBC* dbc) {
    auto dir = fs::temp_directory_path() / sub;
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);

    REQUIRE(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, env) == SQL_SUCCESS);
    REQUIRE(SQLSetEnvAttr(*env, SQL_ATTR_ODBC_VERSION,
                          reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0)
            == SQL_SUCCESS);
    REQUIRE(SQLAllocHandle(SQL_HANDLE_DBC, *env, dbc) == SQL_SUCCESS);
    std::string cs = "DRIVER={OpenADS};DataDir=" + dir.string()
                   + ";ServerType=local";
    REQUIRE(SQLDriverConnect(*dbc, nullptr,
                             reinterpret_cast<SQLCHAR*>(const_cast<char*>(cs.c_str())),
                             SQL_NTS, nullptr, 0, nullptr, SQL_DRIVER_NOPROMPT)
            == SQL_SUCCESS);
}

TEST_CASE("openads ODBC driver: typed describe + typed SQLGetData") {
    SQLHENV env = SQL_NULL_HENV;
    SQLHDBC dbc = SQL_NULL_HDBC;
    connect_fresh("openads_odbc_typed", &env, &dbc);

    exec_ok(dbc, "CREATE TABLE nums (NAME Character(20), AGE Numeric(3,0))");
    exec_ok(dbc, "INSERT INTO nums (NAME, AGE) VALUES ('alice', 30)");
    exec_ok(dbc, "INSERT INTO nums (NAME, AGE) VALUES ('bob', 41)");

    SQLHSTMT st = SQL_NULL_HSTMT;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &st) == SQL_SUCCESS);
    REQUIRE(SQLExecDirect(st,
            reinterpret_cast<SQLCHAR*>(const_cast<char*>("SELECT NAME, AGE FROM nums")),
            SQL_NTS) == SQL_SUCCESS);

    // AGE describes as a numeric SQL type, not the everything-is-char default.
    SQLCHAR cn[64] = {0};
    SQLSMALLINT nlen = 0, dtype = 0, ddec = 0, dnull = 0;
    SQLULEN dsize = 0;
    REQUIRE(SQLDescribeCol(st, 2, cn, sizeof(cn), &nlen,
                           &dtype, &dsize, &ddec, &dnull) == SQL_SUCCESS);
    CHECK(std::string(reinterpret_cast<char*>(cn)) == "AGE");
    CHECK(dtype == SQL_NUMERIC);

    // NAME still describes as a character type.
    SQLSMALLINT ntype = 0;
    REQUIRE(SQLDescribeCol(st, 1, cn, sizeof(cn), &nlen,
                           &ntype, &dsize, &ddec, &dnull) == SQL_SUCCESS);
    CHECK(ntype == SQL_VARCHAR);

    // First row: AGE retrieved as a real integer via SQL_C_LONG.
    REQUIRE(SQLFetch(st) == SQL_SUCCESS);
    SQLINTEGER age = 0;
    SQLLEN ind = 0;
    REQUIRE(SQLGetData(st, 2, SQL_C_LONG, &age, sizeof(age), &ind) == SQL_SUCCESS);
    CHECK(age == 30);
    CHECK(ind == static_cast<SQLLEN>(sizeof(SQLINTEGER)));

    // Char retrieval on the same row still works.
    SQLCHAR nm[32] = {0};
    REQUIRE(SQLGetData(st, 1, SQL_C_CHAR, nm, sizeof(nm), &ind) == SQL_SUCCESS);
    CHECK(std::string(reinterpret_cast<char*>(nm)) == "alice");

    SQLFreeHandle(SQL_HANDLE_STMT, st);
    REQUIRE(SQLDisconnect(dbc) == SQL_SUCCESS);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
}

// Read column 1 of the current row as text (helper for the scroll case).
static std::string get_str1(SQLHSTMT st) {
    SQLCHAR buf[32] = {0};
    SQLLEN ind = 0;
    REQUIRE(SQLGetData(st, 1, SQL_C_CHAR, buf, sizeof(buf), &ind) == SQL_SUCCESS);
    return std::string(reinterpret_cast<char*>(buf));
}

TEST_CASE("openads ODBC driver: scrollable cursor (SQLFetchScroll)") {
    SQLHENV env = SQL_NULL_HENV;
    SQLHDBC dbc = SQL_NULL_HDBC;
    connect_fresh("openads_odbc_scroll", &env, &dbc);

    exec_ok(dbc, "CREATE TABLE seq (NM Character(2))");
    for (int i = 1; i <= 5; ++i) {
        char ins[64];
        std::snprintf(ins, sizeof(ins),
                      "INSERT INTO seq (NM) VALUES ('0%d')", i);
        exec_ok(dbc, ins);
    }

    SQLHSTMT st = SQL_NULL_HSTMT;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &st) == SQL_SUCCESS);
    REQUIRE(SQLExecDirect(st,
            reinterpret_cast<SQLCHAR*>(const_cast<char*>("SELECT NM FROM seq")),
            SQL_NTS) == SQL_SUCCESS);

    REQUIRE(SQLFetchScroll(st, SQL_FETCH_LAST, 0) == SQL_SUCCESS);
    CHECK(get_str1(st) == "05");

    REQUIRE(SQLFetchScroll(st, SQL_FETCH_FIRST, 0) == SQL_SUCCESS);
    CHECK(get_str1(st) == "01");

    REQUIRE(SQLFetchScroll(st, SQL_FETCH_ABSOLUTE, 3) == SQL_SUCCESS);
    CHECK(get_str1(st) == "03");

    REQUIRE(SQLFetchScroll(st, SQL_FETCH_PRIOR, 0) == SQL_SUCCESS);
    CHECK(get_str1(st) == "02");

    REQUIRE(SQLFetchScroll(st, SQL_FETCH_NEXT, 0) == SQL_SUCCESS);
    CHECK(get_str1(st) == "03");

    REQUIRE(SQLFetchScroll(st, SQL_FETCH_RELATIVE, 2) == SQL_SUCCESS);
    CHECK(get_str1(st) == "05");

    // Past the last row → SQL_NO_DATA.
    CHECK(SQLFetchScroll(st, SQL_FETCH_NEXT, 0) == SQL_NO_DATA);

    // Plain SQLFetch still advances forward from the top after a re-exec.
    REQUIRE(SQLExecDirect(st,
            reinterpret_cast<SQLCHAR*>(const_cast<char*>("SELECT NM FROM seq")),
            SQL_NTS) == SQL_SUCCESS);
    REQUIRE(SQLFetch(st) == SQL_SUCCESS);
    CHECK(get_str1(st) == "01");

    SQLFreeHandle(SQL_HANDLE_STMT, st);
    REQUIRE(SQLDisconnect(dbc) == SQL_SUCCESS);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
}

TEST_CASE("openads ODBC driver: SQLBindParameter (positional ?)") {
    SQLHENV env = SQL_NULL_HENV;
    SQLHDBC dbc = SQL_NULL_HDBC;
    connect_fresh("openads_odbc_param", &env, &dbc);

    exec_ok(dbc, "CREATE TABLE pp (ID Numeric(4,0), NM Character(10))");

    // Parameterised INSERT: prepare once, bind, execute twice with new values
    // (the bound buffers are read at execute time).
    SQLHSTMT st = SQL_NULL_HSTMT;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &st) == SQL_SUCCESS);
    REQUIRE(SQLPrepare(st,
            reinterpret_cast<SQLCHAR*>(const_cast<char*>(
                "INSERT INTO pp (ID, NM) VALUES (?, ?)")), SQL_NTS) == SQL_SUCCESS);

    SQLINTEGER id = 7;
    SQLCHAR    nm[16] = "hello";
    SQLLEN     nmind = SQL_NTS;
    REQUIRE(SQLBindParameter(st, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &id, 0, nullptr) == SQL_SUCCESS);
    REQUIRE(SQLBindParameter(st, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                             10, 0, nm, sizeof(nm), &nmind) == SQL_SUCCESS);
    REQUIRE(SQLExecute(st) == SQL_SUCCESS);

    id = 9;
    std::strcpy(reinterpret_cast<char*>(nm), "world");
    REQUIRE(SQLExecute(st) == SQL_SUCCESS);
    SQLFreeHandle(SQL_HANDLE_STMT, st);

    // Parameterised SELECT ... WHERE ID = ? returns only the matching row.
    SQLHSTMT q = SQL_NULL_HSTMT;
    REQUIRE(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &q) == SQL_SUCCESS);
    REQUIRE(SQLPrepare(q,
            reinterpret_cast<SQLCHAR*>(const_cast<char*>(
                "SELECT NM FROM pp WHERE ID = ?")), SQL_NTS) == SQL_SUCCESS);
    SQLINTEGER want = 9;
    REQUIRE(SQLBindParameter(q, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER,
                             0, 0, &want, 0, nullptr) == SQL_SUCCESS);
    REQUIRE(SQLExecute(q) == SQL_SUCCESS);
    REQUIRE(SQLFetch(q) == SQL_SUCCESS);
    CHECK(get_str1(q) == "world");
    CHECK(SQLFetch(q) == SQL_NO_DATA);
    SQLFreeHandle(SQL_HANDLE_STMT, q);

    REQUIRE(SQLDisconnect(dbc) == SQL_SUCCESS);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
}
