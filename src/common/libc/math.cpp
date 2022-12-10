#include "math.h"

#define M_PI 3.14159265358979323846

unsigned int math::log(signed int x, signed int n) {
    signed int ret = 0, num = 1;
    while (num < n) {
        num *= x;
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

    // If x is negative, compute |x| and use the absolute value
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

unsigned int math::sqr(unsigned int x) {
    return x * x;
}
