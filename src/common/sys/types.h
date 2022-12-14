#ifndef TYPES_H_
#define TYPES_H_


typedef struct { unsigned value : 1; } BIT;        // 1-bit unsigned, at most 1 byte, from 0 to 1
typedef struct { unsigned value : 4; } NIBBLE;     // 4-bit unsigned, at most 1/2 byte, from 0 to 15

typedef unsigned char         BYTE;                // 8 bit unsigned, 1 byte, from 0 to 255
typedef unsigned short        WORD;                // 16 bit unsigned, 2 byte, from 0 to 65,535
typedef unsigned int          DWORD;               // 32 bit unsigned, 4 byte, from 0 to 4,294,967,295
typedef unsigned long long    QWORD;               // 64 bit unsigned, 8 byte, from 0 to 18,446,744,073,709,551,615

typedef signed char           SBYTE;               // 8 bit signed, 1 byte, from -128 to 127
typedef signed short          SWORD;               // 16 bit signed, 2 byte, from -32,768 to 32,767
typedef signed int            SDWORD;              // 32 bit signed, 4 byte, from -2,147,483,648 to 2,147,483,647
typedef signed long long      SQWORD;              // 64 bit signed, 8 byte, from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807

// C Standard
typedef BIT                   uint_1;              // 1-bit unsigned (uint_1), at most 1 byte, from 0 to 1
typedef NIBBLE                uint_4;              // 4-bit unsigned (uint_4), at most 1/2 byte, from 0 to 15

typedef unsigned char         uint_8;              // 8 bit unsigned, 1 byte, from 0 to 255
typedef unsigned short        uint_16;             // 16 bit unsigned, 2 byte, from 0 to 65,535
typedef unsigned int          uint_32;             // 32 bit unsigned, 4 byte, from 0 to 4,294,967,295
typedef unsigned long long    uint_64;             // 64 bit unsigned, 8 byte, from 0 to 18,446,744,073,709,551,615

typedef signed char           int_8;               // 8 bit signed, 1 byte, from -128 to 127
typedef signed short          int_16;              // 16 bit signed, 2 byte, from -32,768 to 32,767
typedef signed int            int_32;              // 32 bit signed, 4 byte, from -2,147,483,648 to 2,147,483,647
typedef signed long long      int_64;              // 64 bit signed, 8 byte, from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807



#endif /* TYPES_H_ */
