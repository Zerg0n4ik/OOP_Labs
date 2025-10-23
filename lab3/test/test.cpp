#include <gtest/gtest.h>
#include "../include/Figure.hpp"
#include "../include/Hexagon.hpp"
#include "../include/Pentagon.hpp"
#include "../include/Romb.hpp"

using namespace std;

// Базовые тесты на создание фигур
TEST(FigureCreationTest, CreateFiguresWithoutCrash) {
    EXPECT_NO_THROW(Hexagon hex);
    EXPECT_NO_THROW(Pentagon pent);
    EXPECT_NO_THROW(Romb romb);
}

// Тесты на вычисление площади
TEST(AreaTest, AreaIsNonNegative) {
    Hexagon hex;
    Pentagon pent;
    Romb romb;
    
    EXPECT_GE(hex.area(), 0.0);
    EXPECT_GE(pent.area(), 0.0);
    EXPECT_GE(romb.area(), 0.0);
}

// Тесты на геометрический центр
TEST(GeometricCenterTest, CenterExists) {
    Hexagon hex;
    Pentagon pent;
    Romb romb;
    
    // Просто проверяем, что метод работает без исключений
    EXPECT_NO_THROW(hex.geometricCenter());
    EXPECT_NO_THROW(pent.geometricCenter());
    EXPECT_NO_THROW(romb.geometricCenter());
}

// Тесты на полиморфизм
TEST(PolymorphismTest, VirtualFunctionsWork) {
    Figure* hex = new Hexagon();
    Figure* pent = new Pentagon();
    Figure* romb = new Romb();
    
    // Проверяем, что виртуальные методы можно вызвать
    EXPECT_NO_THROW(hex->geometricCenter());
    EXPECT_NO_THROW(hex->area());
    
    EXPECT_NO_THROW(pent->geometricCenter());
    EXPECT_NO_THROW(pent->area());
    
    EXPECT_NO_THROW(romb->geometricCenter());
    EXPECT_NO_THROW(romb->area());
    
    delete hex;
    delete pent;
    delete romb;
}

// Тесты на оператор приведения к double
TEST(DoubleConversionTest, ConversionToDoubleWorks) {
    Hexagon hex;
    Pentagon pent;
    Romb romb;
    
    double area1 = hex.area();
    double area2 = static_cast<double>(hex);
    EXPECT_DOUBLE_EQ(area1, area2);
    
    area1 = pent.area();
    area2 = static_cast<double>(pent);
    EXPECT_DOUBLE_EQ(area1, area2);
    
    area1 = romb.area();
    area2 = static_cast<double>(romb);
    EXPECT_DOUBLE_EQ(area1, area2);
}

// Тесты на пользовательские фигуры
TEST(CustomFigureTest, CustomHexagon) {
    // Создаем простой шестиугольник
    Figure::point p1(0, 0), p2(1, 0), p3(2, 1), p4(1, 2), p5(0, 2), p6(-1, 1);
    Hexagon hex(p1, p2, p3, p4, p5, p6);
    
    EXPECT_GE(hex.area(), 0.0);
    EXPECT_NO_THROW(hex.geometricCenter());
}

TEST(CustomFigureTest, CustomPentagon) {
    // Создаем простой пятиугольник
    Figure::point p1(0, 0), p2(2, 0), p3(3, 2), p4(1, 3), p5(-1, 2);
    Pentagon pent(p1, p2, p3, p4, p5);
    
    EXPECT_GE(pent.area(), 0.0);
    EXPECT_NO_THROW(pent.geometricCenter());
}

TEST(CustomFigureTest, CustomRomb) {
    // Создаем правильный ромб
    Figure::point p1(0, 2), p2(2, 0), p3(0, -2), p4(-2, 0);
    Romb romb(p1, p2, p3, p4);
    
    EXPECT_GE(romb.area(), 0.0);
    EXPECT_NO_THROW(romb.geometricCenter());
}

// Тесты на равенство/неравенство
TEST(EqualityTest, SameFiguresAreEqual) {
    Hexagon hex1;
    Hexagon hex2;
    
    Pentagon pent1;
    Pentagon pent2;
    
    Romb romb1;
    Romb romb2;
    
    EXPECT_TRUE(hex1 == hex2);
    EXPECT_FALSE(hex1 != hex2);
    
    EXPECT_TRUE(pent1 == pent2);
    EXPECT_FALSE(pent1 != pent2);
    
    EXPECT_TRUE(romb1 == romb2);
    EXPECT_FALSE(romb1 != romb2);
}

// Тесты на операторы присваивания
TEST(AssignmentTest, AssignmentWorks) {
    Hexagon hex1;
    Hexagon hex2;
    hex2 = hex1;
    
    Pentagon pent1;
    Pentagon pent2;
    pent2 = pent1;
    
    Romb romb1;
    Romb romb2;
    romb2 = romb1;
    
    EXPECT_TRUE(hex1 == hex2);
    EXPECT_TRUE(pent1 == pent2);
    EXPECT_TRUE(romb1 == romb2);
}

// Простые тесты производительности
TEST(PerformanceTest, MultipleCalculations) {
    Hexagon hex;
    
    for (int i = 0; i < 10; ++i) { // Уменьшили количество итераций
        EXPECT_GE(hex.area(), 0.0);
        EXPECT_NO_THROW(hex.geometricCenter());
    }
}

// Тесты на граничные случаи
TEST(EdgeCaseTest, LargeCoordinates) {
    Figure::point p1(1000, 1000), p2(1001, 1000), p3(1001, 1001), p4(1000, 1001);
    Romb romb(p1, p2, p3, p4);
    
    EXPECT_GE(romb.area(), 0.0);
    EXPECT_NO_THROW(romb.geometricCenter());
}

// Тесты на вычисление сторон
TEST(SideCalculationTest, SideLengthNonNegative) {
    Hexagon hex;
    Pentagon pent;
    Romb romb;
    
    for (int i = 0; i < 6; ++i) {
        EXPECT_GE(hex.calculateSide(i), 0.0);
    }
    
    for (int i = 0; i < 5; ++i) {
        EXPECT_GE(pent.calculateSide(i), 0.0);
    }
    
    for (int i = 0; i < 4; ++i) {
        EXPECT_GE(romb.calculateSide(i), 0.0);
    }
}

// Тест на печать и чтение
TEST(IOTest, PrintDoesNotCrash) {
    Hexagon hex;
    Pentagon pent;
    Romb romb;
    
    stringstream ss;
    
    EXPECT_NO_THROW(hex.print(ss));
    EXPECT_NO_THROW(pent.print(ss));
    EXPECT_NO_THROW(romb.print(ss));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}