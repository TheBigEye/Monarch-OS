#include "common/logging.h"

#include "drivers/display.h"
#include "drivers/keyboard.h"

#include "IDT.h"
#include "memory/map.h"
#include "memory/heap.h"
#include "programs/command.cpp"

#include "CPU.h"

#include "power/power.h"

extern const char Splash[];

void InitializeKernel();
void InitializeShell();
void panic(uint_32 rebootTime);

extern "C" void _start() {
    InitializeKernel();
    return;
}

extern "C" void _end() {
    return;
}

void InitializeKernel() {
    display::initialize(0, 0, BACKGROUND_BLACK | FOREGROUND_WHITE);
    logger::process("Initializing kernel"); sleep(1000);

    InitializeIDT();                        logger::success("IDT initialized");               sleep(1000); // Initialize Interrupt Descriptor Table
    MainKeyboardHandler = KeyboardHandler;  logger::success("Keyboard handler initialized");  sleep(1000); // Initialize Keyboard Handler
    MainCommandsHandler = CommandsHandler;  logger::success("Commands handler initialized");  sleep(1000); // Initialize Commands handler

    logger::process("Initializing shell"); sleep(1000);
    InitializeShell(); // Initialize shell
}

void InitializeShell() {
    display::initialize(0, 0, BACKGROUND_BLACK | FOREGROUND_WHITE);

    display::set_cursor_pos(coords(0, 0));  display::print_string(Splash,                                          BACKGROUND_BLACK | FOREGROUND_BROWN);
    display::set_cursor_pos(coords(15, 0)); display::print_string("Monarch OS - Under building",                   BACKGROUND_BLACK | FOREGROUND_WHITE);
    display::set_cursor_pos(coords(15, 1)); display::print_string("(C)2021-2022, TheBigEye",                       BACKGROUND_BLACK | FOREGROUND_WHITE);
    display::set_cursor_pos(coords(0, 6));  display::print_string("TIP:",                                          BACKGROUND_BLACK | FOREGROUND_YELLOW);
    display::set_cursor_pos(coords(5, 6));  display::print_string("Type 'help' to get an availible commands list", BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    display::set_cursor_pos(coords(0, 8));  display::print_string("$",                                             BACKGROUND_BLACK | FOREGROUND_GREEN);
}

void panic(uint_32 rebootTime) {
    string title = " Monarch OS ";

    string msg_line_1 = "A fatal exception 0E has ocurred at 0028:C0011E36 in VXD VMM(01) +";
    string msg_line_2 = "00010E32. The current application will be terminated.             ";
    string msg_line_3 = "*  The computer will automatically restart to try to fix this.    ";
    string msg_line_4 = "*  If a solution to the problem is not found, contact your        ";
    string msg_line_5 = "   system administrator or technical.                             ";
    string msg_line_6 = "Rebooting ...";

    display::clear_display(BACKGROUND_BLUE| FOREGROUND_WHITE);
    display::print_string_centered(title, 4, BACKGROUND_LIGHTGRAY | FOREGROUND_BLUE);

    display::print_string_centered(msg_line_1, 6, BACKGROUND_BLUE | FOREGROUND_LIGHTGRAY);
    display::print_string_centered(msg_line_2, 7, BACKGROUND_BLUE | FOREGROUND_LIGHTGRAY);
    display::print_string_centered(msg_line_3, 9, BACKGROUND_BLUE | FOREGROUND_LIGHTGRAY);
    display::print_string_centered(msg_line_4, 10, BACKGROUND_BLUE | FOREGROUND_LIGHTGRAY);
    display::print_string_centered(msg_line_5, 11, BACKGROUND_BLUE | FOREGROUND_LIGHTGRAY);
    display::print_string_centered(msg_line_6, 13, BACKGROUND_BLUE | FOREGROUND_YELLOW);

    Power::reboot(rebootTime);
}
