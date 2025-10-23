#ifndef HEXAGON_HPP
#define HEXAGON_HPP

#include "Figure.hpp"

class Hexagon : public Figure {
private:
    point points[6];

public:
    Hexagon();
    Hexagon(const point& p1, const point& p2, const point& p3, const point& p4, const point& p5, const point& p6);

    Hexagon& operator=(const Hexagon& other);
    Hexagon& operator=(Hexagon&& other) noexcept;
    bool operator==(const Hexagon& other) const;
    bool operator!=(const Hexagon& other) const; 
    
    point const geometricCenter() const override;
    double area() const override;
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    bool isEqual(Figure& other) const override;

    double calculateSide(int index) const;
    bool isValidHexagon() const;
};

#endif