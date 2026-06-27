#include "platform/path.h"

#include <algorithm>
#include <cctype>
#include <filesystem>

namespace fs = std::filesystem;

namespace openads::platform {

namespace {

std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) {
                       return static_cast<char>(std::tolower(c));
                   });
    return s;
}

} // namespace

std::string resolve_case_insensitive(const std::string& path) {
    fs::path p(path);
    fs::path parent = p.parent_path();
    if (parent.empty()) parent = ".";

    std::error_code ec;
    if (!fs::is_directory(parent, ec)) {
        // Parent directory does not exist: nothing to resolve.
        return path;
    }

    const std::string leaf      = p.filename().string();
    const std::string leaf_low  = to_lower(leaf);

    // First pass: prefer an exact-case match if one exists. This keeps
    // the input verbatim when the case is already correct.
    for (const auto& entry : fs::directory_iterator(parent, ec)) {
        if (ec) break;
        if (entry.path().filename().string() == leaf) {
            return entry.path().string();
        }
    }

    // Second pass: case-insensitive match returns the on-disk casing.
    for (const auto& entry : fs::directory_iterator(parent, ec)) {
        if (ec) break;
        if (to_lower(entry.path().filename().string()) == leaf_low) {
            return entry.path().string();
        }
    }

    return path;
}

namespace {

bool path_has_prefix(const std::string& path, const std::string& root) {
    if (root.empty()) return true;
    if (path.size() < root.size()) return false;
    if (path.compare(0, root.size(), root) != 0) return false;
    if (path.size() == root.size()) return true;
    const char next = path[root.size()];
    return next == '/' || next == '\\';
}

} // namespace

std::optional<std::string> resolve_under_root(const std::string& root,
                                              const std::string& client_path) {
    namespace fs = std::filesystem;
    if (client_path.empty()) return std::nullopt;

    std::error_code ec;
    fs::path root_p = root.empty()
        ? fs::current_path(ec)
        : fs::absolute(fs::path(root), ec);
    if (ec) return std::nullopt;

    fs::path client_p(client_path);
    fs::path combined = client_p.is_absolute() ? client_p : (root_p / client_p);

    fs::path canon = fs::weakly_canonical(combined, ec);
    if (ec) return std::nullopt;

    fs::path root_canon = fs::weakly_canonical(root_p, ec);
    if (ec) root_canon = root_p.lexically_normal();

    const std::string canon_s = canon.generic_string();
    const std::string root_s  = root_canon.generic_string();
    if (!path_has_prefix(canon_s, root_s)) return std::nullopt;
    return canon_s;
}

} // namespace openads::platform
