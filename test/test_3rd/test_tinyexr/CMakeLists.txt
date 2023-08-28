add_executable(test_tinyexr main.cpp)
add_dependencies(test_tinyexr dep_stb dep_tinyexr )

ExternalProject_Get_Property(dep_tinyexr install_dir)
target_include_directories(test_tinyexr PRIVATE ${install_dir}/include)
target_link_libraries(test_tinyexr PRIVATE miniz)
target_link_directories(test_tinyexr PRIVATE ${install_dir})

ExternalProject_Get_Property(dep_stb install_dir)
target_include_directories(test_tinyexr PRIVATE ${install_dir}/include)
