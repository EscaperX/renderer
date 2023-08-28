add_executable(test_pugixml main.cpp)
add_dependencies(test_pugixml dep_pugixml)
ExternalProject_Get_Property(dep_pugixml install_dir)

target_include_directories(test_pugixml PRIVATE ${install_dir}/include)
target_link_directories(test_pugixml PRIVATE ${install_dir}/lib)
target_link_libraries(test_pugixml PRIVATE pugixml)
