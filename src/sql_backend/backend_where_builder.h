#pragma once

#include <string>
#include <vector>

namespace openads::sql_backend {

// Shared WHERE clause composer for SQL backends. Combines multiple
// filter sources (For clause, user filter, scope, index, historic,
// recno) into a single WHERE fragment joined by AND.
//
// SQLRDD reference: sqlrdd2.prg SR_WORKAREA:SolveRestrictors.
struct BackendWhereBuilder {
    // Individual filter sources. All are optional SQL boolean fragments.
    // Empty strings are ignored (not added to the final clause).
    std::string for_clause;       // FROM index FOR expression
    std::string user_filter;      // SET FILTER predicate
    std::string scope_lower;      // Lower bound (inclusive): field >= value
    std::string scope_upper;      // Upper bound (inclusive): field <= value
    std::string index_where;      // Index-driven restriction
    std::string recno_filter;     // Record number restriction
    std::string aof_filter;       // AOF translated predicate
    std::string extra_where;      // Any additional ad-hoc restriction

    // ── API ─────────────────────────────────────────────────────────

    // Build the combined WHERE clause. Returns "" when no filters
    // are active (meaning no WHERE clause needed).
    std::string build() const {
        std::vector<std::string> parts;
        parts.reserve(8);

        auto add = [&](const std::string& s) {
            if (!s.empty()) parts.push_back(s);
        };

        add(for_clause);
        add(user_filter);
        add(index_where);
        add(aof_filter);
        add(extra_where);

        // Scope: combine lower and upper into a range if both present
        if (!scope_lower.empty() && !scope_upper.empty()) {
            if (scope_lower == scope_upper) {
                // Exact seek: field = value
                parts.push_back(scope_lower);
            } else {
                // Range seek: field >= lower AND field <= upper
                parts.push_back("(" + scope_lower + " AND " + scope_upper + ")");
            }
        } else {
            add(scope_lower);
            add(scope_upper);
        }

        add(recno_filter);

        if (parts.empty()) return "";

        std::string out;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i > 0) out += " AND ";
            // Wrap each part in parentheses for safety
            if (parts[i].size() > 1 && parts[i].front() == '(' &&
                parts[i].back() == ')') {
                out += parts[i];
            } else {
                out += "(" + parts[i] + ")";
            }
        }
        return out;
    }

    // Check if any filter is active.
    bool has_filter() const {
        return !for_clause.empty() || !user_filter.empty() ||
               !scope_lower.empty() || !scope_upper.empty() ||
               !index_where.empty() || !recno_filter.empty() ||
               !aof_filter.empty() || !extra_where.empty();
    }

    // Clear all filters.
    void clear() {
        for_clause.clear();
        user_filter.clear();
        scope_lower.clear();
        scope_upper.clear();
        index_where.clear();
        recno_filter.clear();
        aof_filter.clear();
        extra_where.clear();
    }
};

}  // namespace openads::sql_backend
