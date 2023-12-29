option(USE_GLM "" ON)
if (USE_GLM)
    list(APPEND DEFINITIONS "USE_GLM")
endif ()


file(GLOB_RECURSE core_headers
    "${CMAKE_CURRENT_LIST_DIR}/*.hpp")

file(GLOB_RECURSE core_sources
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/*.c")

add_library(core ${core_sources} ${core_headers})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_dependencies(core dep_glm dep_glad dep_glfw)

ExternalProject_Get_Property(dep_glm install_dir)
target_include_directories(core PUBLIC ${install_dir}/include/)

ExternalProject_Get_Property(dep_glad install_dir)
target_include_directories(core PUBLIC ${install_dir}/include/)

ExternalProject_Get_Property(dep_glfw install_dir)
target_include_directories(core PUBLIC ${install_dir}/include/)
target_link_directories(core PUBLIC ${install_dir}/lib/)
target_link_libraries(core PUBLIC glfw3)

target_compile_definitions(core PUBLIC ${DEFINITIONS})



add_executable(renderer main.cpp)
target_link_libraries(renderer PUBLIC core)