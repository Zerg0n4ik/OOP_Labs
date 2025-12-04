#include <gtest/gtest.h>
#include "../include/lab6.hpp"
#include <sstream>
#include <fstream>
#include <cstdio>

// Тесты для NPCFactory
TEST(NPCFactoryTest, CreateDragon) {
    auto dragon = NPCFactory::create("Дракон", "Dragon1", 10, 20);
    EXPECT_EQ(dragon->getType(), "Дракон");
    EXPECT_EQ(dragon->getName(), "Dragon1");
    EXPECT_EQ(dragon->getX(), 10);
    EXPECT_EQ(dragon->getY(), 20);
}

TEST(NPCFactoryTest, CreateBull) {
    auto bull = NPCFactory::create("Бык", "Bull1", 30, 40);
    EXPECT_EQ(bull->getType(), "Бык");
    EXPECT_EQ(bull->getName(), "Bull1");
    EXPECT_EQ(bull->getX(), 30);
    EXPECT_EQ(bull->getY(), 40);
}

TEST(NPCFactoryTest, CreateFrog) {
    auto frog = NPCFactory::create("Жаба", "Frog1", 50, 60);
    EXPECT_EQ(frog->getType(), "Жаба");
    EXPECT_EQ(frog->getName(), "Frog1");
    EXPECT_EQ(frog->getX(), 50);
    EXPECT_EQ(frog->getY(), 60);
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
    std::remove("log.txt");
    
    {
        // Создаем FileObserver - он откроет файл log.txt
        FileObserver observer;
        // Вызываем запись
        observer.onKill("Бык1", "Жаба1");
    }
    
    // Проверяем что файл log.txt создан
    std::ifstream in("log.txt");
    EXPECT_TRUE(in.good());
    
    // Проверяем содержимое
    if (in.good()) {
        std::string line;
        std::getline(in, line);
        EXPECT_EQ(line, "Бык1 убивает Жаба1");
    }
    
    // Очищаем после теста
    std::remove("log.txt");
}

// Тесты для Editor
TEST(EditorTest, AddNPCValidCoordinates) {
    Editor editor;
    
    EXPECT_NO_THROW(editor.addNPC("Дракон", "D1", 0, 0));
    EXPECT_NO_THROW(editor.addNPC("Бык", "B1", 500, 500));
    EXPECT_NO_THROW(editor.addNPC("Жаба", "F1", 250, 250));
}

TEST(EditorTest, AddNPCInvalidCoordinatesThrows) {
    Editor editor;
    
    EXPECT_THROW(editor.addNPC("Дракон", "D1", -1, 0), std::invalid_argument);
    EXPECT_THROW(editor.addNPC("Бык", "B1", 0, 501), std::invalid_argument);
    EXPECT_THROW(editor.addNPC("Жаба", "F1", 600, 600), std::invalid_argument);
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

// Тесты для битв
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
    
    EXPECT_EQ(buffer.str(), "Dragon1 убивает Bull1\n");
}

TEST(EditorTest, BattleBullKillsFrog) {
    Editor editor;
    ScreenObserver screen;
    editor.attachObserver(&screen);
    
    editor.addNPC("Бык", "Bull1", 0, 0);
    editor.addNPC("Жаба", "Frog1", 5, 0);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    editor.battle(10);
    
    std::cout.rdbuf(old);
    
    EXPECT_EQ(buffer.str(), "Bull1 убивает Frog1\n");
}

TEST(EditorTest, BattleFrogCannotKillDragon) {
    Editor editor;
    ScreenObserver screen;
    editor.attachObserver(&screen);
    
    editor.addNPC("Жаба", "Frog1", 0, 0);
    editor.addNPC("Дракон", "Dragon1", 5, 0);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    editor.battle(10);
    
    std::cout.rdbuf(old);
    
    // Жаба не убивает дракона, но дракон тоже не убивает жабу
    EXPECT_TRUE(buffer.str().empty());
}

TEST(EditorTest, BattleOutOfRangeNoFight) {
    Editor editor;
    ScreenObserver screen;
    editor.attachObserver(&screen);
    
    editor.addNPC("Дракон", "Dragon1", 0, 0);
    editor.addNPC("Бык", "Bull1", 100, 100);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    editor.battle(50);
    
    std::cout.rdbuf(old);
    
    EXPECT_TRUE(buffer.str().empty());
}

TEST(EditorTest, BattleSameTypeNoFight) {
    Editor editor;
    ScreenObserver screen;
    editor.attachObserver(&screen);
    
    editor.addNPC("Дракон", "Dragon1", 0, 0);
    editor.addNPC("Дракон", "Dragon2", 5, 0);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    editor.battle(10);
    
    std::cout.rdbuf(old);
    
    EXPECT_TRUE(buffer.str().empty());
}

TEST(EditorTest, BattleChainReaction) {
    Editor editor;
    ScreenObserver screen;
    editor.attachObserver(&screen);
    
    // Дракон убивает быка, бык убивает жабу, жаба никого не убивает
    editor.addNPC("Дракон", "Dragon1", 0, 0);
    editor.addNPC("Бык", "Bull1", 5, 0);
    editor.addNPC("Жаба", "Frog1", 10, 0);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    editor.battle(15);
    
    std::cout.rdbuf(old);
    
    // Дракон должен убить быка
    EXPECT_NE(buffer.str().find("Dragon1 убивает Bull1"), std::string::npos);
}

TEST(EditorTest, SaveAndLoadNPCs) {
    Editor editor1;
    editor1.addNPC("Дракон", "Dragon1", 10, 20);
    editor1.addNPC("Бык", "Bull1", 30, 40);
    editor1.addNPC("Жаба", "Frog1", 50, 60);
    
    editor1.save("test_save.txt");
    
    Editor editor2;
    editor2.load("test_save.txt");
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    editor2.printAll();
    
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    EXPECT_NE(output.find("Дракон Dragon1 (10,20)"), std::string::npos);
    EXPECT_NE(output.find("Бык Bull1 (30,40)"), std::string::npos);
    EXPECT_NE(output.find("Жаба Frog1 (50,60)"), std::string::npos);
    
    std::remove("test_save.txt");
}

TEST(EditorTest, LoadNonexistentFile) {
    Editor editor;
    
    // Загрузка несуществующего файла не должна падать
    EXPECT_NO_THROW(editor.load("nonexistent.txt"));
    // Просто очистит npcs
}

// Тесты для FightVisitor
TEST(FightVisitorTest, FightResults) {
    std::vector<Observer*> observers;
    ScreenObserver screen;
    observers.push_back(&screen);
    
    FightVisitor visitor(observers);
    
    Dragon dragon("Dragon1", 0, 0);
    Bull bull("Bull1", 0, 0);
    Frog frog("Frog1", 0, 0);
    
    // Дракон убивает быка
    std::stringstream buffer1;
    std::streambuf* old1 = std::cout.rdbuf(buffer1.rdbuf());
    EXPECT_EQ(visitor.fight(dragon, bull), 1);
    std::cout.rdbuf(old1);
    EXPECT_EQ(buffer1.str(), "Dragon1 убивает Bull1\n");
    
    // Бык убивает жабу
    std::stringstream buffer2;
    std::streambuf* old2 = std::cout.rdbuf(buffer2.rdbuf());
    EXPECT_EQ(visitor.fight(bull, frog), 1);
    std::cout.rdbuf(old2);
    EXPECT_EQ(buffer2.str(), "Bull1 убивает Frog1\n");
    
    // Жаба не убивает дракона
    std::stringstream buffer3;
    std::streambuf* old3 = std::cout.rdbuf(buffer3.rdbuf());
    EXPECT_EQ(visitor.fight(frog, dragon), 0);
    std::cout.rdbuf(old3);
    EXPECT_TRUE(buffer3.str().empty());
    
    // Два дракона не дерутся
    Dragon dragon2("Dragon2", 0, 0);
    std::stringstream buffer4;
    std::streambuf* old4 = std::cout.rdbuf(buffer4.rdbuf());
    EXPECT_EQ(visitor.fight(dragon, dragon2), 0);
    std::cout.rdbuf(old4);
    EXPECT_TRUE(buffer4.str().empty());
}

// Интеграционный тест
TEST(IntegrationTest, CompleteScenario) {
    Editor editor;
    ScreenObserver screen;
    FileObserver file;
    
    editor.attachObserver(&screen);
    editor.attachObserver(&file);
    
    // Добавляем разных NPC
    editor.addNPC("Дракон", "FireDragon", 0, 0);
    editor.addNPC("Бык", "AngryBull", 10, 0);
    editor.addNPC("Жаба", "BigFrog", 20, 0);
    editor.addNPC("Бык", "CowardBull", 30, 0);
    editor.addNPC("Дракон", "IceDragon", 100, 100);
    
    // Тестируем вывод всех NPC
    std::stringstream buffer1;
    std::streambuf* old1 = std::cout.rdbuf(buffer1.rdbuf());
    editor.printAll();
    std::cout.rdbuf(old1);
    
    std::string output1 = buffer1.str();
    EXPECT_NE(output1.find("FireDragon"), std::string::npos);
    EXPECT_NE(output1.find("AngryBull"), std::string::npos);
    EXPECT_NE(output1.find("BigFrog"), std::string::npos);
    EXPECT_NE(output1.find("CowardBull"), std::string::npos);
    EXPECT_NE(output1.find("IceDragon"), std::string::npos);
    
    // Тестируем битву
    std::stringstream buffer2;
    std::streambuf* old2 = std::cout.rdbuf(buffer2.rdbuf());
    editor.battle(15);
    std::cout.rdbuf(old2);
    
    // Должны быть сообщения об убийствах
    EXPECT_FALSE(buffer2.str().empty());
    
    // Тестируем вывод оставшихся NPC
    std::stringstream buffer3;
    std::streambuf* old3 = std::cout.rdbuf(buffer3.rdbuf());
    editor.printAll();
    std::cout.rdbuf(old3);
    
    EXPECT_FALSE(buffer3.str().empty());
    
    // Сохраняем и загружаем
    editor.save("integration_test.txt");
    
    Editor editor2;
    editor2.load("integration_test.txt");
    
    std::remove("integration_test.txt");
    std::remove("log.txt"); // Удаляем лог-файл созданный FileObserver
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}