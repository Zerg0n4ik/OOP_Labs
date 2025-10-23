#include "../include/Pentagon.hpp"
#include <cmath>
#include <iostream>

Pentagon::Pentagon() {
    for (int i = 0; i < 5; ++i) {
        double angle = 2 * M_PI * i / 5.0 - M_PI / 2.0;
        points[i] = point(cos(angle), sin(angle));
    }
}

Pentagon::Pentagon(const point& p1, const point& p2, const point& p3, const point& p4, const point& p5) {
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    points[3] = p4;
    points[4] = p5;
}

Figure::point const Pentagon::geometricCenter() const {
    double sumX = 0, sumY = 0;
    for (int i = 0; i < 5; ++i) {
        sumX += points[i].x;
        sumY += points[i].y;
    }
    return point(sumX / 5.0, sumY / 5.0);
}

double Pentagon::area() const {
    double area = 0.0;
    
    for (int i = 0; i < 5; ++i) {
        int j = (i + 1) % 5;
        area += points[i].x * points[j].y - points[j].x * points[i].y;
    }
    
    return abs(area) / 2.0;
}

void Pentagon::print(std::ostream& os) const {
    for (int i = 0; i < 5; ++i) {
        os << points[i].x << " " << points[i].y;
        if (i < 4) {
            os << " ";
        }
    }
}

void Pentagon::read(std::istream& is) {
    for (int i = 0; i < 5; ++i) {
        is >> points[i].x >> points[i].y;
    }

    if (!isValidPentagon()) {
        std::cout << "Warning: The entered points may not form a valid pentagon!" << std::endl;
    }
}

bool Pentagon::isEqual(Figure& other) const {
    const Pentagon* pentagon = dynamic_cast<const Pentagon*>(&other);
    if (!pentagon) return false;
    
    for (int i = 0; i < 5; ++i) {
        if (points[i] != pentagon->points[i]) {
            return false;
        }
    }
    return true;
}

double Pentagon::calculateSide(int index) const {
    int next = (index + 1) % 5;
    double dx = points[next].x - points[index].x;
    double dy = points[next].y - points[index].y;
    return sqrt(dx * dx + dy * dy);
}

bool Pentagon::isValidPentagon() const {
    for (int i = 0; i < 5; ++i) {
        if (calculateSide(i) <= 0) {
            return false;
        }
    }
    return true;
}

Pentagon& Pentagon::operator=(const Pentagon& other) {
    if (this != &other) {
        for (int i = 0; i < 5; ++i) {
            points[i] = other.points[i];
        }
    }
    return *this;
}

Pentagon& Pentagon::operator=(Pentagon&& other) noexcept {
    if (this != &other) {
        for (int i = 0; i < 5; ++i) {
            points[i] = std::move(other.points[i]);
        }
    }
    return *this;
}

bool Pentagon::operator==(const Pentagon& other) const {
    for (int i = 0; i < 5; ++i) {
        if (points[i] != other.points[i]) {
            return false;
        }
    }
    return true;
}

bool Pentagon::operator!=(const Pentagon& other) const {
    return !(*this == other);
}
