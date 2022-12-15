#include "cmath.h"
#include "../sys/types.h"

#define PI 3.14159265358979323846

uint_32 math::log(int_32 x, int_32 n) {
    int_32 ret = 0;
    while (n >= x) {
        n /= x;
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

    if (p == 0) return 1; // If p is 0, return 1
    if (x < 0)  x = math::abs(x); // If x is negative, compute |x| and use the absolute value

    // Compute x^p using a loop
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
    // Check for invalid input
    if (x < 0) return -1;

    // Use the average of 0 and x as the initial guess
    float x1 = 0.5f * (0 + x);

    // Use the Newton-Raphson method to iteratively improve the guess
    for (int i = 0; i < 100; i++) {
        x1 = 0.5f * (x1 + x / x1);
    }

    return x1;
}

uint_32 math::sqr(uint_32 x) {
    return x * x;
}
