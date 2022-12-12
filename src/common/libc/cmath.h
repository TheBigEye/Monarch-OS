#ifndef CMATH_H_
#define CMATH_H_

typedef struct { unsigned int X; unsigned int Y; } coord;
typedef struct { unsigned int X; unsigned int Y; } point;

class math {
    public:
        static unsigned int min(unsigned int a, unsigned int b);
        static unsigned int max(unsigned int a, unsigned int b);
        static unsigned int log(signed int x, signed int n);
        static unsigned int pow(signed int x, signed int p);

        static unsigned int abs(signed int x);
        static float sqrt(float x);
        static unsigned int sqr(unsigned int x);
};

#endif /* CMATH_H_ */
