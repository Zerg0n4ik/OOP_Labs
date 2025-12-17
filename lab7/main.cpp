#include "include/lab7.hpp"
#include <iostream>

int main() {
    Editor editor;

    ScreenObserver screen;
    FileObserver file;

    editor.attachObserver(&screen);
    editor.attachObserver(&file);

    std::cout << "Генерация 50 случайных NPC...\n";
    editor.generateRandomNPCs(50);
    
    editor.runGame(30);

    std::cout << "\n=== Тестирование сохранения/загрузки ===\n";
    
    std::cout << "Сохранение выживших NPC в файл 'survivors.txt'\n";
    editor.save("survivors.txt");

    std::ifstream inFile("survivors.txt");
    if (inFile.good()) {
        std::cout << "Файл успешно создан. Содержимое:\n";
        std::string line;
        while (std::getline(inFile, line)) {
            std::cout << "  " << line << "\n";
        }
    } else {
        std::cout << "Ошибка при создании файла!\n";
    }
    inFile.close();

    std::cout << "\nПроверяем лог-файл 'log.txt'...\n";
    std::ifstream logFile("log.txt");
    if (logFile.good()) {
        std::cout << "Содержимое лог-файла:\n";
        std::string line;
        while (std::getline(logFile, line)) {
            std::cout << "  " << line << "\n";
        }
    } else {
        std::cout << "Лог-файл не найден или пуст.\n";
    }
    logFile.close();

    return 0;
}