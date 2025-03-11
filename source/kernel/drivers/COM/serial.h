#ifndef _DRIVER_COM_H
#define _DRIVER_COM_H 1

#include "../../../common/common.h"

void initializeCOM(void);
void comPrintStr(const char *message);

/**
 * Print formatted output to the COM port
 *
 * @param format The format string containing format specifiers
 * @param ... Additional arguments corresponding to format specifiers
 */
void comPrintFmt(const char *format, ...);

#endif /* _DRIVER_COM_H */
