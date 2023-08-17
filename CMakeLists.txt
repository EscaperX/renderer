cmake_minimum_required(VERSION 3.2)

project(renderer)
include(ExternalProject)
include(cmake/target.cmake)
include(cmake/3rdparty.cmake)

add_subdirectory(3rdparty)
add_subdirectory(src)
# add_subdirectory(test)
