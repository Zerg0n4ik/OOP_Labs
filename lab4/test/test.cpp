#include <gtest/gtest.h>
#include "../include/figures.hpp"
#include <sstream>
#include <cmath>
#include <vector>
#include <memory>
#include <array>

using namespace std;

// Вспомогательные функции для создания тестовых фигур
unique_ptr<Octagon> createTestOctagon() {
    return make_unique<Octagon>(array<pair<double, double>, 8>{
        make_pair(1.0, 0.0),
        make_pair(0.707, 0.707),
        make_pair(0.0, 1.0),
        make_pair(-0.707, 0.707),
        make_pair(-1.0, 0.0),
        make_pair(-0.707, -0.707),
        make_pair(0.0, -1.0),
        make_pair(0.707, -0.707)
    });
}

unique_ptr<Triangle> createTestTriangle() {
    return make_unique<Triangle>(array<pair<double, double>, 3>{
        make_pair(0, 0), make_pair(3, 0), make_pair(0, 4)
    });
}

unique_ptr<Square> createTestSquare() {
    return make_unique<Square>(array<pair<double, double>, 4>{
        make_pair(0, 0), make_pair(2, 0), make_pair(2, 2), make_pair(0, 2)
    });
}

TEST(FigureTest, OctagonAreaAndCenter) {
    auto oct = createTestOctagon();
    EXPECT_NEAR(static_cast<double>(*oct), 2.828, 1e-3);
    auto center = oct->geometricCenter();
    EXPECT_NEAR(center.first, 0.0, 1e-6);
    EXPECT_NEAR(center.second, 0.0, 1e-6);
}

TEST(FigureTest, TriangleAreaAndCenter) {
    auto tri = createTestTriangle();
    EXPECT_NEAR(static_cast<double>(*tri), 6.0, 1e-6);
    auto center = tri->geometricCenter();
    EXPECT_NEAR(center.first, 1.0, 1e-6);
    EXPECT_NEAR(center.second, 1.333, 1e-3);
}

TEST(FigureTest, SquareAreaAndCenter) {
    auto square = createTestSquare();
    EXPECT_NEAR(static_cast<double>(*square), 4.0, 1e-6);
    auto center = square->geometricCenter();
    EXPECT_NEAR(center.first, 1.0, 1e-6);
    EXPECT_NEAR(center.second, 1.0, 1e-6);
}

TEST(FigureTest, OctagonInputOutput) {
    Octagon oct;
    istringstream iss("1 0 0.707 0.707 0 1 -0.707 0.707 -1 0 -0.707 -0.707 0 -1 0.707 -0.707");
    iss >> oct;
    
    EXPECT_NEAR(static_cast<double>(oct), 2.828, 1e-3);
    
    ostringstream oss;
    oss << oct;
    string output = oss.str();
    EXPECT_FALSE(output.empty());
}

TEST(FigureTest, TriangleInputOutput) {
    Triangle tri;
    istringstream iss("0 0 3 0 0 4");
    iss >> tri;
    
    EXPECT_NEAR(static_cast<double>(tri), 6.0, 1e-6);
    
    ostringstream oss;
    oss << tri;
    string output = oss.str();
    EXPECT_FALSE(output.empty());
}

TEST(FigureTest, SquareInputOutput) {
    Square square;
    istringstream iss("0 0 2 0 2 2 0 2");
    iss >> square;
    
    EXPECT_NEAR(static_cast<double>(square), 4.0, 1e-6);
    
    ostringstream oss;
    oss << square;
    string output = oss.str();
    EXPECT_FALSE(output.empty());
}

TEST(CopyMoveTest, OctagonCopyAndMove) {
    auto oct1 = createTestOctagon();
    double original_area = static_cast<double>(*oct1);
    
    // Copy constructor
    Octagon oct2(*oct1);
    EXPECT_NEAR(static_cast<double>(oct2), original_area, 1e-6);
    EXPECT_TRUE(*oct1 == oct2);
    
    // Copy assignment - ИСПОЛЬЗОВАТЬ operator=(const Figure&)
    Octagon oct3;
    oct3 = static_cast<const Figure&>(*oct1); // Явное приведение к базовому классу
    EXPECT_NEAR(static_cast<double>(oct3), original_area, 1e-6);
    EXPECT_TRUE(*oct1 == oct3);
    
    // Move constructor
    Octagon oct4(move(*oct1));
    EXPECT_NEAR(static_cast<double>(oct4), original_area, 1e-6);
    
    // Move assignment
    Octagon oct5;
    oct5 = move(oct4);
    EXPECT_NEAR(static_cast<double>(oct5), original_area, 1e-6);
}

TEST(CopyMoveTest, TriangleCopyAndMove) {
    auto tri1 = createTestTriangle();
    double original_area = static_cast<double>(*tri1);
    
    // Copy constructor
    Triangle tri2(*tri1);
    EXPECT_NEAR(static_cast<double>(tri2), original_area, 1e-6);
    EXPECT_TRUE(*tri1 == tri2);
    
    // Copy assignment - ИСПОЛЬЗОВАТЬ operator=(const Figure&)
    Triangle tri3;
    tri3 = static_cast<const Figure&>(*tri1); // Явное приведение к базовому классу
    EXPECT_NEAR(static_cast<double>(tri3), original_area, 1e-6);
    EXPECT_TRUE(*tri1 == tri3);
    
    // Move constructor
    Triangle tri4(move(*tri1));
    EXPECT_NEAR(static_cast<double>(tri4), original_area, 1e-6);
    
    // Move assignment
    Triangle tri5;
    tri5 = move(tri4);
    EXPECT_NEAR(static_cast<double>(tri5), original_area, 1e-6);
}

TEST(CopyMoveTest, SquareCopyAndMove) {
    auto square1 = createTestSquare();
    double original_area = static_cast<double>(*square1);
    
    // Copy constructor
    Square square2(*square1);
    EXPECT_NEAR(static_cast<double>(square2), original_area, 1e-6);
    EXPECT_TRUE(*square1 == square2);
    
    // Copy assignment - ИСПОЛЬЗОВАТЬ operator=(const Figure&)
    Square square3;
    square3 = static_cast<const Figure&>(*square1); // Явное приведение к базовому классу
    EXPECT_NEAR(static_cast<double>(square3), original_area, 1e-6);
    EXPECT_TRUE(*square1 == square3);
    
    // Move constructor
    Square square4(move(*square1));
    EXPECT_NEAR(static_cast<double>(square4), original_area, 1e-6);
    
    // Move assignment
    Square square5;
    square5 = move(square4);
    EXPECT_NEAR(static_cast<double>(square5), original_area, 1e-6);
}

TEST(ComparisonTest, OctagonEquality) {
    array<pair<double, double>, 8> vertices1 = {{
        make_pair(1.0, 0.0),
        make_pair(0.707, 0.707),
        make_pair(0.0, 1.0),
        make_pair(-0.707, 0.707),
        make_pair(-1.0, 0.0),
        make_pair(-0.707, -0.707),
        make_pair(0.0, -1.0),
        make_pair(0.707, -0.707)
    }};
    array<pair<double, double>, 8> vertices2 = {{
        make_pair(1.0, 0.0),
        make_pair(0.707, 0.707),
        make_pair(0.0, 1.0),
        make_pair(-0.707, 0.707),
        make_pair(-1.0, 0.0),
        make_pair(-0.707, -0.707),
        make_pair(0.0, -1.0),
        make_pair(0.707, -0.707)
    }};
    
    Octagon oct1(vertices1);
    Octagon oct2(vertices2);
    EXPECT_TRUE(oct1 == oct2);
}

TEST(ComparisonTest, TriangleEquality) {
    array<pair<double, double>, 3> vertices1 = {{{0, 0}, {3, 0}, {0, 4}}};
    array<pair<double, double>, 3> vertices2 = {{{0, 0}, {3, 0}, {0, 4}}};
    
    Triangle tri1(vertices1);
    Triangle tri2(vertices2);
    EXPECT_TRUE(tri1 == tri2);
}

TEST(ComparisonTest, SquareEquality) {
    array<pair<double, double>, 4> vertices1 = {{{0, 0}, {2, 0}, {2, 2}, {0, 2}}};
    array<pair<double, double>, 4> vertices2 = {{{0, 0}, {2, 0}, {2, 2}, {0, 2}}};
    
    Square square1(vertices1);
    Square square2(vertices2);
    EXPECT_TRUE(square1 == square2);
}

TEST(ComparisonTest, OctagonInequality) {
    array<pair<double, double>, 8> vertices1 = {{
        make_pair(1.0, 0.0),
        make_pair(0.707, 0.707),
        make_pair(0.0, 1.0),
        make_pair(-0.707, 0.707),
        make_pair(-1.0, 0.0),
        make_pair(-0.707, -0.707),
        make_pair(0.0, -1.0),
        make_pair(0.707, -0.707)
    }};
    array<pair<double, double>, 8> vertices2 = {{
        make_pair(2.0, 0.0),
        make_pair(1.414, 1.414),
        make_pair(0.0, 2.0),
        make_pair(-1.414, 1.414),
        make_pair(-2.0, 0.0),
        make_pair(-1.414, -1.414),
        make_pair(0.0, -2.0),
        make_pair(1.414, -1.414)
    }};
    
    Octagon oct1(vertices1);
    Octagon oct2(vertices2);
    EXPECT_FALSE(oct1 == oct2);
}

TEST(ComparisonTest, TriangleInequality) {
    array<pair<double, double>, 3> vertices1 = {{{0, 0}, {3, 0}, {0, 4}}};
    array<pair<double, double>, 3> vertices2 = {{{0, 0}, {4, 0}, {0, 3}}};
    
    Triangle tri1(vertices1);
    Triangle tri2(vertices2);
    EXPECT_FALSE(tri1 == tri2);
}

TEST(ComparisonTest, SquareInequality) {
    array<pair<double, double>, 4> vertices1 = {{{0, 0}, {2, 0}, {2, 2}, {0, 2}}};
    array<pair<double, double>, 4> vertices2 = {{{0, 0}, {3, 0}, {3, 3}, {0, 3}}};
    
    Square square1(vertices1);
    Square square2(vertices2);
    EXPECT_FALSE(square1 == square2);
}

TEST(ComparisonTest, DifferentTypesNotEqual) {
    Octagon oct;
    Triangle tri;
    Square square;
    
    EXPECT_FALSE(oct == tri);
    EXPECT_FALSE(oct == square);
    EXPECT_FALSE(tri == square);
}

TEST(PolymorphismTest, PolymorphicBehavior) {
    vector<unique_ptr<Figure>> figures;
    figures.push_back(make_unique<Octagon>());
    figures.push_back(make_unique<Triangle>());
    figures.push_back(make_unique<Square>());
    
    EXPECT_EQ(figures.size(), 3);
    
    for (const auto& fig : figures) {
        EXPECT_GE(static_cast<double>(*fig), 0);
        auto center = fig->geometricCenter();
        EXPECT_FALSE(isnan(center.first));
        EXPECT_FALSE(isnan(center.second));
        
        // Test output stream operator
        ostringstream oss;
        oss << *fig;
        EXPECT_FALSE(oss.str().empty());
    }
}

TEST(PolymorphismTest, VectorOfFiguresTotalArea) {
    vector<unique_ptr<Figure>> figures;
    figures.push_back(make_unique<Octagon>(array<pair<double, double>, 8>{{
        make_pair(1.0, 0.0),
        make_pair(0.707, 0.707),
        make_pair(0.0, 1.0),
        make_pair(-0.707, 0.707),
        make_pair(-1.0, 0.0),
        make_pair(-0.707, -0.707),
        make_pair(0.0, -1.0),
        make_pair(0.707, -0.707)
    }}));
    figures.push_back(make_unique<Triangle>(array<pair<double, double>, 3>{{{0, 0}, {3, 0}, {0, 4}}}));
    figures.push_back(make_unique<Square>(array<pair<double, double>, 4>{{{0, 0}, {2, 0}, {2, 2}, {0, 2}}}));
    
    double total_area = 0;
    for (const auto& fig : figures) {
        total_area += static_cast<double>(*fig);
    }
    
    EXPECT_NEAR(total_area, 2.828 + 6.0 + 4.0, 1e-3);
    EXPECT_EQ(figures.size(), 3);
}

TEST(PolymorphismTest, CloneMethod) {
    auto oct = createTestOctagon();
    auto tri = createTestTriangle();
    auto square = createTestSquare();
    
    auto oct_clone = oct->clone();
    auto tri_clone = tri->clone();
    auto square_clone = square->clone();
    
    EXPECT_TRUE(*oct_clone == *oct);
    EXPECT_TRUE(*tri_clone == *tri);
    EXPECT_TRUE(*square_clone == *square);
    
    EXPECT_NEAR(static_cast<double>(*oct_clone), static_cast<double>(*oct), 1e-6);
    EXPECT_NEAR(static_cast<double>(*tri_clone), static_cast<double>(*tri), 1e-6);
    EXPECT_NEAR(static_cast<double>(*square_clone), static_cast<double>(*square), 1e-6);
}

TEST(EdgeCaseTest, DefaultConstructors) {
    Octagon oct;
    Triangle tri;
    Square square;
    
    EXPECT_GE(static_cast<double>(oct), 0);
    EXPECT_GE(static_cast<double>(tri), 0);
    EXPECT_GE(static_cast<double>(square), 0);
    
    auto center_oct = oct.geometricCenter();
    auto center_tri = tri.geometricCenter();
    auto center_square = square.geometricCenter();
    
    EXPECT_FALSE(isnan(center_oct.first));
    EXPECT_FALSE(isnan(center_oct.second));
    EXPECT_FALSE(isnan(center_tri.first));
    EXPECT_FALSE(isnan(center_tri.second));
    EXPECT_FALSE(isnan(center_square.first));
    EXPECT_FALSE(isnan(center_square.second));
}

TEST(EdgeCaseTest, ZeroAreaFigures) {
    // Degenerate triangle (points on a line)
    array<pair<double, double>, 3> degenerate_triangle = {{
        {0, 0}, {1, 0}, {2, 0}
    }};
    
    Triangle tri(degenerate_triangle);
    EXPECT_NEAR(static_cast<double>(tri), 0.0, 1e-6);
    
    // Very small square
    array<pair<double, double>, 4> tiny_square = {{
        {0, 0}, {0.0001, 0}, {0.0001, 0.0001}, {0, 0.0001}
    }};
    
    Square square(tiny_square);
    EXPECT_NEAR(static_cast<double>(square), 0.00000001, 1e-10);
}

TEST(ArrayFunctionsTest, CalculateTotalArea) {
    vector<Figure*> figures;
    
    figures.push_back(new Octagon(array<pair<double, double>, 8>{{
        make_pair(1.0, 0.0),
        make_pair(0.707, 0.707),
        make_pair(0.0, 1.0),
        make_pair(-0.707, 0.707),
        make_pair(-1.0, 0.0),
        make_pair(-0.707, -0.707),
        make_pair(0.0, -1.0),
        make_pair(0.707, -0.707)
    }}));
    figures.push_back(new Triangle(array<pair<double, double>, 3>{{{0, 0}, {3, 0}, {0, 4}}}));
    figures.push_back(new Square(array<pair<double, double>, 4>{{{0, 0}, {2, 0}, {2, 2}, {0, 2}}}));
    
    double total_area = calculateTotalArea(figures);
    EXPECT_NEAR(total_area, 2.828 + 6.0 + 4.0, 1e-3);
    
    // Cleanup
    for (auto fig : figures) {
        delete fig;
    }
}

TEST(ArrayFunctionsTest, RemoveFigureByIndex) {
    vector<Figure*> figures;
    
    figures.push_back(new Octagon());
    figures.push_back(new Triangle());
    figures.push_back(new Square());
    
    size_t original_size = figures.size();
    EXPECT_EQ(original_size, 3);
    
    // Remove middle element
    removeFigureByIndex(figures, 1);
    EXPECT_EQ(figures.size(), 2);
    
    // Remove first element
    removeFigureByIndex(figures, 0);
    EXPECT_EQ(figures.size(), 1);
    
    // Remove last element
    removeFigureByIndex(figures, 0);
    EXPECT_EQ(figures.size(), 0);
    
    // Test removing from empty array (should not crash)
    removeFigureByIndex(figures, 0);
    EXPECT_EQ(figures.size(), 0);
}

TEST(ArrayFunctionsTest, PrintAllFiguresInfo) {
    vector<Figure*> figures;
    
    figures.push_back(new Octagon(array<pair<double, double>, 8>{{
        make_pair(1.0, 0.0),
        make_pair(0.707, 0.707),
        make_pair(0.0, 1.0),
        make_pair(-0.707, 0.707),
        make_pair(-1.0, 0.0),
        make_pair(-0.707, -0.707),
        make_pair(0.0, -1.0),
        make_pair(0.707, -0.707)
    }}));
    figures.push_back(new Triangle(array<pair<double, double>, 3>{{{0, 0}, {1, 0}, {0, 1}}}));
    figures.push_back(new Square(array<pair<double, double>, 4>{{{0, 0}, {1, 0}, {1, 1}, {0, 1}}}));
    
    // Redirect cout to stringstream to capture output
    streambuf* old_cout = cout.rdbuf();
    ostringstream test_output;
    cout.rdbuf(test_output.rdbuf());
    
    printAllFiguresInfo(figures);
    
    // Restore cout
    cout.rdbuf(old_cout);
    
    string output = test_output.str();
    EXPECT_NE(output.find("Figure 0"), string::npos);
    EXPECT_NE(output.find("Figure 1"), string::npos);
    EXPECT_NE(output.find("Figure 2"), string::npos);
    EXPECT_NE(output.find("Geometric center"), string::npos);
    EXPECT_NE(output.find("Area"), string::npos);
    
    // Cleanup
    for (auto fig : figures) {
        delete fig;
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    cout << "Running comprehensive test suite for geometric figures" << endl;
    cout << "Testing: Octagon, Triangle, Square" << endl;
    cout << "Areas: Octagon(2.828), Triangle(3-4-5), Square(2x2)" << endl;
    
    int result = RUN_ALL_TESTS();
    
    cout << "\n=== Test Summary ===" << endl;
    cout << "Core functionality tested:" << endl;
    cout << "- Area and geometric center calculations" << endl;
    cout << "- Input/output operations" << endl;
    cout << "- Copy and move semantics" << endl;
    cout << "- Figure comparisons" << endl;
    cout << "- Polymorphic behavior and cloning" << endl;
    cout << "- Edge cases and error handling" << endl;
    cout << "- Array operations (total area, removal, printing)" << endl;
    
    return result;
}