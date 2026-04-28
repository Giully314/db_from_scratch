cmake_minimum_required(VERSION 4.1)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD d0edc3af-4c50-42ea-a356-e2862fe7a444)

project(ScratchDB LANGUAGES CXX)

set(CMAKE_CXX_MODULE_STD 1)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# TODO: set build or release

add_library(scratchdb_compiler_flags)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -print-resource-dir
        OUTPUT_VARIABLE CLANG_RESOURCE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    cmake_path(GET CLANG_RESOURCE_DIR PARENT_PATH CLANG_LIB_DIR)
    cmake_path(GET CLANG_LIB_DIR PARENT_PATH CLANG_LIB_DIR)
    target_compile_options(scratchdb_compiler_flags INTERFACE -Werror -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion -stdlib=libc++)
    target_link_options(scratchdb_compiler_flags INTERFACE -stdlib=libc++ -L${CLANG_LIB_DIR}/c++)
endif()

enable_testing()
add_subdirectory(tests)

add_subdirectory(src)
add_subdirectory(app)