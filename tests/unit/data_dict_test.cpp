#include "doctest.h"
#include "engine/data_dict.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;
using openads::engine::DataDict;

TEST_CASE("DataDict create + add_table + reopen + resolve") {
    auto p = fs::temp_directory_path() / "openads_m6_dd_basic.add";
    fs::remove(p);
    {
        auto created = DataDict::create(p.string());
        REQUIRE(created.has_value());
        DataDict dd = std::move(created).value();
        REQUIRE(dd.add_table("clientes", "clientes.dbf").has_value());
        REQUIRE(dd.add_table("ventas",   "v\\ventas.dbf").has_value());
    }
    {
        auto opened = DataDict::open(p.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK(dd.has_alias("clientes"));
        CHECK(dd.resolve("clientes")     == "clientes.dbf");
        CHECK(dd.resolve("ventas")       == "v\\ventas.dbf");
        CHECK(dd.resolve("not_an_alias") == "not_an_alias");
    }
    fs::remove(p);
}

// -------------------------------------------------------------------------
// Binary .add round-trip tests
// -------------------------------------------------------------------------

// Locate the pmsys.add fixture.
// Primary: testdata/pmsys/pmsys.add (authoritative test dataset).
// Fallback: tests/fixtures/adi/pmsys.add (legacy path, may not exist).
static fs::path pmsys_fixture() {
    auto primary = fs::path(__FILE__).parent_path().parent_path().parent_path()
                   / "testdata" / "pmsys" / "pmsys.add";
    if (fs::exists(primary)) return primary;
    return fs::path(__FILE__).parent_path().parent_path()
           / "fixtures" / "adi" / "pmsys.add";
}

TEST_CASE("DataDict open — ADS binary .add — reads Table entries") {
    // pmsys.add is the real proprietary ADS data dictionary from the PMSys
    // test dataset.  We only need to verify that the binary parser loads the
    // Table registry correctly; we do not mutate or re-save the file.
    auto fixture = pmsys_fixture();
    if (!fs::exists(fixture)) {
        WARN("pmsys.add fixture not found, skipping binary DD test");
        return;
    }
    auto opened = DataDict::open(fixture.string());
    REQUIRE(opened.has_value());
    DataDict dd = std::move(opened).value();

    CHECK(dd.has_alias("landlords"));
    CHECK(dd.resolve("landlords")   == ".\\landlords.adt");
    CHECK(dd.has_alias("leases"));
    CHECK(dd.resolve("leases")      == ".\\leases.adt");
    CHECK(dd.has_alias("properties"));
    CHECK(dd.has_alias("managers"));
    // Aliases that should NOT appear
    CHECK_FALSE(dd.has_alias("Database"));
    CHECK_FALSE(dd.has_alias("AutoTasks"));
}

// Copy pmsys.add to a writable temp path so mutations don't touch the source.
static fs::path stage_pmsys(const fs::path& dest_dir) {
    fs::create_directories(dest_dir);
    auto src = pmsys_fixture();
    auto dst = dest_dir / "pmsys.add";
    fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
    return dst;
}

TEST_CASE("DataDict binary .add — add_table round-trips through file") {
    auto fixture = pmsys_fixture();
    if (!fs::exists(fixture)) {
        WARN("pmsys.add fixture not found, skipping binary write test");
        return;
    }
    const auto dir = fs::temp_directory_path() / "openads_dd_bin_add";
    std::error_code ec;
    fs::remove_all(dir, ec);
    auto add_path = stage_pmsys(dir);

    // Add a new alias to the binary DD.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        REQUIRE(dd.add_table("newtable", ".\\newtable.adt").has_value());
    }

    // File must still start with the binary signature.
    {
        std::ifstream f(add_path, std::ios::binary);
        char sig[20] = {};
        f.read(sig, 20);
        CHECK(std::memcmp(sig, "ADS Data Dictionary", 19) == 0);
        CHECK(static_cast<unsigned char>(sig[19]) == 0x00);
    }

    // Reopen: new alias present, original aliases still intact.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK(dd.has_alias("newtable"));
        CHECK(dd.resolve("newtable") == ".\\newtable.adt");
        CHECK(dd.has_alias("landlords"));
        CHECK(dd.has_alias("leases"));
        CHECK(dd.has_alias("properties"));
    }

    fs::remove_all(dir, ec);
}

TEST_CASE("DataDict binary .add — remove_table round-trips through file") {
    auto fixture = pmsys_fixture();
    if (!fs::exists(fixture)) {
        WARN("pmsys.add fixture not found, skipping binary write test");
        return;
    }
    const auto dir = fs::temp_directory_path() / "openads_dd_bin_rm";
    std::error_code ec;
    fs::remove_all(dir, ec);
    auto add_path = stage_pmsys(dir);

    // Remove an existing alias.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        REQUIRE(dd.has_alias("landlords"));
        REQUIRE(dd.remove_table("landlords").has_value());
    }

    // File must still be binary.
    {
        std::ifstream f(add_path, std::ios::binary);
        char sig[20] = {};
        f.read(sig, 20);
        CHECK(std::memcmp(sig, "ADS Data Dictionary", 19) == 0);
    }

    // Reopen: removed alias gone, others intact.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK_FALSE(dd.has_alias("landlords"));
        CHECK(dd.has_alias("leases"));
        CHECK(dd.has_alias("properties"));
    }

    fs::remove_all(dir, ec);
}

TEST_CASE("DataDict binary .add — all original records preserved after mutation") {
    auto fixture = pmsys_fixture();
    if (!fs::exists(fixture)) {
        WARN("pmsys.add fixture not found, skipping binary write test");
        return;
    }
    const auto dir = fs::temp_directory_path() / "openads_dd_bin_preserve";
    std::error_code ec;
    fs::remove_all(dir, ec);
    auto add_path = stage_pmsys(dir);

    auto orig_size = fs::file_size(add_path);

    // Add a table — file must grow by exactly one record (524 bytes).
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        REQUIRE(dd.add_table("extra", ".\\extra.adt").has_value());
    }
    CHECK(fs::file_size(add_path) == orig_size + 524u);

    // Remove the added table — size stays the same (slot marked deleted, not reclaimed).
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        REQUIRE(dd.remove_table("extra").has_value());
    }
    CHECK(fs::file_size(add_path) == orig_size + 524u);

    // The original 32 Table aliases must all survive.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        for (auto* name : {"landlords", "leases", "properties", "managers",
                           "sys_registry"}) {
            CHECK(dd.has_alias(name));
        }
        CHECK_FALSE(dd.has_alias("extra"));
    }

    fs::remove_all(dir, ec);
}

TEST_CASE("DataDict binary .add — create_user / delete_user round-trip") {
    auto fixture = pmsys_fixture();
    if (!fs::exists(fixture)) {
        WARN("pmsys.add fixture not found, skipping binary write test");
        return;
    }
    const auto dir = fs::temp_directory_path() / "openads_dd_bin_user";
    std::error_code ec;
    fs::remove_all(dir, ec);
    auto add_path = stage_pmsys(dir);

    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        REQUIRE(dd.create_user("newuser_crud").has_value());
    }
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK(dd.has_user("newuser_crud"));
        REQUIRE(dd.delete_user("newuser_crud").has_value());
    }
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK_FALSE(dd.has_user("newuser_crud"));
        // File is still binary.
        std::ifstream f(add_path, std::ios::binary);
        char sig[20] = {};
        f.read(sig, 20);
        CHECK(std::memcmp(sig, "ADS Data Dictionary", 19) == 0);
    }

    fs::remove_all(dir, ec);
}

TEST_CASE("DataDict binary .add — create_group + add_user_to_group round-trip") {
    auto fixture = pmsys_fixture();
    if (!fs::exists(fixture)) {
        WARN("pmsys.add fixture not found, skipping binary group membership test");
        return;
    }
    const auto dir = fs::temp_directory_path() / "openads_dd_bin_grp";
    std::error_code ec;
    fs::remove_all(dir, ec);
    auto add_path = stage_pmsys(dir);

    // Add a new group and add an existing user to it.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        REQUIRE(dd.create_group("newgrp").has_value());
        REQUIRE(dd.add_user_to_group("user", "newgrp").has_value());
    }

    // Reopen: membership must persist via the Permission record we wrote.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK(dd.has_group("newgrp"));
        CHECK(dd.is_member_of("user", "newgrp"));
    }

    fs::remove_all(dir, ec);
}

TEST_CASE("DataDict binary .add — remove_user_from_group round-trip") {
    auto fixture = pmsys_fixture();
    if (!fs::exists(fixture)) {
        WARN("pmsys.add fixture not found, skipping binary group remove test");
        return;
    }
    const auto dir = fs::temp_directory_path() / "openads_dd_bin_grprm";
    std::error_code ec;
    fs::remove_all(dir, ec);
    auto add_path = stage_pmsys(dir);

    // Set up: create group and add user.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        REQUIRE(dd.create_group("tmpgrp").has_value());
        REQUIRE(dd.add_user_to_group("user", "tmpgrp").has_value());
    }

    // Verify member, then remove.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        REQUIRE(dd.is_member_of("user", "tmpgrp"));
        REQUIRE(dd.remove_user_from_group("user", "tmpgrp").has_value());
    }

    // Reopen: membership gone.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK_FALSE(dd.is_member_of("user", "tmpgrp"));
    }

    fs::remove_all(dir, ec);
}

TEST_CASE("DataDict binary .add — DB: built-in group membership round-trip") {
    // Verifies that add_user_to_group for DB: built-in groups (DB:Admin etc.)
    // persists across save/reload even though SAP .add files have no Group
    // records for these built-ins.  The fix auto-creates a Group record so the
    // Permission record has a valid target.
    auto fixture = pmsys_fixture();
    if (!fs::exists(fixture)) {
        WARN("pmsys.add fixture not found, skipping DB: built-in group test");
        return;
    }
    const auto dir = fs::temp_directory_path() / "openads_dd_bin_dbgrp";
    std::error_code ec;
    fs::remove_all(dir, ec);
    auto add_path = stage_pmsys(dir);

    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        // Add user-admin to DB:Admin without prior create_group("DB:Admin").
        REQUIRE(dd.add_user_to_group("user-admin", "DB:Admin").has_value());
        REQUIRE(dd.add_user_to_group("user-backup", "DB:Backup").has_value());
    }

    // Reopen: memberships and auto-created Group records must survive.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK(dd.has_group("DB:Admin"));
        CHECK(dd.has_group("DB:Backup"));
        CHECK(dd.is_member_of("user-admin", "DB:Admin"));
        CHECK(dd.is_member_of("user-backup", "DB:Backup"));
        // Second add_user_to_group must not duplicate the Group record.
        REQUIRE(dd.add_user_to_group("user-debug", "DB:Admin").has_value());
    }

    // Verify second membership added in the second session persists.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK(dd.is_member_of("user-admin", "DB:Admin"));
        CHECK(dd.is_member_of("user-debug", "DB:Admin"));
        CHECK(dd.is_member_of("user-backup", "DB:Backup"));
    }

    fs::remove_all(dir, ec);
}

TEST_CASE("DataDict binary .add — grant_permission round-trip") {
    // Verifies that grant_permission() persists fine-grained ACL bitmasks
    // to binary .add files and that they survive a save/reload cycle.
    // Also verifies that a second call for the same (grantee, object) pair
    // deactivates the prior record rather than accumulating duplicates.
    auto fixture = pmsys_fixture();
    if (!fs::exists(fixture)) {
        WARN("pmsys.add fixture not found, skipping grant_permission test");
        return;
    }
    const auto dir = fs::temp_directory_path() / "openads_dd_bin_perm";
    std::error_code ec;
    fs::remove_all(dir, ec);
    auto add_path = stage_pmsys(dir);

    // Grant SELECT-only (0x001) on "landlords" to "user-public".
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        REQUIRE(dd.grant_permission("Table", "landlords", "user-public", 0x001u).has_value());
    }

    // Reload: effective level must be 1 (read), not 4 (full from SAP sentinel).
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK(dd.get_effective_permission("user-public", "landlords") == 1);
    }

    // Upgrade to SELECT+UPDATE+INSERT (0x013) — second call must supersede first.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        REQUIRE(dd.grant_permission("Table", "landlords", "user-public", 0x013u).has_value());
    }

    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK(dd.get_effective_permission("user-public", "landlords") == 2);
    }

    // set_table_permission (level wrapper) must still work and also persist.
    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        REQUIRE(dd.set_table_permission("landlords", "user-admin", 3).has_value());
    }

    {
        auto opened = DataDict::open(add_path.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK(dd.get_effective_permission("user-admin", "landlords") == 3);
    }

    fs::remove_all(dir, ec);
}

TEST_CASE("DataDict binary .add — property-byte group memberships decoded") {
    // Verifies that groups stored in SAP-format User property bytes (not
    // Permission records) are correctly decoded by load_add_binary_().
    // pmsys root is in Administrators, Supervisors, General via property bytes.
    // Note: testuser/testgroup were removed from the fixture; root is used instead.
    auto fixture = pmsys_fixture();
    if (!fs::exists(fixture)) {
        WARN("pmsys.add fixture not found, skipping property-byte decode test");
        return;
    }
    auto opened = DataDict::open(fixture.string());
    REQUIRE(opened.has_value());
    DataDict dd = std::move(opened).value();

    // root → groups decoded from XOR property bytes (cross-validated slots).
    // Verified against AdsDDGetUserProperty(1102): root is in
    // Administrators, Supervisors, General, DB:Public, DB:Admin, DB:Backup, DB:Debug.
    CHECK(dd.is_member_of("root", "Administrators"));
    CHECK(dd.is_member_of("root", "Supervisors"));
    CHECK(dd.is_member_of("root", "General"));

    // RCB → XOR property-byte groups only (Permission records targeting groups
    // are stale orphans; AdsDDGetUserProperty(1102) confirms RCB is in:
    // Administrators, Supervisors, General, Internet, DB:Public, DB:Admin).
    // Internet is at slot 3 (skipped: single-user, ambiguous candidates).
    CHECK(dd.is_member_of("RCB", "General"));
    CHECK(dd.is_member_of("RCB", "Administrators"));
    CHECK(dd.is_member_of("RCB", "Supervisors"));
    // Stale Permission records (Tenants/Agents/Owners) must NOT produce memberships
    // — SAP's own property 1102 does not include these for RCB.
    CHECK_FALSE(dd.is_member_of("RCB", "Tenants"));
    CHECK_FALSE(dd.is_member_of("RCB", "Agents"));
    CHECK_FALSE(dd.is_member_of("RCB", "Owners"));
    // Ambiguous slot 3 must NOT assign the wrong group.
    CHECK_FALSE(dd.is_member_of("RCB", "Readonly"));

    // DB:Public — hardcoded for all users (SAP built-in, no record in .add)
    CHECK(dd.is_member_of("RCB", "DB:Public"));
    CHECK(dd.is_member_of("root", "DB:Public"));
    CHECK(dd.is_member_of("user-admin", "DB:Public"));
    CHECK(dd.is_member_of("user-public", "DB:Public"));
}

TEST_CASE("DataDict remove_table + reopen no longer has the alias") {
    auto p = fs::temp_directory_path() / "openads_m6_dd_remove.add";
    fs::remove(p);
    {
        auto created = DataDict::create(p.string());
        REQUIRE(created.has_value());
        DataDict dd = std::move(created).value();
        REQUIRE(dd.add_table("a", "a.dbf").has_value());
        REQUIRE(dd.add_table("b", "b.dbf").has_value());
        REQUIRE(dd.remove_table("a").has_value());
    }
    {
        auto opened = DataDict::open(p.string());
        REQUIRE(opened.has_value());
        DataDict dd = std::move(opened).value();
        CHECK_FALSE(dd.has_alias("a"));
        CHECK(dd.has_alias("b"));
    }
    fs::remove(p);
}
