#ifndef _CMOS_H
#define _CMOS_H 1

#include "../../../common/sysutils.h"

#define CMOS_ADDRESS    0x70
#define CMOS_DATAREG    0x71


/**
 * Convert a Binary Coded Decimal (BCD) to a binary number.
 *
 * @param bcd   Binary Coded Decimal value
 * @return  The corresponding binary number
 */
uint8_t getBCD(uint8_t bcd);

/**
 * Convert a binary number to a Binary Coded Decimal (BCD).
 *
 * @param bin   Binary numer value
 * @return  The corresponding BCD number
 */
uint8_t getBIN(uint8_t bin);

uint8_t readFromCMOS(uint8_t address);
void writeToCMOS(uint8_t address, uint32_t value);
void dumpFromCMOS(uint16_t *values);

#endif /* _CMOS_H */
