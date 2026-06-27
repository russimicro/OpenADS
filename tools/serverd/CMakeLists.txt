add_executable(openads_serverd main.cpp config_ini.cpp setup_wizard.cpp)
target_link_libraries(openads_serverd PRIVATE openads_core)
target_include_directories(openads_serverd PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src)

if(OPENADS_WITH_HTTP)
    target_sources(openads_serverd PRIVATE http_server.cpp)
    target_compile_definitions(openads_serverd PRIVATE OPENADS_WITH_HTTP=1)
    target_link_libraries(openads_serverd PRIVATE
        httplib::httplib
        nlohmann_json::nlohmann_json)
endif()
