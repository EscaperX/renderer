add_executable(test_stb main.cpp)
add_dependencies(test_stb dep_stb)
ExternalProject_Get_Property(dep_stb install_dir)
target_include_directories(test_stb PRIVATE ${install_dir}/include)
