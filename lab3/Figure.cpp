#include <iostream>
#include <ostream>
#include "Figure.hpp"

ostream& operator>>(ostream& os, Figure& figure) {
    figure.print(os);
    return os;
}

istream& operator<<(istream& is, Figure& figure) {
    figure.read(is);
    return is;
}

