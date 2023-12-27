file(GLOB_RECURSE util_wrapper_files *.cpp *.hpp *.c PARENT_SCOPE)
add_library(opengl_utils  ${util_wrapper_files})
message("files: ${util_wrapper_files}")
target_include_directories(opengl_utils PUBLIC ${CMAKE_CURRENT_LIST_DIR}/util_wrapper)
add_dependencies(opengl_utils dep_glfw dep_glad)

ExternalProject_Get_Property(dep_glad install_dir)
target_include_directories(opengl_utils PUBLIC ${install_dir}/include/)
message("glad_include ${install_dir}/include/")

ExternalProject_Get_Property(dep_glfw install_dir)
target_include_directories(opengl_utils PUBLIC ${install_dir}/include/)
target_link_libraries(opengl_utils PUBLIC glfw3)
target_link_directories(opengl_utils PUBLIC ${install_dir}/lib)



target_include_directories(opengl_utils PUBLIC ${CMAKE_CURRENT_LIST_DIR}/)

get_target_property(dirs opengl_utils INCLUDE_DIRECTORIES)
message("glad_include ${dirs}")