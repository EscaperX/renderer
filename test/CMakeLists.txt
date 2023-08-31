OPTION(TEST_3RD_COMPILATION "Check if 3rdparty libraries work well" ON)
OPTION(TEST_FEATURE "Check if features work well" ON)


if (TEST_3RD_COMPILATION)
    add_subdirectory(test_3rd)
endif()

if (TEST_FEATURE)
    add_subdirectory(test_feat)
endif()
