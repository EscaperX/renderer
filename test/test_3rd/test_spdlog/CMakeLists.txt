# my_add_executable(
#     NAME test
#     SRC main.cpp
#     LIB spdlog
# )
add_executable(test_spdlog main.cpp)
add_dependencies(test_spdlog dep_spdlog)

ExternalProject_Get_Property(dep_spdlog install_dir)
message("${install_dir}")
target_include_directories(test_spdlog PRIVATE ${install_dir}/include)
target_link_directories(test_spdlog PRIVATE ${install_dir}/lib)
target_link_libraries(test_spdlog PRIVATE spdlog)#$<$<CONFIG:Debug>:d>)
