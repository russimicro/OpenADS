#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace openads::sql_backend {

// Shared field-access optimizer for SQL backends. Tracks which columns
// are actually read per table, and after enough single-column reads
// switches to SELECT * to avoid repeated demand-fetches.
//
// SQLRDD reference: sqlrdd2.prg SR_WORKAREA:sqlGetValue, FIELD_LIST_*,
// SQLRDD_LEARNING_REPETITIONS.
struct BackendFieldOptimizer {
    // Number of single-column fetches before we give up and load all
    // columns.  Matches SQLRDD_LEARNING_REPETITIONS.
    static constexpr std::uint32_t LEARNING_THRESHOLD = 5;

    // Status of the field list learning.
    enum class FieldListStatus : std::uint8_t {
        Learning    = 0,   // Still tracking individual column reads
        Stable      = 1,   // Switched to SELECT * — stop tracking
        Changed     = 2,   // Schema changed, re-enter Learning
        NewValueRead = 3,  // A new column was read, increment counter
    };

    FieldListStatus status = FieldListStatus::Learning;

    // Per-column access counter. Key = column name (uppercase).
    struct ColumnStats {
        std::uint32_t read_count = 0;       // How many times fetched
        bool          ever_read  = false;   // Was it ever fetched?
    };
    std::unordered_map<std::string, ColumnStats> columns;

    // Total number of single-column reads in Learning state.
    std::uint32_t single_col_reads = 0;

    // The select list to use. When empty, SELECT * applies.
    std::vector<std::string> select_list;

    // ── API ─────────────────────────────────────────────────────────

    // Notify that a specific column was read. Returns the column list
    // that should be used in the next SELECT (empty = SELECT *).
    const std::vector<std::string>& note_column_read(
            const std::string& col_name) {
        if (status == FieldListStatus::Stable) {
            return select_list;  // already SELECT *
        }

        auto up = to_upper(col_name);
        auto& s = columns[up];
        if (!s.ever_read) {
            s.ever_read = true;
            ++single_col_reads;
        }
        ++s.read_count;

        // After threshold unique single-column reads, switch to all
        if (single_col_reads >= LEARNING_THRESHOLD) {
            status = FieldListStatus::Stable;
            select_list.clear();  // empty = SELECT *
        }
        return select_list;
    }

    // Build the SELECT column list fragment.
    // Returns "*" when stable, or a comma-separated list of columns.
    std::string select_fragment() const {
        if (status == FieldListStatus::Stable || select_list.empty()) {
            return "*";
        }
        std::string out;
        for (size_t i = 0; i < select_list.size(); ++i) {
            if (i > 0) out += ", ";
            out += select_list[i];
        }
        return out;
    }

    // Force all-columns mode (e.g. when the caller needs every column
    // for an aggregate or ORDER BY on an unlisted column).
    void force_all() {
        status = FieldListStatus::Stable;
        select_list.clear();
    }

    // Reset on table close / re-open.
    void reset() {
        status = FieldListStatus::Learning;
        columns.clear();
        single_col_reads = 0;
        select_list.clear();
    }

    bool is_all_columns() const {
        return status == FieldListStatus::Stable || select_list.empty();
    }

private:
    static std::string to_upper(const std::string& s) {
        std::string r = s;
        for (auto& c : r) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
        return r;
    }
};

}  // namespace openads::sql_backend
