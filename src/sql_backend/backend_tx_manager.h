#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace openads::sql_backend {

// Shared transaction manager for SQL backends. Embeds in each
// BackendConnection to provide:
//   - Nested transaction counting (BEGIN at nesting=1, COMMIT only at 0)
//   - Auto-commit threshold (auto-commit after N DML statements)
//   - Dual-connection support (main + metadata connection)
//
// SQLRDD reference: sqlrdd.prg SR_CONNECTION:nTransacCount, nAutoCommit,
// nIteractions, oSqlTransact.
struct BackendTxManager {
    // ── Nested transaction depth ────────────────────────────────────
    // begin_tx() increments; commit_tx() / rollback_tx() decrements.
    // Actual database COMMIT/ROLLBACK fires only when nesting reaches 0.
    std::int32_t nesting = 0;

    // ── Auto-commit threshold ───────────────────────────────────────
    // When > 0, after dml_count reaches this threshold an auto-commit
    // fires. Set via connection string AUTOCOMMIT=N; 0 = disabled.
    std::int32_t auto_commit_threshold = 0;

    // Running count of DML statements since last commit.
    std::int32_t dml_count = 0;

    // ── Dirty flag ──────────────────────────────────────────────────
    // Set to true by any DML (INSERT/UPDATE/DELETE). Cleared on commit.
    bool dirty = false;

    // ── Callbacks ───────────────────────────────────────────────────
    // Each backend wires these to its native BEGIN/COMMIT/ROLLBACK.
    // The begin callback receives `is_nested` (true when nesting > 1,
    // meaning the backend should use SAVEPOINT instead of BEGIN).
    std::function<void(bool is_nested)> on_begin;
    std::function<void(bool is_nested)> on_commit;
    std::function<void()>               on_rollback;
    std::function<void(const std::string&)> on_savepoint;  // name
    std::function<void(const std::string&)> on_release_savepoint;
    std::function<void(const std::string&)> on_rollback_savepoint;

    // ── Auto-commit hook ────────────────────────────────────────────
    // Called after each DML. Returns true if auto-commit fired.
    bool note_dml() {
        if (auto_commit_threshold <= 0) return false;
        ++dml_count;
        if (dml_count >= auto_commit_threshold && dirty) {
            commit(false);
            return true;
        }
        return false;
    }

    // ── Transaction lifecycle ───────────────────────────────────────

    void begin() {
        if (nesting == 0) {
            dirty = false;
            dml_count = 0;
            if (on_begin) on_begin(false);
        } else {
            // Nested: use SAVEPOINT if backend supports it
            if (on_savepoint) {
                on_savepoint("sp_" + std::to_string(nesting));
            } else if (on_begin) {
                on_begin(true);
            }
        }
        ++nesting;
    }

    // Returns true when the actual COMMIT fired (nesting reached 0).
    bool commit(bool force = false) {
        if (nesting <= 0) return false;
        --nesting;
        if (nesting == 0 || force) {
            if (on_commit) on_commit(false);
            dirty = false;
            dml_count = 0;
            // Release all savepoints
            return true;
        }
        // Nested commit: release the most recent savepoint
        if (on_release_savepoint) {
            on_release_savepoint("sp_" + std::to_string(nesting + 1));
        }
        return false;
    }

    // Returns true when the actual ROLLBACK fired (nesting reached 0).
    bool rollback() {
        if (nesting <= 0) return false;
        if (nesting == 1) {
            if (on_rollback) on_rollback();
            dirty = false;
            dml_count = 0;
            nesting = 0;
            return true;
        }
        // Nested rollback: rollback to savepoint only
        if (on_rollback_savepoint) {
            on_rollback_savepoint("sp_" + std::to_string(nesting));
        }
        --nesting;
        return false;
    }

    bool in_transaction() const { return nesting > 0; }
};

}  // namespace openads::sql_backend
