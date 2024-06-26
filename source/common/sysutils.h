#ifndef _SYS_UTILS_H
#define _SYS_UTILS_H 1

    // Coss-compiler heaaders
    #include <stdbool.h>
    #include <stdint.h>


    /* Common typedefs */
    #ifndef _COMMON_TYPEDEFS
    #define _COMMON_TYPEDEFS 1

        /**
         * @brief Sometimes we want to keep parameters to a function for later use
         * and this is a solution to avoid the 'unused parameter' compiler warning
         */
        #define UNUSED(x) (void)(x)

        #define NULL ((void *) 0UL)

        #define INF __builtin_inf()
        #define NAN __builtin_nan("")
        #define PI 3.141592653589793
        #define EPSILON 1e-9

        #define isInf __builtin_isinf
        #define isNan __builtin_isnan

        #define MIN(a, b) ((a) < (b) ? (a) : (b)) /* @brief The minimum of two values */
        #define MAX(a, b) ((a) > (b) ? (a) : (b)) /* @brief The maximum of two values */
        #define ABS(n) ((n) > 0 ? (n) : -(n)) /* @brief The absolute value of a number */
        #define SQR(n) ((n) * (n)) /* @brief The square value of a number */

        #define SIN(x)   __builtin_sin((x))
        #define COS(x)   __builtin_cos((x))
        #define TAN(x)   __builtin_tan((x))

        #define CEIL(x)  __builtin_ceil((x))
        #define FLOOR(x) __builtin_floor((x))

        #define FOREVER for (;;)
        #define NOTHING ;
        #define PACKED __attribute__ ((__packed__))

        #define DOUBLE_MAX 1E37
        #define DOUBLE_MIN -1E37

        #define ARRAY_LEN(a) (sizeof((a)) / sizeof(*(a)))

        #define isDigit(c) ((unsigned) ((c) - '0') < 10)
        #define isLower(c) ((unsigned) ((c) - 'a') < 26)
        #define isUpper(c) ((unsigned) ((c) - 'A') < 26)
        #define isPrint(c) ((unsigned) ((c) - ' ') < 95)
        #define isAscii(c) ((unsigned) (c) < 128)

        /* Definition of common types used in the codebase */

        typedef unsigned int    size;
        typedef unsigned int    dwrd;
        typedef unsigned short  word;
        typedef unsigned char   byte;

        /* Some math (yeah, maths sucks) */

        typedef struct { uint32_t x; uint32_t y; } Point;
        typedef struct { uint32_t w; uint32_t h; } Size;
        typedef struct { float x; float y; float z; } Vector;
        typedef struct { int numerator; int denominator; } Fraction;

        /* Builtin C definitions */

        typedef __builtin_va_list va_list;

        #define va_start(ap, last_arg)  __builtin_va_start(ap, last_arg)
        #define va_arg(ap, type)        __builtin_va_arg(ap, type)
        #define va_end(ap)              __builtin_va_end(ap)
        #define va_copy(d, s)           __builtin_va_copy(d, s)

    #endif /* _COMMON_TYPEDEFS */

    /* Common system functions */

    double sqrt(int n);
    double factorial(int n);
    double pow(double base, int exp);

    double sin(double x);
    double cos(double x);
    double tan(double x);

    double ceil(double x);
    double floor(double x);

    int atoi(const char* string);

    char* itoa(int integer);
    char* ftoa(double value);
    char* htoa(int integer);

    int lengthString(const char *string);
    void reverseString(char *string);
    void combineString(char *dest, char *source);
    void copyString(char *dest, const char *source);

    bool backspace(char string[]);

    int toLower(char character);
    int toUpper(char character);

    void toLowercase(char *string);
    void toUppercase(char *string);

    void toString(int integer, char *buffer, int base);

    void appendChar(char string[], char num);

    int equalsWith(const char *a, const char *b);
    int matchWith(char *a, char *b);
    bool startsWith(const char *a, const char *b);

    /* Non standard functions */

#endif /* ! _SYS_UTILS_H */
