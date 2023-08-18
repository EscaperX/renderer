OPTION(TEST_3RD_COMPILATION "Check if 3rdparty libraries work well" ON)

if (TEST_3RD_COMPILATION)
    add_subdirectory(test_3rd)
endif()