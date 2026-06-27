// tests/unit/backend_tier1_test.cpp
// Unit tests for the Tier 1 SQLRDD-inspired utilities:
//   - BackendTxManager (nested transactions, auto-commit threshold)
//   - BackendFieldOptimizer (lazy field loading, learning)
//   - BackendWhereBuilder (restrictor composition)

#include "doctest.h"
#include "sql_backend/backend_tx_manager.h"
#include "sql_backend/backend_field_optimizer.h"
#include "sql_backend/backend_where_builder.h"

using openads::sql_backend::BackendTxManager;
using openads::sql_backend::BackendFieldOptimizer;
using openads::sql_backend::BackendWhereBuilder;

// ═══════════════════════════════════════════════════════════════════
// BackendTxManager
// ═══════════════════════════════════════════════════════════════════

TEST_CASE("BackendTxManager: basic begin/commit cycle") {
    BackendTxManager tx;
    int begin_count = 0, commit_count = 0, rollback_count = 0;
    tx.on_begin   = [&](bool) { ++begin_count; };
    tx.on_commit  = [&](bool) { ++commit_count; };
    tx.on_rollback = [&]() { ++rollback_count; };

    tx.begin();
    CHECK(tx.in_transaction());
    CHECK(tx.nesting == 1);

    tx.commit();
    CHECK_FALSE(tx.in_transaction());
    CHECK(tx.nesting == 0);
    CHECK(begin_count == 1);
    CHECK(commit_count == 1);
}

TEST_CASE("BackendTxManager: nested transactions") {
    BackendTxManager tx;
    int begin_count = 0, commit_count = 0, release_count = 0;
    tx.on_begin   = [&](bool) { ++begin_count; };
    tx.on_commit  = [&](bool) { ++commit_count; };
    tx.on_release_savepoint = [&](const std::string&) { ++release_count; };

    tx.begin();  // nesting = 1
    CHECK(tx.nesting == 1);

    tx.begin();  // nesting = 2 (SAVEPOINT)
    CHECK(tx.nesting == 2);

    tx.commit(); // nesting = 1 (release savepoint)
    CHECK(tx.nesting == 1);
    CHECK(release_count == 1);
    CHECK(commit_count == 0);

    tx.commit(); // nesting = 0 (actual COMMIT)
    CHECK(tx.nesting == 0);
    CHECK(commit_count == 1);
}

TEST_CASE("BackendTxManager: rollback") {
    BackendTxManager tx;
    int rollback_count = 0;
    tx.on_rollback = [&]() { ++rollback_count; };

    tx.begin();
    tx.begin();
    tx.rollback();  // nested: rollback to savepoint
    CHECK(tx.nesting == 1);
    CHECK(rollback_count == 0);

    tx.rollback();  // top-level: actual rollback
    CHECK(tx.nesting == 0);
    CHECK(rollback_count == 1);
    CHECK_FALSE(tx.dirty);
}

TEST_CASE("BackendTxManager: auto-commit threshold") {
    BackendTxManager tx;
    int commit_count = 0;
    tx.on_commit = [&](bool) { ++commit_count; };
    tx.auto_commit_threshold = 3;

    tx.begin();
    tx.dirty = true;  // must be dirty for auto-commit to fire
    CHECK_FALSE(tx.note_dml());  // count=1
    CHECK_FALSE(tx.note_dml());  // count=2
    CHECK(tx.note_dml());        // count=3, auto-commit fires
    CHECK(commit_count == 1);
    CHECK_FALSE(tx.in_transaction());
}

TEST_CASE("BackendTxManager: dirty flag") {
    BackendTxManager tx;
    tx.on_commit = [](bool) {};
    tx.begin();
    CHECK_FALSE(tx.dirty);
    tx.dirty = true;
    tx.commit();
    CHECK_FALSE(tx.dirty);
}

TEST_CASE("BackendTxManager: commit returns true at nesting=0") {
    BackendTxManager tx;
    tx.on_commit = [](bool) {};
    tx.begin();
    tx.begin();
    CHECK_FALSE(tx.commit());  // nested, returns false
    CHECK(tx.commit());        // top-level, returns true
}

// ═══════════════════════════════════════════════════════════════════
// BackendFieldOptimizer
// ═══════════════════════════════════════════════════════════════════

TEST_CASE("BackendFieldOptimizer: starts in Learning state") {
    BackendFieldOptimizer opt;
    CHECK(opt.status == BackendFieldOptimizer::FieldListStatus::Learning);
    CHECK(opt.is_all_columns());
    CHECK(opt.select_fragment() == "*");
}

TEST_CASE("BackendFieldOptimizer: tracks single column reads") {
    BackendFieldOptimizer opt;
    auto& list = opt.note_column_read("NAME");
    CHECK(list.empty());  // still learning
    CHECK(opt.single_col_reads == 1);
    CHECK(opt.select_fragment() == "*");  // one column isn't enough

    // Read the same column again — doesn't increment single_col_reads
    opt.note_column_read("NAME");
    CHECK(opt.single_col_reads == 1);
}

TEST_CASE("BackendFieldOptimizer: switches to SELECT * after threshold") {
    BackendFieldOptimizer opt;
    opt.note_column_read("COL_A");
    opt.note_column_read("COL_B");
    opt.note_column_read("COL_C");
    opt.note_column_read("COL_D");
    auto& list = opt.note_column_read("COL_E");
    CHECK(opt.status == BackendFieldOptimizer::FieldListStatus::Stable);
    CHECK(list.empty());  // SELECT *
    CHECK(opt.is_all_columns());
    CHECK(opt.select_fragment() == "*");
}

TEST_CASE("BackendFieldOptimizer: force_all") {
    BackendFieldOptimizer opt;
    opt.note_column_read("A");
    opt.note_column_read("B");
    opt.force_all();
    CHECK(opt.is_all_columns());
}

TEST_CASE("BackendFieldOptimizer: reset") {
    BackendFieldOptimizer opt;
    opt.note_column_read("A");
    opt.note_column_read("B");
    opt.reset();
    CHECK(opt.status == BackendFieldOptimizer::FieldListStatus::Learning);
    CHECK(opt.single_col_reads == 0);
    CHECK(opt.is_all_columns());
}

// ═══════════════════════════════════════════════════════════════════
// BackendWhereBuilder
// ═══════════════════════════════════════════════════════════════════

TEST_CASE("BackendWhereBuilder: empty when no filters") {
    BackendWhereBuilder wb;
    CHECK(wb.build() == "");
    CHECK_FALSE(wb.has_filter());
}

TEST_CASE("BackendWhereBuilder: single filter") {
    BackendWhereBuilder wb;
    wb.for_clause = "STATUS = 'A'";
    CHECK(wb.build() == "(STATUS = 'A')");
    CHECK(wb.has_filter());
}

TEST_CASE("BackendWhereBuilder: multiple filters joined by AND") {
    BackendWhereBuilder wb;
    wb.for_clause   = "STATUS = 'A'";
    wb.user_filter  = "AGE > 18";
    wb.aof_filter   = "CITY = 'NYC'";
    auto result = wb.build();
    CHECK(result.find("STATUS = 'A'") != std::string::npos);
    CHECK(result.find("AGE > 18") != std::string::npos);
    CHECK(result.find("CITY = 'NYC'") != std::string::npos);
    // All connected by AND
    CHECK(result.find(" AND ") != std::string::npos);
}

TEST_CASE("BackendWhereBuilder: scope range") {
    BackendWhereBuilder wb;
    wb.scope_lower = "NAME >= 'A'";
    wb.scope_upper = "NAME <= 'Z'";
    auto result = wb.build();
    CHECK(result.find("NAME >= 'A'") != std::string::npos);
    CHECK(result.find("NAME <= 'Z'") != std::string::npos);
}

TEST_CASE("BackendWhereBuilder: exact seek (lower == upper)") {
    BackendWhereBuilder wb;
    wb.scope_lower = "NAME = 'ALICE'";
    wb.scope_upper = "NAME = 'ALICE'";
    auto result = wb.build();
    // When lower == upper, it should emit a single equality
    CHECK(result.find("NAME = 'ALICE'") != std::string::npos);
}

TEST_CASE("BackendWhereBuilder: clear") {
    BackendWhereBuilder wb;
    wb.for_clause  = "X = 1";
    wb.user_filter = "Y = 2";
    wb.clear();
    CHECK_FALSE(wb.has_filter());
    CHECK(wb.build() == "");
}
