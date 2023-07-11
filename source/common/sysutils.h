#ifndef _SYS_UTILS_H
#define _SYS_UTILS_H 1

    // coss-compiler heaaders
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

        #define MIN(a, b) ((a) < (b) ? (a) : (b)) /* @brief The minimum of two values */
        #define MAX(a, b) ((a) > (b) ? (a) : (b)) /* @brief The maximum of two values */
        #define ABS(a) ((a) < 0 ? - (a) : (a)) /* @brief The absolute value of a number */

        #define FOREVER for(;;)
        #define NOTHING ;
        #define PACKED __attribute__((__packed__))

        #define isDigit(c)	((unsigned) ((c) - '0') < 10)
        #define isLower(c)	((unsigned) ((c) - 'a') < 26)
        #define isUpper(c)	((unsigned) ((c) - 'A') < 26)
        #define isPrint(c)	((unsigned) ((c) - ' ') < 95)
        #define isAscii(c)	((unsigned) (c) < 128)

        /* Definition of common types used in the codebase */

        typedef unsigned int    dword; // unsigned 32 bit value (0 to 4,294,967,295)
        typedef unsigned short   word; // unsigned 16 bit value (ot to 65,535)
        typedef unsigned char    byte; // unsigned 8 bit value (0 to 255)

        /* Some math (yeah, maths sucks) */

        typedef struct { int x; int y; } point;
        typedef struct { float x; float y; float z; } vector;
        typedef struct { int numerator; int denominator; } fraction;

        typedef __builtin_va_list va_list;

        #define va_start(ap, last_arg) (__builtin_va_start(ap, last_arg))
        #define va_arg(ap, type) (__builtin_va_arg(ap, type))
        #define va_end(ap) (__builtin_va_end(ap))
        #define va_copy(d, s) (__builtin_va_copy(d, s))

    #endif /* _COMMON_TYPEDEFS */

    /* Common system functions */

    void setRandom(unsigned int seed);
    int getSignedRandom();
    unsigned int getUnsignedRandom();

    char* itoa(int integer);
    char* htoa(int integer);

    int lengthString(char *string);
    void reverseString(char *string);

    bool backspace(char string[]);

    int toLower(char character);
    int toUpper(char character);

    void toLowercase(char *string);
    void toUppercase(char *string);

    void toString(int integer, char *buffer, int base);

    void appendChar(char string[], char num);

    int equalsWith(const char *a, const char *b);
    bool startsWith(char *a, char *b);

    /* Non standard functions */

    int getInputSum(char input[], int pos);
    int getInputSub(char input[], int pos);
    int getInputMul(char input[], int pos);

#endif /* _SYS_UTILS_H */
