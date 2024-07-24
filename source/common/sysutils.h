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

        #define PRIVATE static
        #define PUBLIC  extern

        #define NULL ((void *) 0UL)

        #define INF __builtin_inf()
        #define NAN __builtin_nan("")

        /**
         * @brief This definition of PI has 17 decimal digits, which is sufficient
         * to accurately represent a `double` value according to the IEEE 754 standard.
         * Although the GCC M_PI constant has 53 digits, only the first 17 are correct
         * due to the 53-bit precision of a `double`.
         */
        #define PI 3.141592653589793

        /*
        * NOTE: (355 ⁄ 113) and (22 / 7) are PI aproximations :)
        */

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

        #define isDigit(c) ((unsigned int) ((c) - '0') < 10u)
        #define isLower(c) ((unsigned int) ((c) - 'a') < 26u)
        #define isUpper(c) ((unsigned int) ((c) - 'A') < 26u)
        #define isPrint(c) ((unsigned int) ((c) - ' ') < 95u)
        #define isAscii(c) ((unsigned int) (c) < 128u)

        /** Character to lowercase */
        #define toLower(c) (isUpper((c)) ? ((c) + 0x20) : (c))

        /** Character to uppercase */
        #define toUpper(c) (isLower((c)) ? ((c) - 0x20) : (c))

        /* Definition of common types used in the codebase */

        typedef unsigned long    ULONG;
        typedef unsigned int     DWORD;
        typedef unsigned short   WORD;
        typedef unsigned char    BYTE;

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

    int atoi(const char* string);

    char *itoa(int integer);
    char *ftoa(double value);
    char *htoa(int integer);

    int stringLength(const char *string);

    void stringReverse(char *string);
    void stringCombine(char *destination, char *source);

    char *stringCopy(char *destination, const char *source);
    char *stringCopyTo(char *destination, const char *source, unsigned int nchars);

    void toLowercase(char *string);
    void toUppercase(char *string);

    void toString(int integer, char *buffer, int base);

    void appendChar(char string[], char num);

    int stringCompare(const char *a, const char *b);
    int stringCompareTo(const char *a, const char *b, unsigned int n);

    int stringMatch(char *a, char *b);
    bool stringStarts(const char *a, const char *b);

    char *stringFindChar(const char *string, int character);
    char *stringFindLastChar(const char *string, int character);

    /* Non standard functions */

#endif /* ! _SYS_UTILS_H */
