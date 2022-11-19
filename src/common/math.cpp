#include "math.h"
#include "typedefs.h"

#define M_PI 3.14159265358979323846

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
    uint_32 ret = 1; // Initialize result

    // If x is negative, compute |x| and use the absolute value
    for (uint_32 i = 0; i < p; i++) {
        ret *= x;
    }
    return ret;
}

uint_32 math::abs(int_32 x) {
    // If x is negative, compute |x|
    return x < 0 ? -x : x;
}

float math::sqrt(float x) {
    float x1, x2, guess = 0;

    while((guess * guess) <= x) {
        guess += 0.1F;
    }

    x1 = guess;

    for(int j = 0; j < 10; j++) {
        x2 =  x;
        x2 /= x1;
        x2 += x1;
        x2 /= 2;
        x1 =  x2;
    }

    return x2;
}

uint_32 math::sqr(uint_32 x) {
    return x * x;
}
