#pragma once

#include <optional>
#include <string>

namespace openads::platform {

// On Windows the filesystem is already case-insensitive; on POSIX this
// scans the parent directory once to find a case-folded match. Returns
// the input unchanged if no match exists.
std::string resolve_case_insensitive(const std::string& path);

// Resolve `client_path` under `root` (relative paths are joined first),
// canonicalize, and return the result only when it stays inside `root`.
// Absolute client paths are accepted when they already lie under `root`.
// Returns nullopt when the path escapes the jail (e.g. `..` segments).
std::optional<std::string> resolve_under_root(const std::string& root,
                                              const std::string& client_path);

} // namespace openads::platform
