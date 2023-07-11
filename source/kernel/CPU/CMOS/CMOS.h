#ifndef _CMOS_H
#define _CMOS_H 1

#include <stdint.h>

#define CMOS_ADDRESS    0x70
#define CMOS_DATA       0x71

/**
 * Convert a Binary Coded Decimal (BCD) to a binary number.
 *
 * @param bcd   Binary Coded Decimal value
 * @return  The corresponding binary number
 */
uint8_t getBCD(uint8_t bcd);

uint32_t getCmosTotalMemory();

#endif /* _CMOS_H */
