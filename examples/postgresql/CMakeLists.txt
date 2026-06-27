cmake_minimum_required(VERSION 3.16)
project(openads_pg_example C)

# Standalone example: links against an installed / built OpenADS engine
# (openace) through the ACE API. libpq is NOT needed here — it lives inside
# the engine; the example only speaks the ACE API.
#
# Point these at your OpenADS build/install:
#   -DOPENADS_INCLUDE=<dir containing openads/ace.h>
#   -DOPENACE_LIB=<path to openace64.lib / libopenace64.so / .dylib>
#
# Example (Windows, against an in-tree build):
#   cmake -S . -B build ^
#     -DOPENADS_INCLUDE=../../include ^
#     -DOPENACE_LIB=../../build/msvc-x64/src/Release/openace64.lib
#   cmake --build build --config Release

set(OPENADS_INCLUDE "" CACHE PATH "Directory containing openads/ace.h")
set(OPENACE_LIB      "" CACHE FILEPATH "OpenADS engine import library (openace64.lib / libopenace64.*)")

if(NOT OPENADS_INCLUDE OR NOT OPENACE_LIB)
    message(FATAL_ERROR
        "Set -DOPENADS_INCLUDE=<include dir> and -DOPENACE_LIB=<openace lib>. "
        "See the comment block in this CMakeLists.txt.")
endif()

add_executable(pg_example main.c)
target_include_directories(pg_example PRIVATE "${OPENADS_INCLUDE}")
target_link_libraries(pg_example PRIVATE "${OPENACE_LIB}")

# On Windows the engine DLL (openace64.dll) and libpq.dll must be on PATH
# (or beside the .exe) at run time.
