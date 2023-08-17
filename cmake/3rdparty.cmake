set(3RD_DIR ${CMAKE_SOURCE_DIR}/3rdparty CACHE PATH "" FORCE)
set(3RD_SOURCE_DIR ${3RD_DIR}/ CACHE PATH "" FORCE)
set(3RD_BINARY_DIR ${CMAKE_BINARY_DIR}/ThirdPartyBuild CACHE PATH "" FORCE)
set(3RD_INSTALL_DIR ${CMAKE_BINARY_DIR}/ThirdPartyInstall CACHE PATH "" FORCE)

function(expand_3rd_path_expression)
    cmake_parse_arguments(PARAMS "" "OUTPUT;SOURCE_DIR;BINARY_DIR;INSTALL_DIR" "INPUT" ${ARGN})

    foreach(I ${PARAMS_INPUT})
        set(TEMP "${I}")
        if (DEFINED PARAMS_SOURCE_DIR)
            string(REPLACE "$<SOURCE_DIR>" ${PARAMS_SOURCE_DIR} TEMP ${TEMP})
            message("${PARAMS_SOURCE_DIR}")
        endif()
        if (DEFINED PARAMS_BINARY_DIR)
            string(REPLACE "$<BINARY_DIR>" ${PARAMS_BINARY_DIR} TEMP ${TEMP})
            message("${PARAMS_BINARY_DIR}")
        endif()
        if (DEFINED PARAMS_INSTALL_DIR)
            string(REPLACE "$<INSTALL_DIR>" ${PARAMS_INSTALL_DIR} TEMP ${TEMP})
            message("${PARAMS_INSTALL_DIR}")
        endif()
        list(APPEND RESULT ${TEMP})
    endforeach()
    message("${RESULT}")
    message("")
    set(${PARAMS_OUTPUT} ${RESULT} PARENT_SCOPE)
endfunction()

function(get_3rd_platform_path)
    cmake_parse_arguments(PARAMS "" "OUTPUT" "INPUT" ${ARGN})

    set(PLATFORM_KEYWORDS "Windows;Darwin;Linux")

    # match a platform
    set(HAS_KEYWORDS FALSE)
    foreach (I ${PARAMS_INPUT})
        foreach (K ${PLATFORM_KEYWORDS})
            if (${I} STREQUAL ${K})
                set(HAS_KEYWORDS TRUE)
                set(RESULT ${I})
                break()
            endif()
        endforeach()

        if (${HAS_KEYWORDS})
            break()
        endif()
    endforeach()

    # I don't understand
#     set(START_LOG FALSE)
#     foreach (I ${PARAMS_INPUT})
#         if ((NOT ${START_LOG}) AND (${I} STREQUAL ${CMAKE_SYSTEM_NAME}))
#             set(START_LOG TRUE)
#             continue()
#         endif()
#
#         if (NOT ${START_LOG})
#             continue()
#         endif()
#
#         set(END_LOG FALSE)
#         foreach (K ${PLATFORM_KEYWORDS})
#             if (${K} STREQUAL ${I})
#                 set(END_LOG TRUE)
#                 break()
#             endif()
#         endforeach()
#
#         if (${END_LOG})
#             break()
#         endif()
#
#         list(APPEND RESULT ${I})
#     endforeach ()

    if (${HAS_KEYWORDS})
        set(${PARAMS_OUTPUT} ${RESULT} PARENT_SCOPE)
    else()
        set(${PARAMS_OUTPUT} ${PARAMS_INPUT} PARENT_SCOPE)
    endif()
endfunction()

function(add_cmake_project)
    cmake_parse_arguments(PARAMS "" "NAME;PLATFORM;" "CMAKE_ARG;INCLUDE;LINK;LIB;RUNTIME_DEP" ${ARGN})

    if ((NOT (${PARAMS_PLATFORM} STREQUAL "All")) AND (NOT (${PARAMS_PLATFORM} STREQUAL ${CMAKE_SYSTEM_NAME})))
        return()
    endif()

    set(NAME "${PARAMS_NAME}")
    set(FULL_NAME "${PARAMS_NAME}-${PARAMS_PLATFORM}")
    set(SOURCE_DIR "${3RD_SOURCE_DIR}/${NAME}")
    set(BINARY_DIR "${3RD_BINARY_DIR}/${NAME}")
    set(INSTALL_DIR "${3RD_INSTALL_DIR}/${NAME}/$<CONFIG>")

    ExternalProject_Add(
        ${NAME}
        SOURCE_DIR ${SOURCE_DIR}
        BINARY_DIR ${BINARY_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} ${PARAMS_CMAKE_ARG}
        BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG>
        INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIG>
    )
    set_target_properties(
        ${NAME} PROPERTIES
        3RD_TYPE "CMakeProject"
    )

    if (DEFINED PARAMS_INCLUDE)
        expand_3rd_path_expression(
            INPUT ${PARAMS_INCLUDE}
            OUTPUT R_INCLUDE
            SOURCE_DIR ${SOURCE_DIR}
            BINARY_DIR ${BINARY_DIR}
            INSTALL_DIR ${INSTALL_DIR}
        )
        get_3rd_platform_path(
            INPUT ${R_INCLUDE}
            OUTPUT P_INCLUDE
        )
        set_target_properties(
            ${NAME} PROPERTIES
            3RD_INCLUDE "${P_INCLUDE}"
        )
        message("INCLUDE path: ${P_INCLUDE}")
    endif()

    if (DEFINED PARAMS_LINK)
        expand_3rd_path_expression(
            INPUT ${PARAMS_LINK}
            OUTPUT R_LINK
            SOURCE_DIR ${SOURCE_DIR}
            BINARY_DIR ${BINARY_DIR}
            INSTALL_DIR ${INSTALL_DIR}
        )
        get_3rd_platform_path(
            INPUT ${R_LINK}
            OUTPUT P_LINK
        )
        set_target_properties(
            ${NAME} PROPERTIES
            3RD_LINK "${P_LINK}"
        )
        message("LINK path: ${P_LINK}")
    endif()

    if (DEFINED PARAMS_LIB)
        expand_3rd_path_expression(
            INPUT ${PARAMS_LIB}
            OUTPUT R_LIB
            SOURCE_DIR ${SOURCE_DIR}
            BINARY_DIR ${BINARY_DIR}
            INSTALL_DIR ${INSTALL_DIR}
        )
        get_3rd_platform_path(
            INPUT ${R_LIB}
            OUTPUT P_LIB
        )
        set_target_properties(
            ${NAME} PROPERTIES
            3RD_LIB "${P_LIB}"
        )
        message("LIB path: ${P_LIB}")
    endif()

    if (DEFINED PARAMS_RUNTIME_DEP)
        expand_3rd_path_expression(
            INPUT ${PARAMS_RUNTIME_DEP}
            OUTPUT R_RUNTIME_DEP
            SOURCE_DIR ${SOURCE_DIR}
            BINARY_DIR ${BINARY_DIR}
            INSTALL_DIR ${INSTALL_DIR}
        )
        get_3rd_platform_path(
            INPUT ${R_RUNTIME_DEP}
            OUTPUT P_RUNTIME_DEP
        )
        set_target_properties(
            ${NAME} PROPERTIES
            3RD_RUNTIME_DEP "${P_RUNTIME_DEP}"
        )
    endif()
endfunction(add_cmake_project)
