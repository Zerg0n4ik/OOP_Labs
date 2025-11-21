#include "../include/figures.hpp"
#include <iostream>
#include <cmath>

Figure::operator double() const {
    return area();
}

Octagon::Octagon() {
    vertices = {
        std::make_pair(1.0, 0.0),
        std::make_pair(0.707, 0.707),
        std::make_pair(0.0, 1.0),
        std::make_pair(-0.707, 0.707),
        std::make_pair(-1.0, 0.0),
        std::make_pair(-0.707, -0.707),
        std::make_pair(0.0, -1.0),
        std::make_pair(0.707, -0.707)
    };
}

Octagon::Octagon(const std::array<std::pair<double, double>, 8>& points) : vertices(points) {}

std::pair<double, double> Octagon::geometricCenter() const {
    double sumX = 0.0, sumY = 0.0;
    for (const auto& vertex : vertices) {
        sumX += vertex.first;
        sumY += vertex.second;
    }
    return std::make_pair(sumX / 8.0, sumY / 8.0);
}

double Octagon::area() const {
    double area = 0.0;
    for (int i = 0; i < 8; ++i) {
        int j = (i + 1) % 8;
        area += vertices[i].first * vertices[j].second - vertices[j].first * vertices[i].second;
    }
    return std::abs(area) / 2.0;
}

void Octagon::printVertices(std::ostream& os) const {
    for (size_t i = 0; i < vertices.size(); ++i) {
        os << vertices[i].first << " " << vertices[i].second;
        if (i < vertices.size() - 1) {
            os << " ";
        }
    }
}

void Octagon::readVertices(std::istream& is) {
    for (size_t i = 0; i < 8; ++i) {
        is >> vertices[i].first >> vertices[i].second;
    }
}

bool Octagon::operator==(const Figure& other) const {
    const Octagon* octagon = dynamic_cast<const Octagon*>(&other);
    if (!octagon) return false;
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (vertices[i] != octagon->vertices[i]) {
            return false;
        }
    }
    return true;
}

Octagon& Octagon::operator=(const Figure& other) {
    const Octagon* octagon = dynamic_cast<const Octagon*>(&other);
    if (octagon) {
        vertices = octagon->vertices;
    }
    return *this;
}

Octagon::Octagon(const Octagon& other) : vertices(other.vertices) {}

Octagon& Octagon::operator=(const Octagon& other) {
    if (this != &other) {
        vertices = other.vertices;
    }
    return *this;
}

Octagon::Octagon(Octagon&& other) noexcept : vertices(std::move(other.vertices)) {}

Octagon& Octagon::operator=(Octagon&& other) noexcept {
    if (this != &other) {
        vertices = std::move(other.vertices);
    }
    return *this;
}

std::unique_ptr<Figure> Octagon::clone() const {
    return std::make_unique<Octagon>(*this);
}

Triangle::Triangle() {
    vertices = {std::make_pair(0.0, 0.0), std::make_pair(1.0, 0.0), std::make_pair(0.5, 0.866)};
}

Triangle::Triangle(const std::array<std::pair<double, double>, 3>& points) : vertices(points) {}

std::pair<double, double> Triangle::geometricCenter() const {
    double sumX = 0.0, sumY = 0.0;
    for (const auto& vertex : vertices) {
        sumX += vertex.first;
        sumY += vertex.second;
    }
    return std::make_pair(sumX / 3.0, sumY / 3.0);
}

double Triangle::area() const {
    double x1 = vertices[0].first, y1 = vertices[0].second;
    double x2 = vertices[1].first, y2 = vertices[1].second;
    double x3 = vertices[2].first, y3 = vertices[2].second;
    
    return std::abs((x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2)) / 2.0);
}

void Triangle::printVertices(std::ostream& os) const {
    for (size_t i = 0; i < vertices.size(); ++i) {
        os << vertices[i].first << " " << vertices[i].second;
        if (i < vertices.size() - 1) {
            os << " ";
        }
    }
}

void Triangle::readVertices(std::istream& is) {
    for (size_t i = 0; i < 3; ++i) {
        is >> vertices[i].first >> vertices[i].second;
    }
}

bool Triangle::operator==(const Figure& other) const {
    const Triangle* triangle = dynamic_cast<const Triangle*>(&other);
    if (!triangle) return false;
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (vertices[i] != triangle->vertices[i]) {
            return false;
        }
    }
    return true;
}

Triangle& Triangle::operator=(const Figure& other) {
    const Triangle* triangle = dynamic_cast<const Triangle*>(&other);
    if (triangle) {
        vertices = triangle->vertices;
    }
    return *this;
}

Triangle::Triangle(const Triangle& other) : vertices(other.vertices) {}

Triangle& Triangle::operator=(const Triangle& other) {
    if (this != &other) {
        vertices = other.vertices;
    }
    return *this;
}

Triangle::Triangle(Triangle&& other) noexcept : vertices(std::move(other.vertices)) {}

Triangle& Triangle::operator=(Triangle&& other) noexcept {
    if (this != &other) {
        vertices = std::move(other.vertices);
    }
    return *this;
}

std::unique_ptr<Figure> Triangle::clone() const {
    return std::make_unique<Triangle>(*this);
}

Square::Square() {
    vertices = {
        std::make_pair(0.0, 0.0),
        std::make_pair(1.0, 0.0),
        std::make_pair(1.0, 1.0),
        std::make_pair(0.0, 1.0)
    };
}

Square::Square(const std::array<std::pair<double, double>, 4>& points) : vertices(points) {}

std::pair<double, double> Square::geometricCenter() const {
    double sumX = 0.0, sumY = 0.0;
    for (const auto& vertex : vertices) {
        sumX += vertex.first;
        sumY += vertex.second;
    }
    return std::make_pair(sumX / 4.0, sumY / 4.0);
}

double Square::area() const {
    double side1 = std::sqrt(std::pow(vertices[1].first - vertices[0].first, 2) + 
                           std::pow(vertices[1].second - vertices[0].second, 2));
    double side2 = std::sqrt(std::pow(vertices[2].first - vertices[1].first, 2) + 
                           std::pow(vertices[2].second - vertices[1].second, 2));
    return side1 * side2;
}

void Square::printVertices(std::ostream& os) const {
    for (size_t i = 0; i < vertices.size(); ++i) {
        os << vertices[i].first << " " << vertices[i].second;
        if (i < vertices.size() - 1) {
            os << " ";
        }
    }
}

void Square::readVertices(std::istream& is) {
    for (size_t i = 0; i < 4; ++i) {
        is >> vertices[i].first >> vertices[i].second;
    }
}

bool Square::operator==(const Figure& other) const {
    const Square* square = dynamic_cast<const Square*>(&other);
    if (!square) return false;
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (vertices[i] != square->vertices[i]) {
            return false;
        }
    }
    return true;
}

Square& Square::operator=(const Figure& other) {
    const Square* square = dynamic_cast<const Square*>(&other);
    if (square) {
        vertices = square->vertices;
    }
    return *this;
}

Square::Square(const Square& other) : vertices(other.vertices) {}

Square& Square::operator=(const Square& other) {
    if (this != &other) {
        vertices = other.vertices;
    }
    return *this;
}

Square::Square(Square&& other) noexcept : vertices(std::move(other.vertices)) {}

Square& Square::operator=(Square&& other) noexcept {
    if (this != &other) {
        vertices = std::move(other.vertices);
    }
    return *this;
}

std::unique_ptr<Figure> Square::clone() const {
    return std::make_unique<Square>(*this);
}

std::ostream& operator<<(std::ostream& os, const Figure& figure) {
    figure.printVertices(os);
    return os;
}

std::istream& operator>>(std::istream& is, Figure& figure) {
    figure.readVertices(is);
    return is;
}

double calculateTotalArea(const std::vector<Figure*>& figures) {
    double total = 0.0;
    for (const auto& figure : figures) {
        total += figure->area();
    }
    return total;
}

void printAllFiguresInfo(const std::vector<Figure*>& figures) {
    for (size_t i = 0; i < figures.size(); ++i) {
        std::cout << "Figure " << i << ":\n";
        std::cout << "  Vertices: ";
        figures[i]->printVertices(std::cout);
        std::cout << "\n";
        
        auto center = figures[i]->geometricCenter();
        std::cout << "  Geometric center: (" << center.first << ", " << center.second << ")\n";
        
        double area = figures[i]->area();
        std::cout << "  Area: " << area << "\n\n";
    }
}

void removeFigureByIndex(std::vector<Figure*>& figures, size_t index) {
    if (index < figures.size()) {
        delete figures[index];
        figures.erase(figures.begin() + index);
    }
}