add_executable(test_eigen main.cpp)
add_dependencies(test_eigen dep_eigen)
ExternalProject_Get_Property(dep_eigen install_dir)
target_include_directories(test_eigen PRIVATE ${install_dir}/include/eigen3)
