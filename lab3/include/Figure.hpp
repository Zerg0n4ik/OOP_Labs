#ifndef FIGURE_HPP
#define FIGURE_HPP


#include <iostream>
#include <istream>
#include <ostream>
#include <cmath>

using namespace std;



class Figure {
public:
    struct point {
    int x;
    int y;

    point() : x(0), y(0) {}    
    point(int x_val, int y_val) : x(x_val), y(y_val) {}

    bool operator==(const point& other) const {
            return x == other.x && y == other.y;
        }

    bool operator!=(const point& other) const {
            return !(*this == other);
        }
    };

    virtual ~Figure() = default;

    virtual const point geometricCenter() const = 0;
    virtual double area() const = 0;
    virtual void print(ostream& os) const = 0;
    virtual void read(istream& is) = 0;
    virtual bool isEqual(Figure& other) const = 0;

    virtual operator double() {
        return area();
    }
};

ostream& operator<<(ostream& os, const Figure& figure);
istream& operator>>(istream& is, Figure& figure);

#endif 