add_executable(lab2 main.cpp src/lab2_utils.cpp)

add_executable(lab2_tests test/test.cpp src/lab2_utils.cpp)
target_link_libraries(lab2_tests GTest::gtest_main)