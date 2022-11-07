#pragma once

#include "../common/typedefs.h"
#include "../common/colors.h"
#include "../drivers/display.h"
#include "../power/power.h"
#include "../IO.h"
#include "../CPU.h"

#define COMMAND_COUNT 6

char CommandBuffer[256];
uint_8 CommandBufferIndex = 0;

void cls() {
    display::clear_display();
    display::set_cursor_pos(0);
}

void cpufreq() {
    display::print_string("CPU Frequency: ", BACKGROUND_BLACK | FOREGROUND_YELLOW); display::print_string(int_to_string(CPU_freq()), BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY); display::print_string(" MHz\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    display::print_string("CPU Cycles per sec: ", BACKGROUND_BLACK | FOREGROUND_YELLOW); display::print_string(int_to_string(CPU_cycles()), BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY); display::print_string("\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
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

// Commands
char* Commands[COMMAND_COUNT] = {
    "clear",
    "help",
    "cpufreq",
    "shutdown",
    "reboot",
    ""
};

// Commands functions
void (*CommandFunctions[COMMAND_COUNT])() = {
    cls,
    help,
    cpufreq,
    shutdown,
    reboot,
    0
};

void CheckCommand(char* command) {
    for (uint_8 i = 0; i < COMMAND_COUNT; i++) {
        if (compare_string(command, Commands[i]) == true) { // If the command is found
            CommandFunctions[i](); // Run the asociated function
            display::print_string("$", BACKGROUND_BLACK | FOREGROUND_GREEN); // Print the prompt
            return;
        }
    }
    // If the command is not found
    display::print_string("Unknown command. Type help for a list of available commands\n\r", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    display::print_string("$", BACKGROUND_BLACK | FOREGROUND_GREEN);

}

void CommandsHandler(uint_8 scanCode, uint_8 chr) {
    if (chr != 0) { // If a chararcter key was pressed
        switch (LeftShiftPressed | RightShiftPressed) {
            case true: CommandBuffer[CommandBufferIndex++] = chr - 32; break; // Uppercase
            case false: CommandBuffer[CommandBufferIndex++] = chr; break; // Lowercase
        }
    } else {
        switch (scanCode) {
            case 0x8E: // Backspace
                if (CommandBufferIndex > 0) { // If there something in the buffer
                    CommandBuffer[--CommandBufferIndex] = 0; // Remove the last character
                }
                break;
            case 0x9C: // Enter
                CommandBuffer[CommandBufferIndex] = 0; // Add a null terminator
                CheckCommand(CommandBuffer); // Check and run the command
                CommandBufferIndex = 0; // Reset the buffer
                break;
        }
    }
}
