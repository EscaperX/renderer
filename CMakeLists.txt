cmake_minimum_required(VERSION 3.2)

project(renderer)

add_definitions(-DBUILD_CONFIG_DEBUG=$<IF:$<CONFIG:Debug>,1,0>)

include(cmake/Docs.cmake)
# include(cmake/target.cmake)
# include(cmake/3rdparty.cmake)

add_subdirectory(3rdparty)
add_subdirectory(src)
add_subdirectory(test)
