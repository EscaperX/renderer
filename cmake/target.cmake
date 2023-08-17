function(my_link_libraries)
    cmake_parse_arguments(PARAMS "" "NAME" "LIB" ${ARGN})

    foreach(L ${PARAMS_LIB})
        if (NOT (TARGET ${L}))
            target_link_libraries(${PARAMS_NAME} ${L})
            continue()
        endif()

        get_target_property(3RD_TYPE ${L} 3RD_TYPE)
        if (${3RD_TYPE} STREQUAL "3RD_TYPE-NOTFOUND")
            target_link_libraries(${PARAMS_NAME} ${L})
        else()
            get_target_property(INCLUDE ${L} 3RD_INCLUDE)
            get_target_property(LINK ${L} 3RD_LINK)
            get_target_property(LIB ${L} 3RD_LIB)

            if (NOT ("${INCLUDE}" STREQUAL "INCLUDE-NOTFOUND"))
                target_include_directories(${PARAMS_NAME} PUBLIC ${INCLUDE})
            endif()
            if (NOT ("${LINK}" STREQUAL "LINK-NOTFOUND"))
                target_link_directories(${PARAMS_NAME} PUBLIC ${LINK})
            endif()
            if (NOT ("${LIB}" STREQUAL "LIB-NOTFOUND"))
                target_link_libraries(${PARAMS_NAME} ${LIB})
            endif()

            if (${3RD_TYPE} STREQUAL "CMakeProject")
                add_dependencies(${PARAMS_NAME} ${L})
            endif()
        endif()

        get_target_property(RUNTIME_DEP ${L} 3RD_RUNTIME_DEP)
        if (NOT ("${RUNTIME_DEP}" STREQUAL "RUNTIME_DEP-NOTFOUND"))
            CombineRuntimeDependencies(
                NAME ${PARAMS_NAME}
                RUNTIME_DEP "${RUNTIME_DEP}"
            )
        endif()
    endforeach()

endfunction(my_link_libraries)

function(my_add_executable)
    cmake_parse_arguments(PARAMS "SAMPLE;META" "NAME" "SRC;INC;LINK;LIB;DEP_TARGET;RES" ${ARGN})

    if (${PARAMS_SAMPLE} AND (NOT ${BUILD_SAMPLE}))
        return()
    endif()

    # if (DEFINED PARAMS_REFLECT)
    #     AddMirrorInfoSourceGenerationTarget(
    #         NAME ${PARAMS_NAME}
    #         OUTPUT_SRC GENERATED_SRC
    #         OUTPUT_TARGET_NAME GENERATED_TARGET
    #         SEARCH_DIR ${PARAMS_REFLECT}
    #         PRIVATE_INC ${PARAMS_INC}
    #         LIB ${PARAMS_LIB}
    #     )
    # endif()

    add_executable(
        ${PARAMS_NAME}
        ${PARAMS_SRC}
        ${GENERATED_SRC}
    )
    target_include_directories(
        ${PARAMS_NAME}
        PRIVATE ${PARAMS_INC}
    )
    target_link_directories(
        ${PARAMS_NAME}
        PRIVATE ${PARAMS_LINK}
    )
    my_link_libraries(
        NAME ${PARAMS_NAME}
        LIB ${PARAMS_LIB}
    )
    # AddRuntimeDependenciesCopyCommand(
    #     NAME ${PARAMS_NAME}
    # )
    # AddResourcesCopyCommand(
    #     NAME ${PARAMS_NAME}
    #     RES ${PARAMS_RES}
    # )
    if (DEFINED PARAMS_DEP_TARGET)
        add_dependencies(${PARAMS_NAME} ${PARAMS_DEP_TARGET})
    endif()
    # if (DEFINED PARAMS_REFLECT)
    #     add_dependencies(${PARAMS_NAME} ${GENERATED_TARGET})
    # endif()

    if (${MSVC})
        set_target_properties(${PARAMS_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>)
    endif()
#    IF (${MINGW})
#        target_link_libraries(${PARAMS_NAME} "ws2_32")
#    ENDIF ()
endfunction(my_add_executable)
