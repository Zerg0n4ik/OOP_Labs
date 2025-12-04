add_executable(lab6 
    main.cpp 
    src/lab6_utils.cpp
)

# Добавляем тесты - ИМЯ ДОЛЖНО БЫТЬ lab6_tests
add_executable(lab6_tests 
    test/test.cpp 
    src/lab6_utils.cpp
)

# Связываем тесты с Google Test
target_link_libraries(lab6_tests GTest::gtest_main)

# Добавляем include директории для основного проекта
target_include_directories(lab6 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Добавляем include директории для тестов
target_include_directories(lab6_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Включаем тестирование
enable_testing()
add_test(NAME lab6_tests COMMAND lab6_tests)