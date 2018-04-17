include_directories("${PROJECT_SOURCE_DIR}")

add_executable(string_view_test string_view_test.cpp)
target_link_libraries(string_view_test kvdb gtest gmock_main)
add_test(NAME string_view_test COMMAND string_view_test)

add_executable(status_test status_test.cpp)
target_link_libraries(status_test kvdb gtest gmock_main)
add_test(NAME status_test COMMAND status_test)

