// Tests for trigger firing: AFTER/BEFORE/INSTEAD OF INSERT dispatch,
// priority ordering, disabled trigger, sp_DisableTriggers/sp_EnableTriggers,
// and the AdsDDCreateTrigger combined-type decode fix.

#include "doctest.h"
#include "openads/ace.h"
#include "openads/error.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

// Minimal DBF: one numeric field "ID" N(4), zero records.
fs::path make_orders_dbf(const fs::path& dir) {
    fs::create_directories(dir);
    auto p = dir / "orders.dbf";
    fs::remove(p);
    std::vector<std::uint8_t> f;
    std::array<std::uint8_t, 32> hdr{};
    hdr[0] = 0x03;
    const std::uint16_t hsize = 32 + 32 + 1;
    hdr[8] = hsize & 0xFF; hdr[9] = (hsize >> 8) & 0xFF;
    hdr[10] = 1 + 4;  // delete flag + 4-byte ID
    f.insert(f.end(), hdr.begin(), hdr.end());
    std::array<std::uint8_t, 32> fd{};
    std::strncpy(reinterpret_cast<char*>(fd.data()), "ID", 11);
    fd[11] = 'N'; fd[16] = 4; fd[17] = 0;
    f.insert(f.end(), fd.begin(), fd.end());
    f.push_back(0x0D);
    f.push_back(0x1A);
    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(f.data()), static_cast<std::streamsize>(f.size()));
    return p;
}

// Minimal DBF: one character field "FLAG" C(1) with one pre-populated record.
fs::path make_log_dbf(const fs::path& dir, char initial_flag) {
    fs::create_directories(dir);
    auto p = dir / "log.dbf";
    fs::remove(p);
    std::vector<std::uint8_t> f;
    std::array<std::uint8_t, 32> hdr{};
    hdr[0] = 0x03;
    hdr[4] = 1;  // 1 record
    const std::uint16_t hsize = 32 + 32 + 1;
    hdr[8] = hsize & 0xFF; hdr[9] = (hsize >> 8) & 0xFF;
    hdr[10] = 1 + 1;  // delete flag + 1-byte FLAG
    f.insert(f.end(), hdr.begin(), hdr.end());
    std::array<std::uint8_t, 32> fd{};
    std::strncpy(reinterpret_cast<char*>(fd.data()), "FLAG", 11);
    fd[11] = 'C'; fd[16] = 1;
    f.insert(f.end(), fd.begin(), fd.end());
    f.push_back(0x0D);
    f.push_back(' ');                                           // not deleted
    f.push_back(static_cast<std::uint8_t>(initial_flag));      // FLAG value
    f.push_back(0x1A);
    std::ofstream(p, std::ios::binary).write(
        reinterpret_cast<const char*>(f.data()), static_cast<std::streamsize>(f.size()));
    return p;
}

// Text .add with TABLE orders, TABLE log, plus caller-supplied TRIGGER lines.
// TRIGGER format: name=table;event_mask;timing;priority;enabled;container;proc;comment;options
// event_mask: 1=INSERT 2=UPDATE 3=DELETE   timing: 1=BEFORE 2=INSTEAD_OF 4=AFTER
fs::path make_fire_add(const fs::path& dir, const std::string& trigger_lines) {
    auto p = dir / "fire.add";
    std::ofstream f(p);
    f << "# OpenADS Data Dictionary v1\n"
      << "TABLE orders=orders.dbf\n"
      << "TABLE log=log.dbf\n"
      << trigger_lines;
    return p;
}

ADSHANDLE connect_dd(const fs::path& add_path) {
    ADSHANDLE h = 0;
    auto s = add_path.string();
    UNSIGNED8 buf[512];
    std::memcpy(buf, s.c_str(), s.size() + 1);
    AdsConnect60(buf, ADS_LOCAL_SERVER, nullptr, nullptr, 0, &h);
    return h;
}

ADSHANDLE open_alias(ADSHANDLE hConn, const char* alias) {
    ADSHANDLE h = 0;
    UNSIGNED8 name[32];
    std::strncpy(reinterpret_cast<char*>(name), alias, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    AdsOpenTable(hConn, name, name, ADS_CDX, 0, 0, 0, 0, &h);
    return h;
}

// Read FLAG field value from log record 1.
char read_log_flag(ADSHANDLE hLog) {
    if (AdsGotoRecord(hLog, 1) != 0) return '\0';
    UNSIGNED8 fld[8] = "FLAG";
    char buf[4] = {};
    UNSIGNED32 cap = sizeof(buf);
    AdsGetField(hLog, fld, reinterpret_cast<UNSIGNED8*>(buf), &cap, 0);
    return buf[0];
}

// Navigational INSERT into orders: append blank record and write.
UNSIGNED32 insert_orders_nav(ADSHANDLE hOrders) {
    if (AdsAppendRecord(hOrders) != 0) return 1;
    UNSIGNED8 fld[4] = "ID"; UNSIGNED8 val[8] = "   1";
    AdsSetString(hOrders, fld, val, 4);
    return AdsWriteRecord(hOrders);
}

// SQL INSERT into orders via AdsExecuteSQLDirect.
void insert_orders_sql(ADSHANDLE hConn) {
    ADSHANDLE hStmt = 0, hCur = 0;
    AdsCreateSQLStatement(hConn, &hStmt);
    AdsExecuteSQLDirect(hStmt,
        reinterpret_cast<UNSIGNED8*>(const_cast<char*>("INSERT INTO orders (id) VALUES (1)")),
        &hCur);
    if (hCur) AdsCloseTable(hCur);
    AdsCloseSQLStatement(hStmt);
}

// Execute a stored procedure call via SQL.
void exec_sp(ADSHANDLE hConn, const char* sql) {
    ADSHANDLE hStmt = 0, hCur = 0;
    AdsCreateSQLStatement(hConn, &hStmt);
    AdsExecuteSQLDirect(hStmt,
        reinterpret_cast<UNSIGNED8*>(const_cast<char*>(sql)),
        &hCur);
    if (hCur) AdsCloseTable(hCur);
    AdsCloseSQLStatement(hStmt);
}

}  // namespace


// ---------------------------------------------------------------------------
// AFTER INSERT fires and updates log
// ---------------------------------------------------------------------------
TEST_CASE("Trigger: AFTER INSERT fires") {
    auto dir = fs::temp_directory_path() / "openads_trig_after_ins";
    std::error_code ec;
    fs::remove_all(dir, ec);

    make_orders_dbf(dir);
    make_log_dbf(dir, '0');
    make_fire_add(dir,
        "TRIGGER trig_after=orders;1;4;10;1;UPDATE log SET flag = '1';;;0\n");

    ADSHANDLE hConn   = connect_dd(dir / "fire.add");
    REQUIRE(hConn != 0);
    ADSHANDLE hOrders = open_alias(hConn, "orders");
    ADSHANDLE hLog    = open_alias(hConn, "log");
    REQUIRE(hOrders != 0);
    REQUIRE(hLog    != 0);

    CHECK(read_log_flag(hLog) == '0');          // baseline
    CHECK(insert_orders_nav(hOrders) == 0);     // AFTER trigger fires here
    CHECK(read_log_flag(hLog) == '1');          // trigger updated log

    AdsCloseTable(hOrders);
    AdsCloseTable(hLog);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// BEFORE INSERT fires before DML; DML still proceeds
// ---------------------------------------------------------------------------
TEST_CASE("Trigger: BEFORE INSERT fires before DML") {
    auto dir = fs::temp_directory_path() / "openads_trig_before_ins";
    std::error_code ec;
    fs::remove_all(dir, ec);

    make_orders_dbf(dir);
    make_log_dbf(dir, '0');
    make_fire_add(dir,
        "TRIGGER trig_before=orders;1;1;10;1;UPDATE log SET flag = '1';;;0\n");

    ADSHANDLE hConn   = connect_dd(dir / "fire.add");
    REQUIRE(hConn != 0);
    ADSHANDLE hOrders = open_alias(hConn, "orders");
    ADSHANDLE hLog    = open_alias(hConn, "log");
    REQUIRE(hOrders != 0);
    REQUIRE(hLog    != 0);

    CHECK(insert_orders_nav(hOrders) == 0);
    CHECK(read_log_flag(hLog) == '1');          // BEFORE trigger ran

    // BEFORE must not suppress DML — record must exist
    UNSIGNED32 cnt = 0;
    AdsGetRecordCount(hOrders, 0, &cnt);
    CHECK(cnt == 1u);

    AdsCloseTable(hOrders);
    AdsCloseTable(hLog);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// INSTEAD OF INSERT suppresses DML (via SQL path) and runs body
// ---------------------------------------------------------------------------
TEST_CASE("Trigger: INSTEAD OF INSERT suppresses DML") {
    auto dir = fs::temp_directory_path() / "openads_trig_insteadof";
    std::error_code ec;
    fs::remove_all(dir, ec);

    make_orders_dbf(dir);
    make_log_dbf(dir, '0');
    make_fire_add(dir,
        "TRIGGER trig_io=orders;1;2;10;1;UPDATE log SET flag = '1';;;0\n");

    ADSHANDLE hConn   = connect_dd(dir / "fire.add");
    REQUIRE(hConn != 0);
    ADSHANDLE hOrders = open_alias(hConn, "orders");
    ADSHANDLE hLog    = open_alias(hConn, "log");
    REQUIRE(hOrders != 0);
    REQUIRE(hLog    != 0);

    insert_orders_sql(hConn);               // SQL INSERT fires INSTEAD OF

    CHECK(read_log_flag(hLog) == '1');      // trigger body ran

    // INSTEAD OF suppressed the actual write — orders stays empty
    UNSIGNED32 cnt = 0;
    AdsGetRecordCount(hOrders, 0, &cnt);
    CHECK(cnt == 0u);

    AdsCloseTable(hOrders);
    AdsCloseTable(hLog);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// Disabled trigger does not fire
// ---------------------------------------------------------------------------
TEST_CASE("Trigger: disabled trigger does not fire") {
    auto dir = fs::temp_directory_path() / "openads_trig_disabled";
    std::error_code ec;
    fs::remove_all(dir, ec);

    make_orders_dbf(dir);
    make_log_dbf(dir, '0');
    make_fire_add(dir,
        // enabled = 0
        "TRIGGER trig_dis=orders;1;4;10;0;UPDATE log SET flag = '1';;;0\n");

    ADSHANDLE hConn   = connect_dd(dir / "fire.add");
    REQUIRE(hConn != 0);
    ADSHANDLE hOrders = open_alias(hConn, "orders");
    ADSHANDLE hLog    = open_alias(hConn, "log");
    REQUIRE(hOrders != 0);
    REQUIRE(hLog    != 0);

    CHECK(insert_orders_nav(hOrders) == 0);
    CHECK(read_log_flag(hLog) == '0');      // trigger did NOT fire

    AdsCloseTable(hOrders);
    AdsCloseTable(hLog);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// Priority ordering: lower priority number fires first
// ---------------------------------------------------------------------------
TEST_CASE("Trigger: priority ordering") {
    auto dir = fs::temp_directory_path() / "openads_trig_priority";
    std::error_code ec;
    fs::remove_all(dir, ec);

    make_orders_dbf(dir);
    make_log_dbf(dir, '0');
    // trig_p1 (priority 1) sets flag='1'; trig_p2 (priority 2) sets flag='2'.
    // Priority 1 fires first → '1', then priority 2 fires → '2'. Final == '2'.
    make_fire_add(dir,
        "TRIGGER trig_p2=orders;1;4;2;1;UPDATE log SET flag = '2';;;0\n"
        "TRIGGER trig_p1=orders;1;4;1;1;UPDATE log SET flag = '1';;;0\n");

    ADSHANDLE hConn   = connect_dd(dir / "fire.add");
    REQUIRE(hConn != 0);
    ADSHANDLE hOrders = open_alias(hConn, "orders");
    ADSHANDLE hLog    = open_alias(hConn, "log");
    REQUIRE(hOrders != 0);
    REQUIRE(hLog    != 0);

    CHECK(insert_orders_nav(hOrders) == 0);
    CHECK(read_log_flag(hLog) == '2');      // p2 fired last (as expected)

    AdsCloseTable(hOrders);
    AdsCloseTable(hLog);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// sp_DisableTriggers / sp_EnableTriggers — CURRENT USER scope
// ---------------------------------------------------------------------------
TEST_CASE("Trigger: sp_DisableTriggers and sp_EnableTriggers CURRENT USER") {
    auto dir = fs::temp_directory_path() / "openads_trig_disable_cu";
    std::error_code ec;
    fs::remove_all(dir, ec);

    make_orders_dbf(dir);
    make_log_dbf(dir, '0');
    make_fire_add(dir,
        "TRIGGER trig_a=orders;1;4;10;1;UPDATE log SET flag = '1';;;0\n");

    ADSHANDLE hConn   = connect_dd(dir / "fire.add");
    REQUIRE(hConn != 0);
    ADSHANDLE hOrders = open_alias(hConn, "orders");
    ADSHANDLE hLog    = open_alias(hConn, "log");
    REQUIRE(hOrders != 0);
    REQUIRE(hLog    != 0);

    // Disable triggers for this connection
    exec_sp(hConn, "EXECUTE PROCEDURE sp_DisableTriggers('CURRENT USER')");

    CHECK(insert_orders_nav(hOrders) == 0);
    CHECK(read_log_flag(hLog) == '0');      // trigger did NOT fire

    // Re-enable
    exec_sp(hConn, "EXECUTE PROCEDURE sp_EnableTriggers('CURRENT USER')");

    CHECK(insert_orders_nav(hOrders) == 0);
    CHECK(read_log_flag(hLog) == '1');      // trigger now fires

    AdsCloseTable(hOrders);
    AdsCloseTable(hLog);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}

// ---------------------------------------------------------------------------
// AdsDDCreateTrigger decodes the combined ADS type constant correctly.
// Verifies the round-trip: ADS_AFTER_INSERT (0x0002) → GetTriggerProperty
// returns 0x0002, and the trigger actually fires on INSERT.
// ---------------------------------------------------------------------------
TEST_CASE("Trigger: AdsDDCreateTrigger + round-trip + fires") {
    auto dir = fs::temp_directory_path() / "openads_trig_create_fire";
    std::error_code ec;
    fs::remove_all(dir, ec);

    make_orders_dbf(dir);
    make_log_dbf(dir, '0');

    // Build a binary .add via AdsDDCreate
    auto add_path = (dir / "create.add").string();
    UNSIGNED8 add_buf[512];
    std::memcpy(add_buf, add_path.c_str(), add_path.size() + 1);
    ADSHANDLE hConn = 0;
    REQUIRE(AdsDDCreate(add_buf, 0, nullptr, &hConn) == 0);

    UNSIGNED8 ao[16] = "orders"; UNSIGNED8 po[32] = "orders.dbf";
    UNSIGNED8 al[16] = "log";    UNSIGNED8 pl[32] = "log.dbf";
    REQUIRE(AdsDDAddTable(hConn, ao, po, 0, 0, nullptr, nullptr) == 0);
    REQUIRE(AdsDDAddTable(hConn, al, pl, 0, 0, nullptr, nullptr) == 0);

    // SQL body in pucContainer (size > 4 triggers trigger_sql_body to use it)
    UNSIGNED8 tname[32]  = "trig_decode";
    UNSIGNED8 ttable[16] = "orders";
    UNSIGNED8 tbody[64];
    std::strncpy(reinterpret_cast<char*>(tbody),
                 "UPDATE log SET flag = '1'", sizeof(tbody));

    // ADS_AFTER_INSERT = 0x0002 — previously stored raw as event_mask=2, now decoded to event=1,timing=4
    REQUIRE(AdsDDCreateTrigger(hConn, tname, ttable,
                                ADS_AFTER_INSERT, 0, tbody, nullptr, 5) == 0);

    // Round-trip: GetTriggerProperty must return ADS_AFTER_INSERT (0x0002)
    UNSIGNED32 ev = 0; UNSIGNED16 len = 4;
    REQUIRE(AdsDDGetTriggerProperty(hConn, tname, ADS_DD_TRIGGER_EVENT, &ev, &len) == 0);
    CHECK(ev == ADS_AFTER_INSERT);

    // Open tables and verify trigger fires on INSERT
    ADSHANDLE hOrders = open_alias(hConn, "orders");
    ADSHANDLE hLog    = open_alias(hConn, "log");
    REQUIRE(hOrders != 0);
    REQUIRE(hLog    != 0);

    CHECK(insert_orders_nav(hOrders) == 0);
    CHECK(read_log_flag(hLog) == '1');      // trigger fired correctly

    AdsCloseTable(hOrders);
    AdsCloseTable(hLog);
    AdsDisconnect(hConn);
    fs::remove_all(dir, ec);
}
