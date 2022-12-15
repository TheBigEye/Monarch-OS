#ifndef _TYPES_H
#define _TYPES_H

// SIZES and OFFSETS --------------------------------------------------------------------------------------------------------------
#ifndef BIT
#define BIT unsigned : 1    // 1 bit unsigned (uint_1), at most 1 byte, from 0 to 1
#endif
#ifndef NIBBLE
#define NIBBLE unsigned : 4 // 4 bit unsigned, at most 1/2 byte, from 0 to 15
#endif

#ifndef BYTE
#define BYTE unsigned char // 8 bit unsigned, 1 byte, from 0 to 255
#endif
#ifndef WORD
#define WORD unsigned short // 16 bit unsigned, 2 byte, from 0 to 65,535
#endif
#ifndef DWORD
#define DWORD unsigned int // 32 bit unsigned, 4 byte, from 0 to 4,294,967,295
#endif
#ifndef QWORD
#define QWORD unsigned long long; // 64 bit unsigned, 8 byte, from 0 to 18,446,744,073,709,551,615
#endif

#ifndef SBYTE
#define SBYTE signed char // 8 bit signed, 1 byte, from -128 to 127
#endif
#ifndef SWORD
#define SWORD signed short  // 16 bit signed, 2 byte, from -32,768 to 32,767
#endif
#ifndef SDWORD
#define SDWORD signed int // 32 bit signed, 4 byte, from -2,147,483,648 to 2,147,483,647
#endif
#ifndef SQWORD
#define SQWORD signed long long; // 64 bit signed, 8 byte, from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
#endif

// Monarch data types ---------------------------------------------------------------------------------------------------------------------
/* Used only to return values from functions */
#ifndef BOOL
#define BOOL bool
#endif
#ifndef CHAR
#define CHAR char
#endif
#ifndef STRING
#define STRING char*
#endif
#ifndef CONST
#define CONST const
#endif
#ifndef FLOAT
#define FLOAT float
#endif
#ifndef INT
#define INT int
#endif
#ifndef SHORT
#define SHORT short
#endif
#ifndef HANDLE
#define HANDLE void*
#endif

// C standard ---------------------------------------------------------------------------------------------------------------------
typedef unsigned char         uint_8,  uint8_t,  u8;     // 8 bit unsigned, 1 byte, from 0 to 255
typedef unsigned short        uint_16, uint16_t, u16;    // 16 bit unsigned, 2 byte, from 0 to 65,535
typedef unsigned int          uint_32, uint32_t, u32;    // 32 bit unsigned, 4 byte, from 0 to 4,294,967,295
typedef unsigned long long    uint_64, uint64_t, u64;    // 64 bit unsigned, 8 byte, from 0 to 18,446,744,073,709,551,615

typedef signed char           int_8,  int8_t,  i8;       // 8 bit signed, 1 byte, from -128 to 127
typedef signed short          int_16, int16_t, i16;      // 16 bit signed, 2 byte, from -32,768 to 32,767
typedef signed int            int_32, int32_t, i32;      // 32 bit signed, 4 byte, from -2,147,483,648 to 2,147,483,647
typedef signed long long      int_64, int64_t, i64;      // 64 bit signed, 8 byte, from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807

#endif /* _TYPES_H */
