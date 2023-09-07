add_executable(test_glfw main.cpp)
add_dependencies(test_glfw dep_glfw dep_glad)
ExternalProject_Get_Property(dep_glfw install_dir)
target_include_directories(test_glfw PRIVATE ${install_dir}/include/glfw)
target_link_libraries(test_glfw PRIVATE glfw3)
target_link_directories(test_glfw PRIVATE ${install_dir}/lib)

ExternalProject_Get_Property(dep_glad install_dir)
target_include_directories(test_glfw PRIVATE ${install_dir}/include/)
