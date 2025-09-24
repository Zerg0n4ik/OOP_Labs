#include <iostream>
#include "lab1_utils.h"

using namespace std;

int main() {
    int a, b, result;
    cout << "Введите первое число: ";
    cin >> a;
    cout << "Введите второе число: ";
    cin >> b;
    result = maxDiv(a, b);
    cout << result << endl; 
    return 0;
}