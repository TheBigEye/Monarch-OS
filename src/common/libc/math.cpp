#include "math.h"

#define M_PI 3.14159265358979323846

unsigned int math::log(signed int x, signed int n) {
    signed int ret = 0;
    while (n >= x) {
        n /= x;
        ret++;
    }
    return ret;
}

unsigned int math::min(unsigned int a, unsigned int b) {
    // A is less than B?, return A, if not B
    return a < b ? a : b;
}

unsigned int math::max(unsigned int a, unsigned int b) {
    // A is greater than B?, return A, if not B
    return a > b ? a : b;
}

unsigned int math::pow(signed int x, signed int p) {
    unsigned int ret = 1; // Initialize result

    if (p == 0) return 1; // If p is 0, return 1
    if (x < 0)  x = math::abs(x); // If x is negative, compute |x| and use the absolute value

    // Compute x^p using a loop
    for (unsigned int i = 0; i < p; i++) {
        ret *= x;
    }

    return ret;
}

unsigned int math::abs(signed int x) {
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

unsigned int math::sqr(unsigned int x) {
    return x * x;
}
