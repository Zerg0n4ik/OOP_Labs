#include "include/lab6.hpp"
#include <iostream>

int main() {
    Editor editor;

    ScreenObserver screen;
    FileObserver file;

    editor.attachObserver(&screen);
    editor.attachObserver(&file);

    editor.addNPC("Дракон", "ДракоН1", 10, 10);
    editor.addNPC("Бык", "Бык1", 20, 10);
    editor.addNPC("Жаба", "Жаба1", 15, 10);
    editor.addNPC("Бык", "Бык2", 5, 5);
    editor.addNPC("Дракон", "ДракоН2", 100, 100);

    std::cout << "Все NPC:\n";
    editor.printAll();

    std::cout << "\nБой (радиус = 20):\n";
    editor.battle(20);

    std::cout << "\nОставшиеся NPC:\n";
    editor.printAll();

    std::cout << "Сохранение NPC в файл 'save.txt'\n";
    editor.save("save.txt");

    std::ifstream inFile("save.txt");
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

    Editor editor2;
    editor2.load("save.txt");
    
    std::cout << "Загруженные NPC:\n";
    editor2.printAll();
    
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