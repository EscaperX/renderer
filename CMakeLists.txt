cmake_minimum_required(VERSION 3.5)

project(renderer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DBUILD_CONFIG_DEBUG=$<IF:$<CONFIG:Debug>,1,0>)

include(cmake/Docs.cmake)
# include(cmake/target.cmake)
# include(cmake/3rdparty.cmake)

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
  list (APPEND DEFINITIONS "PLATFORM_WINDOWS" )
elseif (CMAKE_SYSTEM_NAME STREQUAL Darwin)
  list (APPEND DEFINITIONS "PLATFORM_OSX")
elseif (CMAKE_SYSTEM_NAME STREQUAL Linux)
  list (APPEND DEFINITIONS "PLATFORM_LINUX")
endif()


add_subdirectory(3rdparty)
add_subdirectory(src)
# add_subdirectory(test)
