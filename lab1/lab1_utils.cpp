#include <algorithm>
#include "lab1_utils.h"

using namespace std;

int maxDiv(int a, int b) {
    int div = 1;
    if (a == 0 || b == 0) {
        div = 0;
        return div;
    }
    for (int i = 1; i <= min(a, b); i++) {
        if (a % i == 0 && b % i == 0) {
            div = i;
        }
    }
    return div;
}
