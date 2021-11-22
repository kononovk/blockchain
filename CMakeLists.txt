cmake_minimum_required(VERSION 3.11)

project(blockchain)
project(blockchain)

set(BLOCKCHAIN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BLOCKCHAIN_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Include guards
if (BLOCKCHAIN_SOURCE_DIR STREQUAL BLOCKCHAIN_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif ()

# Set variables
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)

set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (BLOCKCHAIN_ENABLE_LTO)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
else ()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
endif ()

if (MSVC)
    list(APPEND BLOCKCHAIN_COMPILE_OPTIONS /W3)
    # TODO We have to use /Wall. But MSVC is dumb and uses this flag for external projects, like gtest.
    if (BLOCKCHAIN_SANITIZER)
        message("Sanitizer's work only on Linux")
    endif ()
else ()
    list(APPEND BLOCKCHAIN_COMPILE_OPTIONS -Wall -Wextra -pedantic)
    if (BLOCKCHAIN_SANITIZER STREQUAL OFF)
        message("All sanitizers disabled")
    elseif (BLOCKCHAIN_SANITIZER STREQUAL ASAN)
        set(SANITIZER_ENABLE_OPTION -fsanitize=address)
        set(SANITIZER_COMPILE_OPTIONS
                # -fsanitize-address-use-after-return=runtime
                -fsanitize-recover=address
                -fsanitize-address-use-after-scope
                )
    elseif (BLOCKCHAIN_SANITIZER STREQUAL TSAN)
        set(SANITIZER_ENABLE_OPTION -fsanitize=thread)
        set(SANITIZER_COMPILE_OPTIONS
                -gcolumn-info
                )
    elseif (BLOCKCHAIN_SANITIZER STREQUAL UBSAN)
        set(SANITIZER_ENABLE_OPTION -fsanitize=undefined)
    elseif (BLOCKCHAIN_SANITIZER STREQUAL MEMSAN) # Memory Santizer doesn't work with GTest (https://github.com/google/sanitizers/issues/648)
        set(SANITIZER_ENABLE_OPTION -fsanitize=memory)
    endif ()
    list(APPEND BLOCKCHAIN_COMPILE_OPTIONS
            -fno-omit-frame-pointer
            ${SANITIZER_ENABLE_OPTION}
            ${SANITIZER_COMPILE_OPTIONS}
            )
    list(APPEND BLOCKCHAIN_LINK_OPTIONS ${SANITIZER_ENABLE_OPTION})
endif ()

if (BLOCKCHAIN_COVERAGE AND CMAKE_SYSTEM_NAME STREQUAL Linux)
    list(APPEND BLOCKCHAIN_COMPILE_OPTIONS
            --coverage
            )
    list(APPEND BLOCKCHAIN_LINK_OPTIONS
            --coverage
            )
endif ()
message("BLOCKCHAIN_COMPILE_OPTIONS: ${BLOCKCHAIN_COMPILE_OPTIONS}")
message("BLOCKCHAIN_LINK_OPTIONS   : ${BLOCKCHAIN_LINK_OPTIONS}")
add_definitions(${BLOCKCHAIN_DEFINITIONS})

# Main library directory
add_subdirectory(src)

if (BUILD_TESTING)
    include(FetchContent)
    enable_testing()
    add_subdirectory(test)
endif ()