add_executable(lab3 main.cpp src/Figure.cpp src/Romb.cpp src/Hexagon.cpp src/Pentagon.cpp)

add_executable(lab3_tests test/test.cpp src/Figure.cpp src/Romb.cpp src/Hexagon.cpp src/Pentagon.cpp)
target_link_libraries(lab3_tests GTest::gtest_main)