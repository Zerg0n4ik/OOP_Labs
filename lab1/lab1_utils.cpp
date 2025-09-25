#include <algorithm>
#include "lab1_utils.h"

using namespace std;

int maxDiv(int a, int b) {
    a = abs(a);
    b = abs(b);
    int div = 1;
    if (a == 0 && b != 0) {
        return abs(b);
    }
    if (a != 0 && b == 0) {
        return abs(a);
    }
    if (a == 0 && b == 0) {
        return 0;
    }
    for (int i = 1; i <= min(a, b); i++) {
        if (a % i == 0 && b % i == 0) {
            div = i;
        }
    }
    return abs(div);
}
