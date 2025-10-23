#ifndef ROMB_HPP
#define ROMB_HPP


#include "Figure.hpp"

class Romb : public Figure {
private:
    point points[4];
public:
    Romb();
    Romb(const point& p1, const point& p2, const point& p3, const point& p4);

    Romb& operator=(const Romb& other);
    Romb& operator=(Romb&& other) noexcept;
    bool operator==(const Romb& other) const;
    bool operator!=(const Romb& other) const;

    const Figure::point geometricCenter() const override;
    double area() const override;
    void print(ostream& os) const override;
    void read(istream& is) override;
    bool isEqual(Figure& other) const override;

    double calculateSide(int index) const;
    bool isValidRomb() const;
};

#endif