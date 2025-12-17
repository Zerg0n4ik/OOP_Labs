cmake_minimum_required(VERSION 3.10)
project(lab7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск пакетов
find_package(Threads REQUIRED)

# Исходные файлы
set(SOURCES
    src/lab7_utils.cpp
)

# Заголовочные файлы
set(HEADERS
    include/lab7.hpp
)

# Основной исполняемый файл - lab7
add_executable(${PROJECT_NAME}
    main.cpp
    ${SOURCES}
)

# Настройка для многопоточности
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# Директории включения
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Флаги компиляции для многопоточности
if(UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE -pthread)
endif()

# Добавляем тесты - ИМЯ ДОЛЖНО БЫТЬ lab7_tests (не lab6_tests!)
add_executable(lab7_tests 
    test/test.cpp 
    src/lab7_utils.cpp
)

# Связываем тесты с Google Test и потоками
target_link_libraries(lab7_tests 
    GTest::gtest_main
    Threads::Threads
)

# Добавляем include директории для тестов
target_include_directories(lab7_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Флаги компиляции для тестов
if(UNIX)
    target_compile_options(lab7_tests PRIVATE -pthread)
endif()

# Включаем тестирование
enable_testing()
add_test(NAME lab7_tests COMMAND lab7_tests)