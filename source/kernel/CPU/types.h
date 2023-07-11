#ifndef _CPU_TYPES_H
#define _CPU_TYPES_H 1

#include <stdint.h>

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

/* Definition of common types used in the codebase */

typedef unsigned int     dword; // unsigned 32 bit value (0 to 4,294,967,295)
typedef unsigned short   word; // unsigned 16 bit value (ot to 65,535)
typedef unsigned char    byte; // unsigned 8 bit value (0 to 255)


#endif /* _CPU_TYPES_H */
