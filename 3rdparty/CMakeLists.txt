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
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DJSON_BuildTests=OFF
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG>
    INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIG>
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

# stb
ExternalProject_Add(
    dep_stb
    SOURCE_DIR ${3RD_SOURCE_DIR}/stb
    BINARY_DIR ${3RD_BINARY_DIR}/stb
    INSTALL_DIR ${3RD_INSTALL_DIR}/stb
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/include &&
        ${CMAKE_COMMAND} -E copy
            <SOURCE_DIR>/stb_image.h <INSTALL_DIR>/include
            <SOURCE_DIR>/stb_image_write.h <INSTALL_DIR>/include
)

# tinyexr
ExternalProject_Add(
    dep_tinyexr
    SOURCE_DIR ${3RD_SOURCE_DIR}/tinyexr
    BINARY_DIR ${3RD_INSTALL_DIR}/tinyexr
    INSTALL_DIR ${3RD_INSTALL_DIR}/tinyexr
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    BUILD_COMMAND ${CMAKE_COMMAND} --build <INSTALL_DIR> --config $<CONFIG>
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E  make_directory <INSTALL_DIR>/include &&
        ${CMAKE_COMMAND} -E copy
            <SOURCE_DIR>/tinyexr.h <INSTALL_DIR>/include
            <SOURCE_DIR>/deps/miniz/miniz.h <INSTALL_DIR>/include
            # <SOURCE_DIR>/deps/miniz/miniz.c <INSTALL_DIR>/include
)

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


# tev

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    file(
        DOWNLOAD "https://github.com/Tom94/tev/releases/download/v1.26/tev.appimage"
        "${3RD_SOURCE_DIR}/tev/tev.appimage"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    file(
        DOWNLOAD "https://github.com/Tom94/tev/releases/download/v1.26/tev.exe"
        "${3RD_SOURCE_DIR}/tev/tev.exe"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    file(
        DOWNLOAD "https://github.com/Tom94/tev/releases/download/v1.26/tev.dmg"
        "${3RD_SOURCE_DIR}/tev/tev.dmg"
    )
endif()

