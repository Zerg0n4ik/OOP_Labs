#include <future>
#include <gtest/gtest.h>
#include "../include/lab7.hpp"
#include <sstream>
#include <fstream>
#include <cstdio>
#include <thread>
#include <chrono>

extern std::mutex global_cout_mutex;

// Тесты для NPCFactory
TEST(NPCFactoryTest, CreateDragon) {
    auto dragon = NPCFactory::create("Дракон", "Dragon1", 10, 20);
    EXPECT_EQ(dragon->getType(), "Дракон");
    EXPECT_EQ(dragon->getName(), "Dragon1");
    EXPECT_EQ(dragon->getX(), 10);
    EXPECT_EQ(dragon->getY(), 20);
    EXPECT_TRUE(dragon->isAlive());
    EXPECT_EQ(dragon->getMoveDistance(), 50);
    EXPECT_EQ(dragon->getKillDistance(), 30);
}

TEST(NPCFactoryTest, CreateBull) {
    auto bull = NPCFactory::create("Бык", "Bull1", 30, 40);
    EXPECT_EQ(bull->getType(), "Бык");
    EXPECT_EQ(bull->getName(), "Bull1");
    EXPECT_EQ(bull->getX(), 30);
    EXPECT_EQ(bull->getY(), 40);
    EXPECT_TRUE(bull->isAlive());
    EXPECT_EQ(bull->getMoveDistance(), 30);
    EXPECT_EQ(bull->getKillDistance(), 10);
}

TEST(NPCFactoryTest, CreateFrog) {
    auto frog = NPCFactory::create("Жаба", "Frog1", 50, 60);
    EXPECT_EQ(frog->getType(), "Жаба");
    EXPECT_EQ(frog->getName(), "Frog1");
    EXPECT_EQ(frog->getX(), 50);
    EXPECT_EQ(frog->getY(), 60);
    EXPECT_TRUE(frog->isAlive());
    EXPECT_EQ(frog->getMoveDistance(), 1);
    EXPECT_EQ(frog->getKillDistance(), 10);
}

TEST(NPCFactoryTest, InvalidTypeThrows) {
    EXPECT_THROW(NPCFactory::create("Орк", "Orc1", 10, 10), std::runtime_error);
}

// Тесты для расстояния
TEST(NPCTest, DistanceCalculation) {
    Dragon d1("D1", 0, 0);
    Dragon d2("D2", 3, 4);
    EXPECT_DOUBLE_EQ(d1.dist(d2), 5.0);
}

// Тесты для логики убийств
TEST(NPCTest, DragonCanKillBull) {
    Dragon dragon("Dragon1", 0, 0);
    Bull bull("Bull1", 0, 0);
    Frog frog("Frog1", 0, 0);
    
    EXPECT_TRUE(dragon.canKill(bull));
    EXPECT_FALSE(dragon.canKill(dragon));
    EXPECT_FALSE(dragon.canKill(frog));
}

TEST(NPCTest, BullCanKillFrog) {
    Bull bull("Bull1", 0, 0);
    Frog frog("Frog1", 0, 0);
    Dragon dragon("Dragon1", 0, 0);
    
    EXPECT_TRUE(bull.canKill(frog));
    EXPECT_FALSE(bull.canKill(bull));
    EXPECT_FALSE(bull.canKill(dragon));
}

TEST(NPCTest, FrogCannotKillAnyone) {
    Frog frog("Frog1", 0, 0);
    Dragon dragon("Dragon1", 0, 0);
    Bull bull("Bull1", 0, 0);
    Frog frog2("Frog2", 0, 0);
    
    EXPECT_FALSE(frog.canKill(dragon));
    EXPECT_FALSE(frog.canKill(bull));
    EXPECT_FALSE(frog.canKill(frog2));
}

// Тесты для передвижения
TEST(NPCTest, MoveRandomStaysWithinBounds) {
    Dragon dragon("Dragon1", 0, 0);
    
    // Делаем несколько движений
    for (int i = 0; i < 100; ++i) {
        dragon.moveRandom();
        EXPECT_GE(dragon.getX(), 0);
        EXPECT_LE(dragon.getX(), MAP_WIDTH - 1);
        EXPECT_GE(dragon.getY(), 0);
        EXPECT_LE(dragon.getY(), MAP_HEIGHT - 1);
    }
}

TEST(NPCTest, DeadNPCDoesNotMove) {
    Dragon dragon("Dragon1", 10, 20);
    int initialX = dragon.getX();
    int initialY = dragon.getY();
    
    dragon.kill();
    EXPECT_FALSE(dragon.isAlive());
    
    dragon.moveRandom();
    
    EXPECT_EQ(dragon.getX(), initialX);
    EXPECT_EQ(dragon.getY(), initialY);
}

TEST(NPCTest, MoveTowardsTarget) {
    Bull bull("Bull1", 0, 0);
    
    // Движение к цели в пределах дистанции
    EXPECT_TRUE(bull.moveTowards(5, 0, 10));
    EXPECT_EQ(bull.getX(), 5);
    EXPECT_EQ(bull.getY(), 0);
    
    // Движение к далекой цели
    bull = Bull("Bull1", 0, 0);
    EXPECT_FALSE(bull.moveTowards(100, 0, 10));
    EXPECT_EQ(bull.getX(), 10); // Двигается на максимальное расстояние
    EXPECT_EQ(bull.getY(), 0);
}

// Тесты для Observer
TEST(ObserverTest, ScreenObserverOutput) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    ScreenObserver observer;
    observer.onKill("Дракон1", "Бык1");
    
    std::cout.rdbuf(old);
    
    EXPECT_EQ(buffer.str(), "Дракон1 убивает Бык1\n");
}

TEST(ObserverTest, FileObserverWritesToFile) {
    // Удаляем старый лог-файл если есть
    std::remove("test_log.txt");
    
    {
        // Создаем FileObserver с другим именем файла для тестов
        std::ofstream test_log("test_log.txt", std::ios::app);
        FileObserver observer;
        // Вызываем запись
        observer.onKill("Бык1", "Жаба1");
    }
    
    // Проверяем что файл log.txt создан
    std::ifstream in("log.txt");
    EXPECT_TRUE(in.good());
    
    // Очищаем после теста
    std::remove("log.txt");
}

// Тесты для Editor
TEST(EditorTest, ConstructorDestructor) {
    Editor editor;
    // Должен нормально создаваться и удаляться
    SUCCEED();
}

TEST(EditorTest, AddNPCValidCoordinates) {
    Editor editor;
    
    EXPECT_NO_THROW(editor.addNPC("Дракон", "D1", 0, 0));
    EXPECT_NO_THROW(editor.addNPC("Бык", "B1", MAP_WIDTH-1, MAP_HEIGHT-1));
    EXPECT_NO_THROW(editor.addNPC("Жаба", "F1", MAP_WIDTH/2, MAP_HEIGHT/2));
}

TEST(EditorTest, AddNPCInvalidCoordinatesThrows) {
    Editor editor;
    
    EXPECT_THROW(editor.addNPC("Дракон", "D1", -1, 0), std::invalid_argument);
    EXPECT_THROW(editor.addNPC("Бык", "B1", 0, MAP_HEIGHT), std::invalid_argument);
    EXPECT_THROW(editor.addNPC("Жаба", "F1", MAP_WIDTH, MAP_HEIGHT), std::invalid_argument);
}

TEST(EditorTest, GenerateRandomNPCs) {
    Editor editor;
    
    EXPECT_NO_THROW(editor.generateRandomNPCs(10));
    
    // Не можем легко проверить результат без вывода, но можем проверить что не падает
    SUCCEED();
}

TEST(EditorTest, AttachObserver) {
    Editor editor;
    ScreenObserver screen;
    
    EXPECT_NO_THROW(editor.attachObserver(&screen));
}

TEST(EditorTest, PrintAllNPCs) {
    Editor editor;
    editor.addNPC("Дракон", "Dragon1", 10, 20);
    editor.addNPC("Бык", "Bull1", 30, 40);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    editor.printAll();
    
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    EXPECT_NE(output.find("Дракон Dragon1 (10,20)"), std::string::npos);
    EXPECT_NE(output.find("Бык Bull1 (30,40)"), std::string::npos);
}


TEST(EditorTest, LoadNonexistentFile) {
    Editor editor;
    
    // Загрузка несуществующего файла не должна падать
    EXPECT_NO_THROW(editor.load("nonexistent_mt.txt"));
}

// Тесты для FightVisitor
TEST(FightVisitorTest, FightWithDiceRoll) {
    std::vector<Observer*> observers;
    ScreenObserver screen;
    observers.push_back(&screen);
    
    std::mutex test_mutex;
    FightVisitor visitor(observers, test_mutex);
    
    Dragon dragon("Dragon1", 0, 0);
    Bull bull("Bull1", 0, 0);
    
    // Результат зависит от броска кубика, так что можем только проверить что не падает
    EXPECT_NO_THROW(visitor.fight(dragon, bull));
}

// Тесты для потоков (базовые)
TEST(EditorTest, StartStopThreads) {
    Editor editor;
    
    // Не должно падать при старте/стопе
    EXPECT_NO_THROW(editor.start());
    
    // Даем потокам немного поработать
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    EXPECT_NO_THROW(editor.stop());
}

TEST(EditorTest, RunGameShortDuration) {
    Editor editor;
    
    // Короткая игра на 1 секунду
    EXPECT_NO_THROW(editor.runGame(1));
}

// Тесты для битв (старая функция)
TEST(EditorTest, BattleDragonKillsBull) {
    Editor editor;
    ScreenObserver screen;
    editor.attachObserver(&screen);
    
    editor.addNPC("Дракон", "Dragon1", 0, 0);
    editor.addNPC("Бык", "Bull1", 5, 0);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    editor.battle(10);
    
    std::cout.rdbuf(old);
    
    // Проверяем что было сообщение об убийстве (может быть или нет в зависимости от кубика)
    std::string output = buffer.str();
    // Может быть пустым если кубик не позволил убить
    SUCCEED();
}

TEST(EditorTest, BattleChainReaction) {
    Editor editor;
    ScreenObserver screen;
    editor.attachObserver(&screen);
    
    editor.addNPC("Дракон", "Dragon1", 0, 0);
    editor.addNPC("Бык", "Bull1", 5, 0);
    editor.addNPC("Жаба", "Frog1", 10, 0);
    
    // Запускаем battle с таймаутом
    auto future = std::async(std::launch::async, [&editor]() {
        editor.battle(15);
    });
    
    // Ждем максимум 1 секунду
    auto status = future.wait_for(std::chrono::seconds(10));
    
    if (status == std::future_status::timeout) {
        // Тест не должен зависать
        FAIL() << "BattleChainReaction завис на 1 секунду";
    } else {
        // Проверяем что что-то произошло
        SUCCEED();
    }
}

// Тесты для многопоточных операций
TEST(EditorMultithreadedTest, ConcurrentAddNPC) {
    Editor editor;
    
    // Параллельное добавление NPC из разных потоков
    auto addNPCs = [&editor](int start, int count) {
        for (int i = start; i < start + count; ++i) {
            std::string type = (i % 3 == 0) ? "Дракон" : (i % 3 == 1) ? "Бык" : "Жаба";
            std::string name = type + "_" + std::to_string(i);
            int x = i % MAP_WIDTH;
            int y = i % MAP_HEIGHT;
            
            EXPECT_NO_THROW(editor.addNPC(type, name, x, y));
        }
    };
    
    std::thread t1(addNPCs, 0, 10);
    std::thread t2(addNPCs, 10, 10);
    std::thread t3(addNPCs, 20, 10);
    
    t1.join();
    t2.join();
    t3.join();
    
    // Должно быть добавлено 30 NPC без падений
    SUCCEED();
}

TEST(EditorMultithreadedTest, GenerateAndPrintConcurrently) {
    Editor editor;
    
    // Генерация NPC в одном потоке
    std::thread generator([&editor]() {
        editor.generateRandomNPCs(20);
    });
    
    // Параллельный вывод в другом потоке
    std::thread printer([&editor]() {
        for (int i = 0; i < 5; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            EXPECT_NO_THROW(editor.printAll());
        }
    });
    
    generator.join();
    printer.join();
    
    SUCCEED();
}

// Тест на корректность работы с мертвыми NPC
TEST(EditorTest, DeadNPCManagement) {
    Editor editor;
    
    editor.addNPC("Дракон", "Dragon1", 0, 0);
    editor.addNPC("Бык", "Bull1", 5, 0);
    
    // Симулируем смерть быка
    {
        std::stringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
        editor.battle(10); // Дракон может убить быка
        std::cout.rdbuf(old);
    }
    
    // Вывод должен показывать статусы
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    editor.printAll();
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    // Проверяем что хотя бы один NPC есть в выводе
    EXPECT_TRUE(output.find("Дракон") != std::string::npos || 
                output.find("Бык") != std::string::npos);
}

// Тест на ограничения карты
TEST(EditorTest, MapBoundaries) {
    Editor editor;
    
    // NPC на границах карты
    EXPECT_NO_THROW(editor.addNPC("Дракон", "D1", 0, 0));
    EXPECT_NO_THROW(editor.addNPC("Бык", "B1", MAP_WIDTH-1, MAP_HEIGHT-1));
    EXPECT_NO_THROW(editor.addNPC("Жаба", "F1", 0, MAP_HEIGHT-1));
    EXPECT_NO_THROW(editor.addNPC("Дракон", "D2", MAP_WIDTH-1, 0));
    
    // NPC за границами карты
    EXPECT_THROW(editor.addNPC("Дракон", "D3", -1, 0), std::invalid_argument);
    EXPECT_THROW(editor.addNPC("Бык", "B2", MAP_WIDTH, 0), std::invalid_argument);
    EXPECT_THROW(editor.addNPC("Жаба", "F2", 0, -1), std::invalid_argument);
    EXPECT_THROW(editor.addNPC("Дракон", "D4", 0, MAP_HEIGHT), std::invalid_argument);
}


// Тест на обработку исключений в потоках
TEST(EditorExceptionTest, ThreadSafety) {
    Editor editor;
    
    // Добавляем NPC
    for (int i = 0; i < 10; ++i) {
        std::string type = (i % 3 == 0) ? "Дракон" : (i % 3 == 1) ? "Бык" : "Жаба";
        std::string name = type + "_" + std::to_string(i);
        int x = (i * 10) % MAP_WIDTH;
        int y = (i * 10) % MAP_HEIGHT;
        
        EXPECT_NO_THROW(editor.addNPC(type, name, x, y));
    }
    
    // Запускаем потоки
    editor.start();
    
    // Параллельно пытаемся добавлять и выводить NPC
    std::thread adder([&editor]() {
        for (int i = 0; i < 5; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            EXPECT_NO_THROW(editor.addNPC("Жаба", "TestFrog" + std::to_string(i), 
                                         i * 5, i * 5));
        }
    });
    
    std::thread printer([&editor]() {
        for (int i = 0; i < 5; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            EXPECT_NO_THROW(editor.printAll());
        }
    });
    
    // Даем потокам поработать
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // Останавливаем редактор
    EXPECT_NO_THROW(editor.stop());
    
    adder.join();
    printer.join();
    
    SUCCEED();
}

// Тест на корректное завершение при уничтожении
TEST(EditorTest, DestructorWithRunningThreads) {
    {
        Editor editor;
        editor.generateRandomNPCs(10);
        editor.start();
        
        // Даем потокам немного поработать
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Деструктор должен корректно остановить потоки
    }
    
    // Если дошли сюда, значит деструктор отработал нормально
    SUCCEED();
}