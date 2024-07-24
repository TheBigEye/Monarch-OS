#include "serial.h"

#include "../../CPU/HAL.h"

#define SERIAL_PORT 0x3f8 // COM1

void initializeCOM(void) {
    writeByteToPort(SERIAL_PORT + 1, 0x00); // Disable all interrupts
    writeByteToPort(SERIAL_PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
    writeByteToPort(SERIAL_PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    writeByteToPort(SERIAL_PORT + 1, 0x00); //                  (hi byte)
    writeByteToPort(SERIAL_PORT + 3, 0x03); // 8 bits, no parity, one stop bit
    writeByteToPort(SERIAL_PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    writeByteToPort(SERIAL_PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set

    comPrintStr("[i] Serial port / COM has been initialized!\n");
}

void comPrintStr(const char *message) {
    while (*message != '\0') {
        while (!(readByteFromPort(SERIAL_PORT + 5) & 0x20));
        writeByteToPort(SERIAL_PORT, *message);
        message++;
    }
}
