#include "CMOS.h"
#include "../ports.h"

/**
 * Convert a Binary Coded Decimal (BCD) to a binary number.
 *
 * @param bcd   Binary Coded Decimal value
 * @return  The corresponding binary number
 */
uint8_t getBCD(uint8_t bcd) {
    /*
    * Decimal:    0     1     2     3     4     5     6     7     8     9
    * BCD:      0000  0001  0010  0011  0100  0101  0110  0111  1000  1001
    */
    return (uint8_t)(((bcd >> 4) * 10) + (bcd & 0x0F));
}

/**
 * Convert a binary number to a Binary Coded Decimal (BCD).
 *
 * @param bin   Binary numer value
 * @return  The corresponding BCD number
 */
uint8_t getBIN(uint8_t bin) {
    /*
    * BCD:      0000  0001  0010  0011  0100  0101  0110  0111  1000  1001
    * Decimal:    0     1     2     3     4     5     6     7     8     9
    */
    return ((bin / 10) << 4) + (bin % 10);
}
