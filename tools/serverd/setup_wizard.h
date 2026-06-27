// openads_serverd — interactive first-run setup wizard.
//
// `openads_serverd --setup` walks the operator through the same questions
// the old Advantage installer asked (data directory, port, whether to start
// automatically) and writes an openads.ini the daemon can later be pointed
// at with `--config`. Cross-platform: the prompts run identically on
// Windows, Linux and macOS. The only OS-specific step is "start at boot":
//   - POSIX: the wizard writes a filled-in systemd unit (Linux) or launchd
//     plist (macOS) next to the config and prints the enable commands.
//   - Windows: the wizard sets want_service so main() can register the
//     Windows Service through the SCM (that code lives in main.cpp).
//
// Code page is deliberately NOT asked here: the engine does not yet expose a
// selectable server code page (UTF-8 / CP437 today), and the house rule is to
// never present a control that does nothing. When code-page selection lands
// it slots in as one more prompt + one more ini key.

#pragma once

#include <string>

namespace openads::serverd {

struct SetupResult {
    std::string ini_path;            // file the wizard wrote
    bool        want_service = false;  // Windows: register the service
};

// Run the wizard, reading answers from stdin. `exe_path` is the absolute
// path to this daemon, embedded in any generated systemd/launchd unit.
// Returns true on success (config written), false if the user aborted or a
// write failed.
bool run_setup_wizard(const std::string& exe_path, SetupResult& out);

}  // namespace openads::serverd
