#include "serial.h"

#include "../../CPU/HAL.h"

#define SERIAL_PORT 0x3F8 // COM1


void initializeCOM(void) {
    writeByteToPort(SERIAL_PORT + 1, 0x00); // Disable all interrupts
    writeByteToPort(SERIAL_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    writeByteToPort(SERIAL_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    writeByteToPort(SERIAL_PORT + 1, 0x00); //                  (hi byte)
    writeByteToPort(SERIAL_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    writeByteToPort(SERIAL_PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    writeByteToPort(SERIAL_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set

    comPrintStr("[i] Serial port / COM has been initialized!\n");
    comPrintStr(" -  You should see kernel.map for debuging!\n\n");
}


void comPrintStr(const char *message) {
    while (*message != '\0') {
        while (!(readByteFromPort(SERIAL_PORT + 5) & 0x20));
        writeByteToPort(SERIAL_PORT, *message);
        message++;
    }
}


/* Parse format string and print to COM port */
static void parseVariadicFormat(const char *format, va_list args) {
    // Flags and parameters for formatting
    bool leftJustify, zeroPadding, precisionSpecified;
    int minWidth, precision;
    char tempBuffer[128];  // Temporary buffer for number conversions

    while (*format != '\0') {
        if (*format == '%') {
            format++;

            // Reset formatting flags
            leftJustify = zeroPadding = false;
            minWidth = precision = 0;
            precisionSpecified = false;

            // Parse flags
            while (*format == '-' || *format == '0') {
                if (*format == '-') leftJustify = true;
                if (*format == '0') zeroPadding = true;
                format++;
            }

            // Parse width
            if (*format == '*') {
                minWidth = va_arg(args, int);
                format++;
            } else while (*format >= '0' && *format <= '9') {
                minWidth = minWidth * 10 + (*format - '0');
                format++;
            }

            // Parse precision
            if (*format == '.') {
                format++;
                precisionSpecified = true;
                if (*format == '*') {
                    precision = va_arg(args, int);
                    format++;
                } else while (*format >= '0' && *format <= '9') {
                    precision = precision * 10 + (*format - '0');
                    format++;
                }
            }

            // Parse length modifier
            char length = '\0';
            if (*format == 'l' || *format == 'h') {
                length = *format++;
            }

            // Handle format specifiers with padding and justification
            switch (*format) {
                case 'd': case 'i': {
                    // Handle different integer types based on length modifier

                    if (length == 'l') {
                        long value = va_arg(args, long);
                        strint(value, tempBuffer, 10);

                    } else if (length == 'h') {
                        short value = (short) va_arg(args, int); // Note: va_arg promotes short to int
                        strint(value, tempBuffer, 10);

                    } else {
                        int value = va_arg(args, int);
                        strint(value, tempBuffer, 10);
                    }

                    int len = strlen(tempBuffer);

                    // Rest of integer formatting remains the same
                    if (precisionSpecified && precision > len) {
                        for (int i = 0; i < precision - len; i++) {
                            comPrintStr("0");
                        }
                    }

                    // Handle justification and width
                    if (leftJustify) {
                        comPrintStr(tempBuffer);
                        for (int i = len; i < minWidth; i++) {
                            comPrintStr(" ");
                        }
                    } else {
                        char pad = zeroPadding ? '0' : ' ';
                        for (int i = len; i < minWidth; i++) {
                            comPrintStr(&pad);
                        }
                        comPrintStr(tempBuffer);
                    }
                    break;
                }

                case 's': {
                    char *str = va_arg(args, char*);
                    int len = strlen(str);

                    // Truncate string if precision specified
                    if (precisionSpecified && precision < len) {
                        len = precision;
                        char temp = str[len];
                        str[len] = '\0';
                        comPrintStr(str);
                        str[len] = temp;
                    } else {
                        // Handle justification and width
                        if (leftJustify) {
                            comPrintStr(str);
                            for (int i = len; i < minWidth; i++) {
                                comPrintStr(" ");
                            }
                        } else {
                            for (int i = len; i < minWidth; i++) {
                                comPrintStr(" ");
                            }
                            comPrintStr(str);
                        }
                    }
                    break;
                }

                case 'x': case 'X': {
                    unsigned int value = va_arg(args, unsigned int);
                    strint(value, tempBuffer, 16);
                    if (*format == 'x') strlwr(tempBuffer);
                    else strupr(tempBuffer);
                    comPrintStr("0x");
                    comPrintStr(tempBuffer);
                    break;
                }
                case 'p': {
                    void *value = va_arg(args, void*);
                    strint((uintptr_t)value, tempBuffer, 16);
                    strlwr(tempBuffer);
                    comPrintStr("0x");
                    comPrintStr(tempBuffer);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    while (!(readByteFromPort(SERIAL_PORT + 5) & 0x20));
                    writeByteToPort(SERIAL_PORT, c);
                    break;
                }
                case 'f': {
                    double value = va_arg(args, double);
                    char *str = ftoa(value);
                    comPrintStr(str);
                    break;
                }
                default: {
                    while (!(readByteFromPort(SERIAL_PORT + 5) & 0x20));
                    writeByteToPort(SERIAL_PORT, '%');
                    while (!(readByteFromPort(SERIAL_PORT + 5) & 0x20));
                    writeByteToPort(SERIAL_PORT, *format);
                    break;
                }
            }
        } else {
            while (!(readByteFromPort(SERIAL_PORT + 5) & 0x20));
            writeByteToPort(SERIAL_PORT, *format);
        }
        format++;
    }
}

void comPrintFmt(const char *format, ...) {
    va_list args;
    va_start(args, format);
    parseVariadicFormat(format, args);
    va_end(args);
}
