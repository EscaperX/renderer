add_cmake_project(
    NAME spdlog
    PLATFORM All
    # CMAKE_ARG
    INCLUDE $<SOURCE_DIR>/include
    LINK $<SOURCE_DIR>/lib
    LIB spdlog$<IF:$<AND:$<CONFIG:Debug>,$<PLATFORM_ID:Windows>>,d,>
)
