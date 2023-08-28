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
