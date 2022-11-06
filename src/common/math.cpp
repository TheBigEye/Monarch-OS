#include "math.h"
#include "typedefs.h"

uint_32 math::log(int_32 x, int_32 n) {
    int_32 ret = 0, num = 1;
    while (num < n) {
        num *= x;
        ret++;
    }

    return ret;
}

uint_32 math::min(uint_32 a, uint_32 b) {
    // A is less than B?, return A, if not B
    return a < b ? a : b;
}

uint_32 math::max(uint_32 a, uint_32 b) {
    // A is greater than B?, return A, if not B
    return a > b ? a : b;
}

uint_32 math::pow(int_32 x, int_32 p) {
    uint_32 ret = 1;
    for (uint_32 i = 0; i < p; i++) {
        ret *= x;
    }
    return ret;
}
