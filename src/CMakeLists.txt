option(USE_EIGEN "" ON)

file(GLOB_RECURSE core_headers
    "${CMAKE_CURRENT_LIST_DIR}/*.hpp")

file(GLOB_RECURSE core_sources
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/*.c")

add_library(core ${core_sources})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_dependencies(core dep_eigen dep_glad dep_glfw)

ExternalProject_Get_Property(dep_eigen install_dir)
target_include_directories(core PUBLIC ${install_dir}/include/eigen3)

ExternalProject_Get_Property(dep_glad install_dir)
target_include_directories(core PUBLIC ${install_dir}/include/)

ExternalProject_Get_Property(dep_glfw install_dir)
target_include_directories(core PUBLIC ${install_dir}/include/)
target_link_directories(core PUBLIC ${install_dir}/lib/)
target_link_libraries(core PUBLIC glfw3)

target_compile_definitions(core PUBLIC ${DEFINITIONS})



add_executable(renderer main.cpp)
target_link_libraries(renderer PUBLIC core)