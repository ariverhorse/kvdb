include_directories("${PROJECT_SOURCE_DIR}")

add_executable(string_view_test string_view_test.cpp)
target_link_libraries(string_view_test gtest gmock_main)
add_test(NAME sv_test COMMAND string_view_test)

