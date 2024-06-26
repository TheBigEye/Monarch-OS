#include "BIOS.h"

#include "CMOS/CMOS.h"
#include "HAL.h"

#include "../drivers/console.h"

void firmwareGetStatus() {
    printOutput("[o] ", BG_BLACK | FG_YELLOW, "System Info from BIOS:\n");

    /* Memory Size */
    // BUGBUG: if i have as RAM an 1GB or more, memoryTotal overflows to 65535
    // (https://forum.osdev.org/viewtopic.php?t=18336)

    uint16_t memory;
    uint8_t low, high;

    low = readFromCMOS(0x30);
    high = readFromCMOS(0x31);

    memory = low | high << 8;

    printOutput("[-] ", BG_BLACK | FG_YELLOW, "RAM memory size: %s KB\n\n", itoa((int) memory));

    /* TODO: List disks (this may be wrong, test this) */

    const char* drivesSlots[4] = {"Primary Master", "Primary Slave", "Secondary Master", "Secondary Slave"};
    const char* drivesNames[5] = {"None", "Floppy", "Hard Disk", "CD-ROM", "ZIP Drive"};

    uint8_t driveTypes[4];

    for (uint8_t i = 0; i < 4; i++) {
        writeByteToPort(CMOS_ADDRESS, 0x10 + i);
        driveTypes[i] = readByteFromPort(CMOS_DATAREG);
    }

    printOutput("[o] ", BG_BLACK | FG_YELLOW, "Storage drives:\n");

    for (uint8_t i = 0; i < 4; i++) {
        if (driveTypes[i] != 0) {
            if (driveTypes[i] <= 4) {
                printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s: %s\n", drivesSlots[i], drivesNames[driveTypes[i]]);
            } else if (driveTypes[i] == 80) {
                printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s: First Hard Drive\n", drivesSlots[i]);
            } else {
                printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s: Unknown (%d)\n", drivesSlots[i], driveTypes[i]);
            }
        } else {
            printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s: None\n", drivesSlots[i]);
        }
    }
}

void firmwareGetFloppy() {
    const char *driveType[16] = {
        "no floppy drive",
        "360kb 5.25in floppy drive",
        "1.2mb 5.25in floppy drive",
        "720kb 3.5in",
        "1.44mb 3.5in",
        "2.88mb 3.5in"
    };

	uint8_t a = 0, b = 0, c = 0;

	writeByteToPort(0x70, 0x10);
	c = readByteFromPort(0x71);
	a = c >> 4; // get the high nibble
	b = c & 0xF; // get the low nibble by ANDing out the high nibble

    printOutput("[o] ", BG_BLACK | FG_YELLOW, "Floppy drives:\n");
	printOutput("[-] ", BG_BLACK | FG_YELLOW, " A: %s \n", driveType[a]);
	printOutput("[-] ", BG_BLACK | FG_YELLOW, " B: %s \n", driveType[b]);
}
