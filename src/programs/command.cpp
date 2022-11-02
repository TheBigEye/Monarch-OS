#pragma once

#include "../common/typedefs.h"
#include "../common/colors.h"
#include "../drivers/display.h"
#include "../power/power.h"
#include "../IO.h"

char CommandBuffer[256];
uint_8 CommandBufferIndex = 0;

void cls() {
    ClearScreen();
    SetCursorPosition(0);
}

void help() {
    PrintString("Available commands:\n\r", BACKGROUND_BLACK | FOREGROUND_YELLOW);
    PrintString(" clear     - Clears the screen buffer\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    PrintString(" help      - Shows this help\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    PrintString(" shutdown  - Shuts down the machine\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    PrintString(" reboot    - Reboot the system\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    PrintString(" ramsize   - Get RAM memory amount\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
}

void checkCommand(char* command) {
    if (CompareString(command, "clear") == true) {
        PrintString("\n\r");
        cls();
    } else if (CompareString(command, "help") == true) {
        PrintString("\n\r");
        help();
    } else if (CompareString(command, "shutdown") == true) {
        PrintString("\n\r");
        PrintString("Shutting down...", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
        power_shutdown();
    } else if (CompareString(command, "reboot") == true) {
        PrintString("\n\r");
        PrintString("Restarting...", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
        power_reboot();
    } else if (CompareString(command, "ramsize") == true) {
        PrintString("\n\r");
        PrintString(IntegerToString(get_ram_size()), BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
        PrintString("\n\r");
    } else if (CompareString(command, "") == true) {
        PrintString("\n\r");
    } else {
        PrintString("\n\r");
        PrintString("Unknown command. Type help for a list of available commands\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    }
    PrintString("$", BACKGROUND_BLACK | FOREGROUND_GREEN);
}

void runCommand() {
    CommandBuffer[CommandBufferIndex] = 0;
    checkCommand(CommandBuffer);
    CommandBufferIndex = 0;
}
