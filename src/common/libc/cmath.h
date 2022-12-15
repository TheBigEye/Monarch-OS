#ifndef _CMATH_H
#define _CMATH_H

#include "../sys/types.h"

typedef struct {
    uint_32 X;
    uint_32 Y;
} point;

namespace math {
    uint_32 min(uint_32 a, uint_32 b);
    uint_32 max(uint_32 a, uint_32 b);
    uint_32 log(int_32 x, int_32 n);
    uint_32 pow(int_32 x, int_32 p);

    uint_32 abs(int_32 x);
    float sqrt(float x);
    uint_32 sqr(uint_32 x);
}

#endif /* cmath.h */
