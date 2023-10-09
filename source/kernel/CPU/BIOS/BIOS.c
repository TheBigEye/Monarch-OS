#include "BIOS.h"

#include "../ports.h"
#include "../../drivers/screen.h"

void getBIOS() {
    printColor("[o] ", BG_BLACK | FG_YELLOW);
    print("System Info from BIOS:\n");

    /* Memory Size */

    uint16_t memoryTotal;
    uint8_t memoryLower, memoryHigh;

    writeByteToPort(BIOS_ADDRESS, 0x30);
    memoryLower = readByteFromPort(BIOS_DATA);

    writeByteToPort(BIOS_ADDRESS, 0x31);
    memoryHigh = readByteFromPort(BIOS_DATA);

    memoryTotal = memoryLower | memoryHigh << 8;
    printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("RAM memory size: %s KB\n\n", itoa(memoryTotal));

    /* List disks */

    char* drivesSlots[4] = {"Primary Master", "Primary Slave", "Secondary Master", "Secondary Slave"};
    char* drivesNames[5] = {"None", "Floppy", "Hard Disk", "CD-ROM", "ZIP Drive"};

    uint8_t driveTypes[4];

    for (uint8_t i = 0; i < 4; i++) {
        writeByteToPort(BIOS_ADDRESS, 0x10 + i);
        driveTypes[i] = readByteFromPort(BIOS_DATA);
    }

    printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Storage Drives:\n");

    for (uint8_t i = 0; i < 4; i++) {
        if (driveTypes[i] != 0) {
            if (driveTypes[i] <= 4) {
                printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s: %s\n", drivesSlots[i], drivesNames[driveTypes[i]]);
            } else if (driveTypes[i] == 80) {
                printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s: First Hard Drive\n", drivesSlots[i]);
            } else {
                printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s: Unknown (%d)\n", drivesSlots[i], driveTypes[i]);
            }
        } else {
            printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s: None\n", drivesSlots[i]);
        }
    }
}
