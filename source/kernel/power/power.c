#include "power.h"

#include "../CPU/ports.h"
#include "../CPU/ISR/ISR.h"

#include "../kernel.h"

/**
 * Reboots the computer after the specified time.
 *
 * @param time The time to wait in milliseconds before rebooting.
 */
void powerReboot(uint32_t time) {

    terminateKernel(); // Clean drivers and kernel

    operationSleep(time);

    // Wait until the input buffer is empty
    bool good = true;
    while (good & 0x02) {
        good = readByteFromPort(0x64);
    }

    writeByteToPort(0x64, 0xFE); // Send the reboot command
    __asm__ __volatile__("hlt"); // Halt the CPU
}

/**
 * Shuts down the computer after the specified time.
 *
 * @param time The time to wait in milliseconds before shutting down.
 */
void powerShutdown(uint32_t time) {

    terminateKernel(); // Clean drivers and kernel

    operationSleep(time);

    writeWordToPort(0xB004, 0x2000); // Bochs (BIOS)
    writeWordToPort(0x604, 0x2000); // QEMU (BIOS)
    writeWordToPort(0x4004, 0x3400); // VirtualBox (BIOS)
}
