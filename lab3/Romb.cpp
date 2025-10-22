#include "Romb.hpp"
#include "Figure.hpp"
#include <cmath>

using namespace std;

Romb::Romb() {
    points[1] = {0, 1};
    points[2] = {1, 0};
    points[3] = {0, -1};
    points[4] = {-1, 0};
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
        sumX = points[i].x;
        sumY = points[i].y;
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

void Romb::print() const {

}