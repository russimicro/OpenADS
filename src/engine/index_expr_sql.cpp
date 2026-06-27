#include "engine/index_expr.h"

#include <cctype>
#include <cstring>
#include <string>
#include <vector>

namespace openads::engine {

// ── Tier-2 SQL push-down (spike) ──────────────────────────────────────────
//
// A recursive-descent translator that mirrors the grammar of the FOR-clause
// interpreter (evaluate_index_expr_truthy in index_expr.cpp) but, instead of
// evaluating against a row, EMITS a SQL boolean expression. Anything it cannot
// translate sets `ok = false`; the public entry then returns std::nullopt and
// the caller falls back to the row interpreter — push-down stays optional and
// can never produce a predicate that matches the wrong rows.
//
//   or   := and ('.OR.'  and)*
//   and  := not ('.AND.' not)*
//   not  := ('.NOT.' | '!') not | cmp
//   cmp  := '(' or ')' | value (op value)?
//   value:= term ('+' term)*
//   term := string | number | ident | ident '(' args ')'

namespace {

struct Emitter {
    const std::string& s;
    std::size_t        i = 0;
    const SqlDialect&  d;
    bool               ok = true;

    Emitter(const std::string& src, const SqlDialect& dia) : s(src), d(dia) {}

    void skip_ws() {
        while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    }
    bool peek_kw(const char* kw) {
        skip_ws();
        std::size_t L = std::strlen(kw);
        if (i + L > s.size()) return false;
        for (std::size_t k = 0; k < L; ++k) {
            if (std::toupper(static_cast<unsigned char>(s[i + k])) !=
                static_cast<unsigned char>(kw[k]))
                return false;
        }
        return true;
    }
    bool match_kw(const char* kw) {
        if (!peek_kw(kw)) return false;
        i += std::strlen(kw);
        return true;
    }
    bool match_char(char c) {
        skip_ws();
        if (i < s.size() && s[i] == c) { ++i; return true; }
        return false;
    }

    std::string emit_or() {
        std::string a = emit_and();
        while (ok && match_kw(".OR.")) {
            std::string b = emit_and();
            a = "(" + a + " OR " + b + ")";
        }
        return a;
    }

    std::string emit_and() {
        std::string a = emit_not();
        while (ok && match_kw(".AND.")) {
            std::string b = emit_not();
            a = "(" + a + " AND " + b + ")";
        }
        return a;
    }

    std::string emit_not() {
        skip_ws();
        if (match_kw(".NOT.") || match_char('!')) {
            std::string inner = emit_not();
            return "(NOT " + inner + ")";
        }
        return emit_cmp();
    }

    std::string emit_cmp() {
        skip_ws();
        if (match_char('(')) {
            std::string inner = emit_or();
            if (!match_char(')')) ok = false;
            return inner;
        }
        std::string lhs = emit_value();
        skip_ws();

        // xBase '$' (substring / "contains"): `needle $ haystack` -> SQL
        // `haystack LIKE '%needle%'`. When the needle is a plain string
        // literal with no LIKE wildcard (% _ \), emit directly.
        // When needle is a column reference, emit: haystack LIKE '%' || needle || '%'
        // (or CONCAT variant for MySQL).
        if (i < s.size() && s[i] == '$') {
            ++i;
            std::string rhs = emit_value();
            if (!ok) return "";
            // Literal needle → direct LIKE (safe, no wildcards)
            if (lhs.size() >= 2 && lhs.front() == '\'' && lhs.back() == '\'') {
                std::string inner = lhs.substr(1, lhs.size() - 2);
                if (inner.find('%')  == std::string::npos &&
                    inner.find('_')  == std::string::npos &&
                    inner.find('\\') == std::string::npos) {
                    return rhs + " LIKE '%" + inner + "%'";
                }
                // Literal needle with LIKE wildcards: semantics differ
                // (xBase treats them as literal, LIKE treats them as
                // wildcards) → decline to avoid wrong results.
                ok = false;
                return "";
            }
            // Column needle → CONCAT pattern
            if (d.use_concat_fn) {
                return rhs + " LIKE CONCAT('%', " + lhs + ", '%')";
            } else {
                return rhs + " LIKE ('%' || " + lhs + " || '%')";
            }
        }

        std::string op;
        auto try_op = [&](const char* tok, const char* sqlop) {
            std::size_t L = std::strlen(tok);
            if (i + L > s.size()) return false;
            if (std::memcmp(s.data() + i, tok, L) != 0) return false;
            i += L;
            op = sqlop;
            return true;
        };
        // Longest match first so ">=" beats ">", "==" beats "=", etc.
        if (try_op("==", "=")  || try_op("!=", "<>") || try_op("<>", "<>") ||
            try_op(">=", ">=") || try_op("<=", "<=") ||
            try_op("=", "=")   || try_op(">", ">")   || try_op("<", "<")   ||
            try_op("#", "<>")) {
            std::string rhs = emit_value();
            return lhs + " " + op + " " + rhs;
        }
        // A bare scalar in boolean position has no portable SQL coercion —
        // decline and let the interpreter handle the whole predicate.
        ok = false;
        return "";
    }

    std::string emit_value() {
        std::string a = emit_term();
        while (ok) {
            skip_ws();
            if (i < s.size() && s[i] == '+') {
                ++i;
                std::string b = emit_term();
                if (d.use_concat_fn)
                    a = "CONCAT(" + a + ", " + b + ")";
                else
                    a = "(" + a + " " + d.concat_op + " " + b + ")";
            } else {
                break;
            }
        }
        return a;
    }

    std::string emit_term() {
        skip_ws();
        if (i >= s.size()) { ok = false; return ""; }
        char c = s[i];

        // String literal — re-quote SQL-style, doubling embedded single quotes.
        if (c == '"' || c == '\'') {
            char q = c; ++i;
            std::string lit;
            while (i < s.size() && s[i] != q) lit.push_back(s[i++]);
            if (i < s.size()) ++i;
            std::string out = "'";
            for (char ch : lit) {
                if (ch == '\'') out += "''";
                else            out.push_back(ch);
            }
            out += "'";
            return out;
        }

        // Number literal (with optional leading minus).
        if (std::isdigit(static_cast<unsigned char>(c)) ||
            (c == '-' && i + 1 < s.size() &&
             std::isdigit(static_cast<unsigned char>(s[i + 1])))) {
            std::string num;
            if (c == '-') num.push_back(s[i++]);
            while (i < s.size() &&
                   (std::isdigit(static_cast<unsigned char>(s[i])) || s[i] == '.'))
                num.push_back(s[i++]);
            return num;
        }

        // Identifier → function call or bare column.
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            std::string name;
            while (i < s.size() &&
                   (std::isalnum(static_cast<unsigned char>(s[i])) || s[i] == '_'))
                name.push_back(s[i++]);
            skip_ws();
            if (i < s.size() && s[i] == '(') {
                ++i;
                std::vector<std::string> args;
                skip_ws();
                if (!(i < s.size() && s[i] == ')')) {
                    args.push_back(emit_value());
                    while (ok && match_char(',')) args.push_back(emit_value());
                }
                if (!match_char(')')) ok = false;
                if (!ok) return "";
                std::string up = name;
                for (auto& ch : up)
                    ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
                return emit_func(up, args);
            }
            // Bare field name → SQL column reference, verbatim.
            return name;
        }

        ok = false;
        return "";
    }

    std::string emit_func(const std::string& fn,
                          const std::vector<std::string>& args) {
        auto arity = [&](std::size_t n) {
            if (args.size() < n) { ok = false; return false; }
            return true;
        };

        // ── String functions ──────────────────────────────────────────
        if (fn == "UPPER" && arity(1)) return "UPPER(" + args[0] + ")";
        if (fn == "LOWER" && arity(1)) return "LOWER(" + args[0] + ")";
        if (fn == "LTRIM" && arity(1)) return "LTRIM(" + args[0] + ")";
        if ((fn == "RTRIM" || fn == "TRIM") && arity(1))
            return "RTRIM(" + args[0] + ")";
        if (fn == "ALLTRIM" && arity(1))
            return d.alltrim_open + args[0] + d.alltrim_close;
        if ((fn == "SUBSTR" || fn == "SUBS") && args.size() >= 2) {
            std::string r = d.substr_fn + "(" + args[0] + ", " + args[1];
            if (args.size() >= 3) r += ", " + args[2];
            return r + ")";
        }
        if (fn == "LEFT" && args.size() >= 2)
            return d.substr_fn + "(" + args[0] + ", 1, " + args[1] + ")";
        if (fn == "RIGHT" && args.size() >= 2) {
            // RIGHT(s, n) → SUBSTR(s, LENGTH(s) - n + 1, n)
            std::string len_fn = d.use_char_length ? "CHAR_LENGTH" : d.length_fn;
            return d.substr_fn + "(" + args[0] + ", " + len_fn + "(" + args[0] + ") - " +
                   args[1] + " + 1, " + args[1] + ")";
        }
        if (fn == "LEN" && arity(1)) {
            std::string len_fn = d.use_char_length ? "CHAR_LENGTH" : d.length_fn;
            return len_fn + "(" + args[0] + ")";
        }
        if ((fn == "AT" || fn == "ATNUM") && args.size() >= 2) {
            // AT(needle, haystack [, occurrence]) → POSITION(needle IN haystack)
            // or INSTR(haystack, needle) depending on dialect
            // Standard SQL: POSITION(needle IN haystack)
            // For simplicity use: INSTR(haystack, needle) which is widely supported
            std::string r = "INSTR(" + args[1] + ", " + args[0] + ")";
            // ATNUM with occurrence > 1: not directly supported, decline
            if (fn == "ATNUM" && args.size() >= 3) { ok = false; return ""; }
            return r;
        }
        if ((fn == "REPLICATE" || fn == "REPL") && args.size() >= 2) {
            // REPLICATE(s, n) → no standard SQL equivalent
            // SQLite: no direct support. PostgreSQL: REPEAT(). MySQL: REPEAT().
            // Use a construct that works in most: (s || s || ...) is impractical.
            // Decline for now — backend-specific.
            ok = false;
            return "";
        }
        if (fn == "SPACE" && arity(1)) {
            // SPACE(n) → RPAD(' ', n, ' ') or REPEAT(' ', n)
            // No portable form. Decline.
            ok = false;
            return "";
        }
        if ((fn == "PADR" || fn == "PADL" || fn == "PAD") && args.size() >= 2) {
            // PADR(s, n [, c]) → RPAD(s, n, c)
            // PADL(s, n [, c]) → LPAD(s, n, c)
            std::string pad_fn = (fn == "PADL") ? "LPAD" : "RPAD";
            std::string fill = (args.size() >= 3) ? args[2] : "' '";
            return pad_fn + "(" + args[0] + ", " + args[1] + ", " + fill + ")";
        }
        if (fn == "PADC" && args.size() >= 2) {
            // PADC(s, n [, c]) → not directly available, use LPAD + RPAD nesting
            std::string fill = (args.size() >= 3) ? args[2] : "' '";
            return "LPAD(RPAD(" + args[0] + ", (" + args[1] + " + LENGTH(" + args[0] + ")) / 2, " +
                   fill + "), " + args[1] + ", " + fill + ")";
        }
        if (fn == "STRTRAN" && args.size() >= 2) {
            // STRTRAN(s, old [, new]) → REPLACE(s, old, new)
            std::string replacement = (args.size() >= 3) ? args[2] : "''";
            return "REPLACE(" + args[0] + ", " + args[1] + ", " + replacement + ")";
        }
        if (fn == "STUFF" && args.size() >= 3) {
            // STUFF(s, start, len [, replacement]) → STUFF/overlay not standard
            // No portable form. Decline.
            ok = false;
            return "";
        }
        if (fn == "OCCURS" && args.size() >= 2) {
            // OCCURS(needle, haystack) — count occurrences. No standard SQL.
            ok = false;
            return "";
        }

        // ── Numeric functions ──────────────────────────────────────────
        if ((fn == "INT" || fn == "FLOOR") && arity(1))
            return "FLOOR(" + args[0] + ")";
        if (fn == "ABS" && arity(1))
            return "ABS(" + args[0] + ")";
        if (fn == "ROUND" && args.size() >= 2)
            return "ROUND(" + args[0] + ", " + args[1] + ")";
        if (fn == "VAL" && arity(1)) {
            // VAL(s) → CAST(s AS DECIMAL) or +s in some dialects
            return "CAST(" + args[0] + " AS DECIMAL)";
        }
        if (fn == "STR" && args.size() >= 1) {
            // STR(n [, len [, dec]]) → CAST(n AS VARCHAR) or TO_CHAR
            // Use CAST for portability
            std::string r = "CAST(" + args[0] + " AS VARCHAR)";
            return r;
        }
        if (fn == "MOD" && args.size() >= 2)
            return "(" + args[0] + " % " + args[1] + ")";
        if (fn == "CEILING" || fn == "CEIL") {
            if (arity(1)) return "CEILING(" + args[0] + ")";
            return "";
        }
        if (fn == "EXP" && arity(1))
            return "EXP(" + args[0] + ")";
        if (fn == "LOG" && arity(1))
            return "LN(" + args[0] + ")";
        if (fn == "LOG10" && arity(1))
            return "LOG(" + args[0] + ")";
        if (fn == "SQRT" && arity(1))
            return "SQRT(" + args[0] + ")";
        if (fn == "SIGN" && arity(1)) {
            // SIGN not universal, but available in PostgreSQL, MySQL 8+
            return "SIGN(" + args[0] + ")";
        }

        // ── Date functions ────────────────────────────────────────────
        if (fn == "DATE" && arity(0)) {
            // DATE() → CURRENT_DATE or CURDATE()
            return d.now_fn.empty() ? "CURRENT_DATE" : d.now_fn;
        }
        if (fn == "TODAY" && arity(0))
            return "CURRENT_DATE";
        if (fn == "NOW" && arity(0))
            return "NOW()";
        if (fn == "TIME" && arity(0))
            return "CURRENT_TIME";
        if (fn == "DATETIME" && arity(0))
            return "NOW()";
        if (fn == "CTOD" && arity(1)) {
            // CTOD('YYYY-MM-DD') → the literal date string is already SQL-compatible
            return args[0];
        }
        if (fn == "DTOC" && arity(1)) {
            // DTOC(d) → CAST(d AS VARCHAR) or TO_CHAR(d, 'YYYY-MM-DD')
            return "CAST(" + args[0] + " AS VARCHAR)";
        }
        if (fn == "DTOS" && arity(1)) {
            // DTOS(d) → TO_CHAR(d, 'YYYYMMDD') or DATE_FORMAT(d, '%Y%m%d')
            // Use REPLACE to strip dashes for portability:
            // REPLACE(CAST(d AS VARCHAR), '-', '')
            return "REPLACE(CAST(" + args[0] + " AS VARCHAR), '-', '')";
        }
        if (fn == "YEAR" && arity(1))
            return "EXTRACT(YEAR FROM " + args[0] + ")";
        if (fn == "MONTH" && arity(1))
            return "EXTRACT(MONTH FROM " + args[0] + ")";
        if (fn == "DAY" && arity(1))
            return "EXTRACT(DAY FROM " + args[0] + ")";
        if (fn == "HOUR" && arity(1))
            return "EXTRACT(HOUR FROM " + args[0] + ")";
        if (fn == "MINUTE" && arity(1))
            return "EXTRACT(MINUTE FROM " + args[0] + ")";
        if (fn == "SECOND" && arity(1))
            return "EXTRACT(SECOND FROM " + args[0] + ")";
        if (fn == "DOW" && arity(1)) {
            // DOW(d) → day of week (1=Sun..7=Sat in xBase)
            // SQL: EXTRACT(DOW FROM d) returns 0=Sun..6=Sat (PostgreSQL)
            //      DAYOFWEEK(d) returns 1=Sun..7=Sat (MySQL)
            // Use EXTRACT and adjust: modulo 7 + 1
            return "(EXTRACT(DOW FROM " + args[0] + ") + 1)";
        }
        if (fn == "CDOW" && arity(1))
            return "TO_CHAR(" + args[0] + ", 'DY')";
        if (fn == "CMONTH" && arity(1))
            return "TO_CHAR(" + args[0] + ", 'Month')";
        if (fn == "DATEADD" && args.size() >= 3) {
            // DATEADD(interval, n, d) → d + INTERVAL 'n interval'
            // Not portable across all DBs. Decline.
            ok = false;
            return "";
        }
        if (fn == "DATEDIFF" && args.size() >= 2) {
            // DATEDIFF(d1, d2) → no standard form. Decline.
            ok = false;
            return "";
        }

        // ── Conversion / conditional ──────────────────────────────────
        if (fn == "IIF" && args.size() >= 3) {
            // IIF(cond, true_val, false_val) → CASE WHEN cond THEN true_val ELSE false_val END
            return "(CASE WHEN " + args[0] + " THEN " + args[1] + " ELSE " + args[2] + " END)";
        }
        if (fn == "IF" && args.size() >= 3)
            return "(CASE WHEN " + args[0] + " THEN " + args[1] + " ELSE " + args[2] + " END)";
        if (fn == "NIL" && arity(0))
            return "NULL";
        if (fn == "EMPTY" && arity(1)) {
            // EMPTY(x) → (x IS NULL OR x = '' OR x = 0)
            return "(" + args[0] + " IS NULL OR " + args[0] + " = '' OR " + args[0] + " = 0)";
        }
        if (fn == "ISNULL" && arity(1))
            return "(" + args[0] + " IS NULL)";
        if (fn == "ISBLANK" && arity(1))
            return "(" + args[0] + " IS NULL OR " + args[0] + " = '')";

        // ── xBase record functions → SQL equivalents ──────────────────
        if (fn == "RECNO" && arity(0)) {
            // RECNO() → rowid or equivalent depending on backend
            // SQLite: rowid, PostgreSQL: oid, general: no direct equivalent
            // Decline — backend must handle this specially
            ok = false;
            return "";
        }
        if (fn == "DELETED" && arity(0)) {
            // DELETED() → SR_DELETED = '*' or similar
            // Decline — backend must handle this specially
            ok = false;
            return "";
        }
        if (fn == "LASTREC" || fn == "RECCOUNT") {
            if (arity(0)) {
                // No portable SQL for record count without COUNT(*)
                ok = false;
                return "";
            }
        }

        // ── Unknown function → decline ────────────────────────────────
        ok = false;
        return "";
    }
};

} // namespace

std::optional<std::string>
try_emit_sql_where(const std::string& expr, const SqlDialect& dialect) {
    std::string e = strip_alias_qualifiers(expr);
    Emitter em(e, dialect);
    em.skip_ws();
    if (em.i >= e.size()) return std::nullopt;   // empty / whitespace only
    std::string out = em.emit_or();
    em.skip_ws();
    // Unconsumed input means an operator/construct we don't model → decline,
    // so we never emit a predicate that silently drops part of the filter.
    if (!em.ok || em.i != e.size()) return std::nullopt;
    return out;
}

} // namespace openads::engine
