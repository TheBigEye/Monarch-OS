#ifndef STDINT_H_
#define STDINT_H_

typedef unsigned char       uint8_t;            // 1 byte, from 0 to
typedef unsigned short      uint16_t;           // 2 byte, from 0 to
typedef unsigned int        uint32_t;           // 4 byte, from 0
typedef unsigned long long  uint64_t;           // 8 byte, from 0

typedef signed char         int8_t;             // 1 byte, from -128 to 127
typedef signed short        int16_t;            // 2 byte, from -32,768 to 32,767
typedef signed int          int32_t;            // 4 byte, from -2,147,483,648 to 2,147,483,647
typedef signed long long    int64_t;            // 8 byte, from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807

typedef unsigned long long  size_t;             // 8 byte, 64 bits object size

#endif /* STDINT_H_ */
