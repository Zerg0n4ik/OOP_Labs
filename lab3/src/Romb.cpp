#include "../include/Romb.hpp"
#include "../include/Figure.hpp"
#include <cmath>
#include <ostream>

using namespace std;

Romb::Romb() {
    points[0] = {0, 1};
    points[1] = {1, 0};
    points[2] = {0, -1};
    points[3] = {-1, 0};
}

Romb::Romb(const point& p1, const point& p2, const point& p3, const point& p4) {
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    points[3] = p4;
}

const Figure::point Romb::geometricCenter() const {
    int sumX = 0;
    int sumY = 0;
    for (int i = 0; i < 4; i++) {
        sumX += points[i].x;
        sumY += points[i].y;
    }

    return point(sumX / 4, sumY / 4);
} 

double Romb::area() const {
    double d1 = sqrt(pow(points[0].x - points[2].x, 2) + 
                         pow(points[0].y - points[2].y, 2));
    double d2 = sqrt(pow(points[1].x - points[3].x, 2) + 
                         pow(points[1].y - points[3].y, 2));
    
    return (d1 * d2) / 2.0;
}

void Romb::print(std::ostream& os) const {
    for (int i = 0; i < 4; ++i) {
        os << points[i].x << " " << points[i].y;
        if (i < 3) os << " ";
    }
}

void Romb::read(istream& is) {
    
    for (int i = 0; i < 4; ++i) {
        is >> points[i].x >> points[i].y;
    }
    
    if (!isValidRomb()) {
        cout << "Warning: Points may not form a perfect rhombus!" << endl;
    }
}

double Romb::calculateSide(int index) const {
    int next = (index + 1) % 4;
    double dx = points[next].x - points[index].x;
    double dy = points[next].y - points[index].y;
    return sqrt(dx * dx + dy * dy);
}

bool Romb::isValidRomb() const {
    double side1 = calculateSide(0);
    double side2 = calculateSide(1);
    double side3 = calculateSide(2);
    double side4 = calculateSide(3);
    
    double epsilon = 1e-6;
    bool sidesEqual = std::abs(side1 - side2) < epsilon &&
                      std::abs(side2 - side3) < epsilon &&
                      std::abs(side3 - side4) < epsilon;
    
    if (!sidesEqual) {
        std::cout << "Sides are not equal: " << side1 << ", " << side2 
                  << ", " << side3 << ", " << side4 << std::endl;
        return false;
    }
    
    return true;
}

bool Romb::isEqual(Figure& other) const {
    const Romb* romb = dynamic_cast<const Romb*>(&other);
    if (!romb) return false;
    
    for (int i = 0; i < 4; ++i) {
        if (points[i] != romb->points[i]) {
            return false;
        }
    }
    return true;
}

Romb& Romb::operator=(const Romb& other) {
    if (this != &other) {
        for (int i = 0; i < 4; ++i) {
            points[i] = other.points[i];
        }
    }
    return *this;
}

Romb& Romb::operator=(Romb&& other) noexcept {
    if (this != &other) {
        for (int i = 0; i < 4; ++i) {
            points[i] = std::move(other.points[i]);
        }
    }
    return *this;
}

bool Romb::operator==(const Romb& other) const {
    for (int i = 0; i < 4; ++i) {
        if (points[i] != other.points[i]) {
            return false;
        }
    }
    return true;
}

bool Romb::operator!=(const Romb& other) const {
    return !(*this == other);
}