add_cmake_project(
    NAME spdlog
    PLATFORM All
    # CMAKE_ARG
    INCLUDE $<INSTALL_DIR>/include
    LINK $<INSTALL_DIR>/lib
    LIB spdlog$<IF:$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>,d,>
#    LIB libspdlog
)
