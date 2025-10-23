#include <iostream>
#include <iomanip>
#include "include/Figure.hpp"
#include "include/Hexagon.hpp"
#include "include/Pentagon.hpp"
#include "include/Romb.hpp"

using namespace std;

const int MAX_FIGURES = 10;

void printMenu() {
    cout << "\n=== МЕНЮ ===" << endl;
    cout << "1. Добавить фигуру" << endl;
    cout << "2. Показать все фигуры" << endl;
    cout << "3. Вычислить общую площадь" << endl;
    cout << "4. Удалить фигуру по индексу" << endl;
    cout << "5. Выйти" << endl;
    cout << "Выберите действие: ";
}

void addFigureMenu(Figure* figures[], int& figureCount) {
    if (figureCount >= MAX_FIGURES) {
        cout << "Массив полон! Максимум " << MAX_FIGURES << " фигур." << endl;
        return;
    }
    
    cout << "\nВыберите тип фигуры:" << endl;
    cout << "1. Шестиугольник (6 точек)" << endl;
    cout << "2. Пятиугольник (5 точек)" << endl;
    cout << "3. Ромб (4 точки)" << endl;
    cout << "Выбор: ";
    
    int choice;
    cin >> choice;
    
    switch (choice) {
        case 1: {
            cout << "Введите 6 пар координат (x y) для шестиугольника:" << endl;
            Figure::point points[6];
            for (int i = 0; i < 6; ++i) {
                cout << "Точка " << i + 1 << ": ";
                cin >> points[i].x >> points[i].y;
            }
            figures[figureCount++] = new Hexagon(points[0], points[1], points[2], 
                                               points[3], points[4], points[5]);
            cout << "Шестиугольник добавлен!" << endl;
            break;
        }
        case 2: {
            cout << "Введите 5 пар координат (x y) для пятиугольника:" << endl;
            Figure::point points[5];
            for (int i = 0; i < 5; ++i) {
                cout << "Точка " << i + 1 << ": ";
                cin >> points[i].x >> points[i].y;
            }
            figures[figureCount++] = new Pentagon(points[0], points[1], points[2], 
                                                points[3], points[4]);
            cout << "Пятиугольник добавлен!" << endl;
            break;
        }
        case 3: {
            cout << "Введите 4 пары координат (x y) для ромба:" << endl;
            Figure::point points[4];
            for (int i = 0; i < 4; ++i) {
                cout << "Точка " << i + 1 << ": ";
                cin >> points[i].x >> points[i].y;
            }
            figures[figureCount++] = new Romb(points[0], points[1], points[2], points[3]);
            cout << "Ромб добавлен!" << endl;
            break;
        }
        default:
            cout << "Неверный выбор!" << endl;
    }
}

void showAllFigures(Figure* figures[], int figureCount) {
    if (figureCount == 0) {
        cout << "Массив пуст!" << endl;
        return;
    }
    
    cout << "\n=== ВСЕ ФИГУРЫ ===" << endl;
    for (int i = 0; i < figureCount; ++i) {
        cout << "Фигура " << i << ":" << endl;
        cout << "  Координаты: ";
        figures[i]->print(cout);
        cout << endl;
        
        Figure::point center = figures[i]->geometricCenter();
        cout << "  Геометрический центр: (" << center.x << ", " << center.y << ")" << endl;
        
        double area = figures[i]->area();
        cout << "  Площадь: " <<  area << endl;
        
    }
}

double calculateTotalArea(Figure* figures[], int figureCount) {
    double total = 0.0;
    for (int i = 0; i < figureCount; ++i) {
        total += figures[i]->area();
    }
    return total;
}

void deleteFigureByIndex(Figure* figures[], int& figureCount) {
    if (figureCount == 0) {
        cout << "Массив пуст!" << endl;
        return;
    }
    
    cout << "Введите индекс для удаления (0-" << figureCount-1 << "): ";
    int index;
    cin >> index;
    
    if (index < 0 || index >= figureCount) {
        cout << "Неверный индекс!" << endl;
        return;
    }
    
    delete figures[index];
    
    for (int i = index; i < figureCount - 1; ++i) {
        figures[i] = figures[i + 1];
    }
    figures[figureCount - 1] = nullptr;
    figureCount--;
    
    cout << "Фигура с индексом " << index << " удалена!" << endl;
}

int main() {
    Figure* figures[MAX_FIGURES] = {nullptr};
    int figureCount = 0;
    int choice;
    
    do {
        printMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                addFigureMenu(figures, figureCount);
                break;
            case 2:
                showAllFigures(figures, figureCount);
                break;
            case 3: {
                double total = calculateTotalArea(figures, figureCount);
                cout << "\nОбщая площадь всех фигур: " << fixed << setprecision(2) << total << endl;
                break;
            }
            case 4:
                deleteFigureByIndex(figures, figureCount);
                break;
            case 5:
                cout << "Выход..." << endl;
                break;
            default:
                cout << "Неверный выбор!" << endl;
        }
    } while (choice != 5);
    
    for (int i = 0; i < figureCount; ++i) {
        delete figures[i];
    }
    
    return 0;
}