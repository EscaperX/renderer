add_executable(test_glm main.cpp)
add_dependencies(test_glm dep_glm)

ExternalProject_Get_Property(dep_glm install_dir)
target_include_directories(test_glm PRIVATE ${install_dir}/include)
target_link_directories(test_glm PRIVATE ${install_dir}/lib)
