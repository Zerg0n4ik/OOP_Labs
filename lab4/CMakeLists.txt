# Добавляем исполняемый файл основной программы
add_executable(lab4 
    main.cpp 
    src/figures.cpp
)

# Добавляем тесты - ИМЯ ДОЛЖНО БЫТЬ lab4_tests
add_executable(lab4_tests 
    test/test.cpp 
    src/figures.cpp
)

# Связываем тесты с Google Test - ДОЛЖНО БЫТЬ lab4_tests
target_link_libraries(lab4_tests GTest::gtest_main)

# Добавляем include директории
target_include_directories(lab4 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(lab4_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Включаем тестирование
enable_testing()
add_test(NAME lab4_tests COMMAND lab4_tests)