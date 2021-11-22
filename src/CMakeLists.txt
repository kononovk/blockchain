set(BLOCKCHAIN_SOURCES)
set(BLOCKCHAIN_HEADERS)

macro(add_files)
    set(BLOCKCHAIN_HEADERS ${BLOCKCHAIN_HEADERS} PARENT_SCOPE)
    set(BLOCKCHAIN_SOURCES ${BLOCKCHAIN_SOURCES} PARENT_SCOPE)
endmacro()

add_compile_options(${BLOCKCHAIN_COMPILE_OPTIONS})
add_link_options(${BLOCKCHAIN_LINK_OPTIONS})

add_subdirectory(block)
add_subdirectory(blockchain)
add_subdirectory(utils)

add_library(
        lib${PROJECT_NAME} STATIC
        ${BLOCKCHAIN_SOURCES}
        ${BLOCKCHAIN_HEADERS}
)

target_include_directories(lib${PROJECT_NAME}
        PUBLIC ${BLOCKCHAIN_SOURCE_DIR}/src
        )