option(USE_EIGEN "" ON)
add_subdirectory(utils)
add_subdirectory(opengl)

add_executable(renderer main.cpp)

add_dependencies(renderer dep_eigen)
ExternalProject_Get_Property(dep_eigen install_dir)
target_include_directories(renderer PRIVATE ${install_dir}/include/eigen3)

if(USE_EIGEN)
    target_compile_definitions(renderer PRIVATE "-DUSE_EIGEN")
endif(USE_EIGEN)

