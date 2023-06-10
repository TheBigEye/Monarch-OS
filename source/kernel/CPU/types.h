#ifndef _CPU_TYPES_H
#define _CPU_TYPES_H 1

#include <stdint.h>

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#endif /* _CPU_TYPES_H */
