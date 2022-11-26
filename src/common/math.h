#ifndef MATH_H_
#define MATH_H_

#include "typedefs.h"

typedef struct { uint_32 X; uint_32 Y; } coord;
typedef struct { uint_32 X; uint_32 Y; } point;

class math {
    public:
        static uint_32 min(uint_32 a, uint_32 b);
        static uint_32 max(uint_32 a, uint_32 b);
        static uint_32 log(int_32 x, int_32 n);
        static uint_32 pow(int_32 x, int_32 p);

        static uint_32 abs(int_32 x);
        static float sqrt(float x);
        static uint_32 sqr(uint_32 x);
};

#endif /* MATH_H_ */
