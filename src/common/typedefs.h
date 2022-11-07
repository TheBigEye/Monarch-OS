#ifndef COMMON_TYPEDEFS_H_
#define COMMON_TYPEDEFS_H_

/*
typedef struct { unsigned value : 1; } uint_1;  // 1 bit    0 to 1
typedef struct { unsigned value : 4; } uint_4;  // 1 Nibble 0 to 15
*/

typedef unsigned char       uint_8;             // 1 byte   0 to 255
typedef unsigned short      uint_16;            // 2 byte   0 to 65,535
typedef unsigned int        uint_32;            // 4 byte   0 to 4,294,967,295
typedef unsigned long long  uint_64;            // 8 byte   0 to 18,446,744,073,709,551,615

typedef char                int_8;              // 1 byte   -128 to 127
typedef short               int_16;             // 2 byte   -32,768 to 32,767
typedef int                 int_32;             // 4 byte   -2,147,483,648 to 2,147,483,647
typedef long long           int_64;             // 8 byte   -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807

typedef char* string;

#endif /* COMMON_TYPEDEFS_H_ */
