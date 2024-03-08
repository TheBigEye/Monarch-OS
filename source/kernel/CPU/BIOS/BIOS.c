#include "BIOS.h"

#include "../ports.h"
#include "../../drivers/screen.h"

void getBIOS(void) {
    printOutput("[o] ", BG_BLACK | FG_YELLOW, "System Info from BIOS:\n");

    /* Memory Size */
    writeByteToPort(BIOS_ADDRESS, 0x30);
    uint8_t memoryLower = readByteFromPort(BIOS_DATA);

    writeByteToPort(BIOS_ADDRESS, 0x31);
    uint8_t memoryHigh = readByteFromPort(BIOS_DATA);

    uint16_t memoryTotal = memoryLower | memoryHigh << 8;
    printOutput("[-] ", BG_BLACK | FG_YELLOW, "RAM memory size: %s KB\n\n", itoa(memoryTotal));

    /* List disks */

    const char* drivesSlots[4] = {"Primary Master", "Primary Slave", "Secondary Master", "Secondary Slave"};
    const char* drivesNames[5] = {"None", "Floppy", "Hard Disk", "CD-ROM", "ZIP Drive"};

    uint8_t driveTypes[4];

    for (uint8_t i = 0; i < 4; i++) {
        writeByteToPort(BIOS_ADDRESS, 0x10 + i);
        driveTypes[i] = readByteFromPort(BIOS_DATA);
    }

    printOutput("[o] ", BG_BLACK | FG_YELLOW, "Storage Drives:\n");

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
