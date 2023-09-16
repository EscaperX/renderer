add_executable(render main.cpp glad.c)

add_dependencies(render dep_glfw dep_glad)
ExternalProject_Get_Property(dep_glfw install_dir)
target_include_directories(render PRIVATE ${install_dir}/include/)
target_link_libraries(render PRIVATE glfw3)
target_link_directories(render PRIVATE ${install_dir}/lib)

ExternalProject_Get_Property(dep_glad install_dir)
target_include_directories(render PRIVATE ${install_dir}/include/)
