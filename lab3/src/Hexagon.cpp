#include "../include/Hexagon.hpp"
#include <cmath>
#include <iostream>

using namespace std;

Hexagon::Hexagon() {
    for (int i = 0; i < 6; ++i) {
        double angle = 2 * M_PI * i / 6.0 - M_PI / 6.0;
        points[i] = point(cos(angle), sin(angle));
    }
}

Hexagon::Hexagon(const point& p1, const point& p2, const point& p3, const point& p4, const point& p5, const point& p6) {
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    points[3] = p4;
    points[4] = p5;
    points[5] = p6;
}

Figure::point const Hexagon::geometricCenter() const {
    double sumX = 0, sumY = 0;
    for (int i = 0; i < 6; ++i) {
        sumX += points[i].x;
        sumY += points[i].y;
    }
    return point(sumX / 6.0, sumY / 6.0);
}

double Hexagon::area() const {
    double area = 0.0;
    
    for (int i = 0; i < 6; ++i) {
        int j = (i + 1) % 6;
        area += points[i].x * points[j].y - points[j].x * points[i].y;
    }
    
    return abs(area) / 2.0;
}

void Hexagon::print(ostream& os) const {
    for (int i = 0; i < 6; ++i) {
        os << points[i].x << " " << points[i].y;
        if (i < 5) {
            os << " ";
        }
    }
}

void Hexagon::read(istream& is) {
    for (int i = 0; i < 6; ++i) {
        is >> points[i].x >> points[i].y;
    }
    
    if (!isValidHexagon()) {
        std::cout << "Warning: The entered points may not form a valid hexagon!" << std::endl;
    }
}

bool Hexagon::isEqual(Figure& other) const {
    const Hexagon* hexagon = dynamic_cast<const Hexagon*>(&other);
    if (!hexagon) return false;
    
    for (int i = 0; i < 6; ++i) {
        if (points[i] != hexagon->points[i]) {
            return false;
        }
    }
    return true;
}

double Hexagon::calculateSide(int index) const {
    int next = (index + 1) % 6;
    double dx = points[next].x - points[index].x;
    double dy = points[next].y - points[index].y;
    return sqrt(dx * dx + dy * dy);
}

bool Hexagon::isValidHexagon() const {
    for (int i = 0; i < 6; ++i) {
        if (calculateSide(i) <= 0) {
            return false;
        }
    }
    return true;
}

Hexagon& Hexagon::operator=(const Hexagon& other) {
    if (this != &other) {
        for (int i = 0; i < 6; ++i) {
            points[i] = other.points[i];
        }
    }
    return *this;
}

Hexagon& Hexagon::operator=(Hexagon&& other) noexcept {
    if (this != &other) {
        for (int i = 0; i < 6; ++i) {
            points[i] = std::move(other.points[i]);
        }
    }
    return *this;
}

bool Hexagon::operator==(const Hexagon& other) const {
    for (int i = 0; i < 6; ++i) {
        if (points[i] != other.points[i]) {
            return false;
        }
    }
    return true;
}

bool Hexagon::operator!=(const Hexagon& other) const {
    return !(*this == other);
}