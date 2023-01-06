add_executable(
    example_test
    test.cpp
)
target_link_libraries(
    example_test
    external
    GTest::gtest_main
)
target_include_directories(
    example_test
    PRIVATE ${SOURCE_DIR}
)

target_compile_options(example_test PRIVATE -Wall -Wextra -Wformat -Wconversion -Wpedantic -Werror)

include(GoogleTest)
gtest_discover_tests(example_test)
