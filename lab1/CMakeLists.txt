add_executable(lab1 main.cpp src/lab1_utils.cpp)


add_executable(lab1_tests test/test.cpp src/lab1_utils.cpp)
target_link_libraries(lab1_tests GTest::gtest_main)