# Добавляем исполняемый файл основной программы
add_executable(lab3 
    main.cpp 
    src/Figure.cpp 
    src/Romb.cpp 
    src/Hexagon.cpp 
    src/Pentagon.cpp
)

# Добавляем тесты - ИМЯ ДОЛЖНО БЫТь lab3_tests
add_executable(lab3_tests 
    test/test.cpp 
    src/Figure.cpp 
    src/Romb.cpp 
    src/Hexagon.cpp 
    src/Pentagon.cpp
)

# Связываем тесты с Google Test - ДОЛЖНО БЫТЬ lab3_tests
target_link_libraries(lab3_tests GTest::gtest_main)

# Добавляем include директории
target_include_directories(lab3_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Включаем тестирование
enable_testing()
add_test(NAME lab3_tests COMMAND lab3_tests)