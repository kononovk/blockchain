unset(CMAKE_INTERPROCEDURAL_OPTIMIZATION)

# GTest
find_package(GTest QUIET)

if (NOT GTEST_FOUND)
    fetchcontent_declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.11.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll")
    fetchcontent_makeavailable(googletest)
    set(GTEST_BOTH_LIBRARIES GTest::gtest GTest::gtest_main)
endif ()

link_libraries(GTest::gtest GTest::gtest_main)

if (ENABLE_LTO)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else ()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
endif ()

add_compile_options(${BLOCKCHAIN_COMPILE_OPTIONS})
add_link_options(${BLOCKCHAIN_LINK_OPTIONS})

# TODO: Add options which shuffle tests list
set(BLOCKCHAIN_UNIT_TESTS
        blockchain
        )
link_libraries(lib${PROJECT_NAME})
set(BLOCKCHAIN_TEST_SOURCES ${BLOCKCHAIN_UNIT_TESTS})

foreach (TEST_SOURCE ${BLOCKCHAIN_TEST_SOURCES})
    string(REPLACE "/" "_" TEST_NAME ${TEST_SOURCE})
    add_executable(${TEST_NAME} ${TEST_SOURCE}.cpp)
    target_sources(${TEST_NAME}
            PRIVATE ${TEST_UTIL_INCLUDES}
            )
    target_include_directories(${TEST_NAME}
            PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
            PRIVATE ${BLOCKCHAIN_SOURCE_DIR}/src
            )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach ()