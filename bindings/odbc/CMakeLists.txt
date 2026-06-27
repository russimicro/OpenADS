# OpenADS ODBC driver (provider) — a self-contained DLL that the ODBC Driver
# Manager loads. It links the static engine (openads_core) so it needs no other
# OpenADS DLL at runtime. Windows only; built when OPENADS_BUILD_ODBC_DRIVER=ON.
add_library(openads_odbc SHARED
    openads_odbc.cpp
    openads_odbc.def)

target_include_directories(openads_odbc PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(openads_odbc PRIVATE openads_core)
set_target_properties(openads_odbc PROPERTIES OUTPUT_NAME openads_odbc)

# ODBC import library (sql.h/sqlext.h prototypes resolve to our own exports;
# odbc32 is not linked — this DLL *is* the driver, not a consumer).
