include(ExternalProject)

set(3RD_DIR ${CMAKE_SOURCE_DIR}/3rdparty CACHE PATH "" FORCE)
set(3RD_SOURCE_DIR ${3RD_DIR}/ CACHE PATH "" FORCE)
set(3RD_BINARY_DIR ${CMAKE_BINARY_DIR}/ThirdPartyBuild CACHE PATH "" FORCE)
set(3RD_INSTALL_DIR ${CMAKE_BINARY_DIR}/ThirdPartyInstall/ CACHE PATH "" FORCE )

# spdlog
ExternalProject_Add(
    dep_spdlog
    SOURCE_DIR ${3RD_SOURCE_DIR}/spdlog
    BINARY_DIR ${3RD_BINARY_DIR}/spdlog
    INSTALL_DIR ${3RD_INSTALL_DIR}/spdlog
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG>
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIG>
)

# json
ExternalProject_Add(
    dep_json
    SOURCE_DIR ${3RD_SOURCE_DIR}/nlohmann
    BINARY_DIR ${3RD_BINARY_DIR}/nlohmann
    INSTALL_DIR ${3RD_INSTALL_DIR}/nlohmann
    PREFIX ${3RD_SOURCE_DIR}/nlohmann
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
    DOWNLOAD_NO_EXTRACT 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/include/nlohmann &&
        ${CMAKE_COMMAND} -E copy
            <SOURCE_DIR>/src/json.hpp <INSTALL_DIR>/include/nlohmann
)

# eigen
ExternalProject_Add(
    dep_eigen
    SOURCE_DIR ${3RD_SOURCE_DIR}/eigen
    BINARY_DIR ${3RD_BINARY_DIR}/eigen
    INSTALL_DIR ${3RD_INSTALL_DIR}/eigen
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DEIGEN_BUILD_TESTING=OFF
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG>
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIG>
)

# # stb
# ExternalProject_Add(
#     dep_stb
#     SOURCE_DIR ${3RD_SOURCE_DIR}/stb
#     BINARY_DIR ${3RD_BINARY_DIR}/stb
#     INSTALL_DIR ${3RD_INSTALL_DIR}/stb
#     CONFIGURE_COMMAND ""
#     BUILD_COMMAND ""
#     INSTALL_COMMAND
#         ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/include &&
#         ${CMAKE_COMMAND} -E copy
#             <SOURCE_DIR>/stb_image.h <INSTALL_DIR>/include
#             <SOURCE_DIR>/stb_image_write.h <INSTALL_DIR>/include
# )

# # tinyexr
# ExternalProject_Add(
#     dep_tinyexr
#     SOURCE_DIR ${3RD_SOURCE_DIR}/tinyexr
#     BINARY_DIR ${3RD_INSTALL_DIR}/tinyexr
#     INSTALL_DIR ${3RD_INSTALL_DIR}/tinyexr
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
#     BUILD_COMMAND ${CMAKE_COMMAND} --build <INSTALL_DIR> --config $<CONFIG>
#     INSTALL_COMMAND
#         ${CMAKE_COMMAND} -E  make_directory <INSTALL_DIR>/include &&
#         ${CMAKE_COMMAND} -E copy
#             <SOURCE_DIR>/tinyexr.h <INSTALL_DIR>/include
#             <SOURCE_DIR>/deps/miniz/miniz.h <INSTALL_DIR>/include
#             # <SOURCE_DIR>/deps/miniz/miniz.c <INSTALL_DIR>/include
# )

#pugixml
ExternalProject_Add(
    dep_pugixml
    SOURCE_DIR ${3RD_SOURCE_DIR}/pugixml
    BINARY_DIR ${3RD_BINARY_DIR}/pugixml
    INSTALL_DIR ${3RD_INSTALL_DIR}/pugixml
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DPUGIXML_BUILD_TESTS=OFF
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG>
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIG>
)

#glfw
ExternalProject_Add(
    dep_glfw
    SOURCE_DIR ${3RD_SOURCE_DIR}/glfw
    BINARY_DIR ${3RD_BINARY_DIR}/glfw
    INSTALL_DIR ${3RD_INSTALL_DIR}/glfw
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG>
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIG>
)

#glad
ExternalProject_Add(
    dep_glad
    SOURCE_DIR ${3RD_SOURCE_DIR}/glad
    BINARY_DIR ${3RD_BINARY_DIR}/glad
    INSTALL_DIR ${3RD_INSTALL_DIR}/glad
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/include/glad &&
        ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/include/KHR &&
        ${CMAKE_COMMAND} -E copy
            <SOURCE_DIR>/gl.h <INSTALL_DIR>/include/glad
            <SOURCE_DIR>/glad.h <INSTALL_DIR>/include/glad &&
        ${CMAKE_COMMAND} -E copy
            <SOURCE_DIR>/KHR/khrplatform.h <INSTALL_DIR>/include/KHR/
)
#glm
ExternalProject_Add(
    dep_glm
    SOURCE_DIR ${3RD_SOURCE_DIR}/glm
    BINARY_DIR ${3RD_BINARY_DIR}/glm
    INSTALL_DIR ${3RD_INSTALL_DIR}/glm
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DBUILD_TESTING=OFF
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG>
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIG>
)


# add_custom_target(dep_glad
#     COMMAND ${CMAKE_COMMAND} -E  make_directory ${3RD_INSTALL_DIR}/glad/include &&
#             ${CMAKE_COMMAND} -E copy ${3RD_SOURCE_DIR}/glad/gl.h ${3RD_INSTALL_DIR}/glad/include
# )
# set_target_properties(dep_glad PROPERTIES install_dir "${3RD_INSTALL_DIR}/glad")

# add_dependencies(dep_glad all)

# # tev
#
# if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#     file(
#         DOWNLOAD "https://github.com/Tom94/tev/releases/download/v1.26/tev.appimage"
#         "${3RD_SOURCE_DIR}/tev/tev.appimage"
#     )
# elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#     file(
#         DOWNLOAD "https://github.com/Tom94/tev/releases/download/v1.26/tev.exe"
#         "${3RD_SOURCE_DIR}/tev/tev.exe"
#     )
# elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
#     file(
#         DOWNLOAD "https://github.com/Tom94/tev/releases/download/v1.26/tev.dmg"
#         "${3RD_SOURCE_DIR}/tev/tev.dmg"
#     )
# endif()

