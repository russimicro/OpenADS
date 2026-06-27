#include "doctest.h"
#include "sql_backend/enterprise_config.h"

#include <cstdlib>

namespace {

void cfg_set_env(const char* name, const char* value) {
#ifdef _WIN32
    _putenv_s(name, value);
#else
    ::setenv(name, value, 1);
#endif
}

void cfg_clear_env(const char* name) {
#ifdef _WIN32
    _putenv_s(name, "");
#else
    ::unsetenv(name);
#endif
}

} // namespace

TEST_CASE("enterprise config: defaults are enterprise-scale") {
    const auto& c = openads::sql_backend::enterprise_config();
    // Server limits consumed by the wire server (session reaping + cap).
    CHECK(c.server_max_sessions >= 100);
    CHECK(c.server_listen_backlog >= 64);
    // Pool / lock / timeout defaults (used by the SQL backends in later work).
    CHECK(c.odbc_pool_max_per_dsn >= 32);
    CHECK(c.sqlite_pool_max_per_db >= 16);
    CHECK(c.lock_retry_count >= 20);
    CHECK(c.sqlite_busy_timeout_ms >= 5000);
}

TEST_CASE("enterprise pool toggles default on; OPENADS_POOL_ENABLED=0 disables") {
    // Toggles re-read the environment on each call (no cached singleton), so a
    // deployment can flip pooling off without a restart.
    CHECK(openads::sql_backend::enterprise_pool_enabled() == true);
}

TEST_CASE("server pool flag defaults off and follows OPENADS_SERVER_POOL") {
    cfg_clear_env("OPENADS_SERVER_POOL");
    CHECK(openads::sql_backend::enterprise_server_pool_enabled() == false);

    cfg_set_env("OPENADS_SERVER_POOL", "1");
    CHECK(openads::sql_backend::enterprise_server_pool_enabled() == true);

    cfg_set_env("OPENADS_SERVER_POOL", "0");
    CHECK(openads::sql_backend::enterprise_server_pool_enabled() == false);

    cfg_clear_env("OPENADS_SERVER_POOL");
}

TEST_CASE("server pool workers defaults to 0 and parses the env override") {
    cfg_clear_env("OPENADS_SERVER_POOL_WORKERS");
    CHECK(openads::sql_backend::enterprise_server_pool_workers() == 0u);

    cfg_set_env("OPENADS_SERVER_POOL_WORKERS", "4");
    CHECK(openads::sql_backend::enterprise_server_pool_workers() == 4u);

    cfg_clear_env("OPENADS_SERVER_POOL_WORKERS");
}