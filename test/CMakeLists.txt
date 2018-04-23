include_directories("${PROJECT_SOURCE_DIR}")

add_executable(string_view_test string_view_test.cpp)
target_link_libraries(string_view_test kvdb gtest gmock_main)
add_test(NAME string_view_test COMMAND string_view_test)

add_executable(status_test status_test.cpp)
target_link_libraries(status_test kvdb gtest gmock_main)
add_test(NAME status_test COMMAND status_test)

add_executable(file_test file_test.cpp)
target_link_libraries(file_test kvdb gtest gmock_main)
add_test(NAME file_test COMMAND file_test)

add_executable(varint_test varint_test.cpp)
target_link_libraries(varint_test kvdb gtest gmock_main)
add_test(NAME varint_test COMMAND varint_test)

add_executable(block_test block_test.cpp)
target_link_libraries(block_test kvdb gtest gmock_main)
add_test(NAME block_test COMMAND block_test)


