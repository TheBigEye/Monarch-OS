#pragma once

#include "IO.h"
#include "common/Typedefs.h"

/**
 * Reboot the computer
 */
void power_reboot() {
    // Wait until the input buffer is empty
    uint_8 good = 0x02;
    while (good & 0x02) {
        good = io_in_byte(0x64);
    }

    // Send the reboot command
    io_out_byte(0x64, 0xFE);
    // Halt the CPU
    __asm__ ( "hlt" );
}

/**
 * Shutdown the computer
 */
void power_shutdown() {
    io_out_word(0xB004, 0x2000); // Bochs (BIOS)
    io_out_word(0x604, 0x2000); // QEMU (BIOS)
    io_out_word(0x4004, 0x3400); // VirtualBox (BIOS)
}
