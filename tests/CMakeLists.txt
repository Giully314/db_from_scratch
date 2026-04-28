include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
FetchContent_MakeAvailable(googletest)

add_executable(scratchdb_tests)
target_sources(scratchdb_tests PRIVATE test_base_functionalities.cpp)
target_link_libraries(scratchdb_tests PRIVATE scratchdb scratchdb_compiler_flags GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(scratchdb_tests)
