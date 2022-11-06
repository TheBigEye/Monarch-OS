#include "power.h"

#include "../IO.h"
#include "../common/typedefs.h"

/**
 * Reboot the computer
 */
void Power::reboot(uint_32 time) {
    sleep(time);

    // Wait until the input buffer is empty
    uint_8 good = 0x02;
    while (good & 0x02) {
        good = IO::in_byte(0x64);
    }

    // Send the reboot command
    IO::out_byte(0x64, 0xFE);
    // Halt the CPU
    __asm__ ( "hlt" );
}

/**
 * Shutdown the computer
 */
void Power::shutdown(uint_32 time) {
    sleep(time);

    IO::out_word(0xB004, 0x2000); // Bochs (BIOS)
    IO::out_word(0x604, 0x2000); // QEMU (BIOS)
    IO::out_word(0x4004, 0x3400); // VirtualBox (BIOS)
}
