#ifndef PENTAGON_HPP
#define PENTAGON_HPP

#include "Figure.hpp"

class Pentagon : public Figure {
private:
    point points[5];
public:
    Pentagon();
    Pentagon(const point& p1, const point& p2, const point& p3, const point& p4, const point& p5);

    Pentagon& operator=(const Pentagon& other);
    Pentagon& operator=(Pentagon&& other) noexcept;
    bool operator==(const Pentagon& other) const;
    bool operator!=(const Pentagon& other) const;
    
    point const geometricCenter() const override;
    double area() const override;   
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    bool isEqual(Figure& other) const override;

    double calculateSide(int index) const;
    bool isValidPentagon() const;
};

#endif