# Добавляем исполняемый файл основной программы
add_executable(lab5 
    lab5_main.cpp 
    src/lab5_func.cpp
)

# Добавляем тесты - ИМЯ ДОЛЖНО БЫТЬ lab5_tests
add_executable(lab5_tests 
    test/test.cpp 
    src/lab5_func.cpp
)

# Связываем тесты с Google Test - ДОЛЖНО БЫТЬ lab5_tests
target_link_libraries(lab5_tests GTest::gtest_main)

# Добавляем include директории
target_include_directories(lab5 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(lab5_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Включаем тестирование
enable_testing()
add_test(NAME lab5_tests COMMAND lab5_tests)