#include <iostream>
#include "include/lab2_utils.h"

int main() {
    try {        
        Octal a("451");
        Octal b("321");
        Octal c{1, 5, 6};
        Octal d(4 );
        std::cout << "a = " << a.toString() << std::endl;
        std::cout << "b = " << b.toString() << std::endl;
        std::cout << "c = " << c.toString() << std::endl;
        std::cout << "d = " << d.toString() << std::endl;
        Octal sum = a.add(b);
        std::cout << "a + b = " << sum.toString() << std::endl;
        Octal diff = a.subtract(b);
        std::cout << "a - b = " << diff.toString() << std::endl;
        std::cout << "a == b: " << (a.equals(b) ? "true" : "false") << std::endl;
        std::cout << "a < b: " << (a.lessThan(b) ? "true" : "false") << std::endl;
        std::cout << "a > b: " << (a.greaterThan(b) ? "true" : "false") << std::endl;
        Octal zero;
        Octal one("1");
        std::cout << "zero = " << zero.toString() << std::endl;
        std::cout << "one = " << one.toString() << std::endl;
        std::cout << "zero + one = " << zero.add(one).toString() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка" << std::endl;
    }
    return 0;
}