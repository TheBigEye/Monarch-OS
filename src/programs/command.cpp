#pragma once

#include "../common/typedefs.h"
#include "../common/colors.h"
#include "../drivers/display.h"
#include "../power/power.h"
#include "../IO.h"

#define COMMAND_COUNT 6

char CommandBuffer[256];
uint_8 CommandBufferIndex = 0;

void cls() {
    display::clear_display();
    display::set_cursor_pos(0);
}

void help() {
    display::print_string("Available commands:\n\r", BACKGROUND_BLACK | FOREGROUND_YELLOW);
    display::print_string(" clear     - Clears the screen buffer\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    display::print_string(" help      - Shows this help\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    display::print_string(" shutdown  - Shuts down the machine\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    display::print_string(" reboot    - Reboot the system\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
}

void shutdown()  {
    display::print_string("Shutting down...", BACKGROUND_BLACK | FOREGROUND_YELLOW);
    Power::shutdown();
}

void reboot() {
    display::print_string("Rebooting...", BACKGROUND_BLACK | FOREGROUND_YELLOW);
    Power::reboot();
}

char* Commands[COMMAND_COUNT] = {
    "clear",
    "help",
    "shutdown",
    "reboot",
    ""
};

void (*CommandFunctions[COMMAND_COUNT])() = {
    cls,
    help,
    shutdown,
    reboot,
    0
};

void CheckCommand(char* command) {
    for (uint_8 i = 0; i < COMMAND_COUNT; i++) {
        if (compare_string(command, Commands[i]) == true) {
            CommandFunctions[i]();
            display::print_string("$", BACKGROUND_BLACK | FOREGROUND_GREEN);
            return;
        }
    }
    display::print_string("Unknown command. Type help for a list of available commands\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    display::print_string("$", BACKGROUND_BLACK | FOREGROUND_GREEN);

}

void CommandHandler() {
    CommandBuffer[CommandBufferIndex] = 0;
    CheckCommand(CommandBuffer);
    CommandBufferIndex = 0;
}
