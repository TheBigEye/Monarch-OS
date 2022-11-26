#include "kernel.h"

#include "../common/libc/string.h"
#include "../common/logging.h"
#include "../common/typedefs.h"
#include "cpu/IDT.h"
#include "cpu/IO.h"
#include "drivers/display.h"
#include "drivers/keyboard.h"
#include "power/power.h"
#include "shell/command.h"

void kernel::initialize() {
    display::initialize(0, 0, BACKGROUND_BLACK | FOREGROUND_WHITE);
    logger::process("Initializing kernel"); sleep(1000);

    InitializeIDT();                        logger::success("IDT initialized");               sleep(512); // Initialize Interrupt Descriptor Table
    MainKeyboardHandler = KeyboardHandler;  logger::success("Keyboard handler initialized");  sleep(512); // Initialize Keyboard Handler
    MainCommandsHandler = CommandsHandler;  logger::success("Commands handler initialized");  sleep(512); // Initialize Commands handler
}

void kernel::finalize() {
    display::cleanup();
    logger::process("Finalizing kernel"); sleep(50);
    power::shutdown(50);
}

void kernel::panic(string msg, uint_32 rebootTime) {
    string title = " Monarch OS ";

    string msg_line_1 = msg;
    string msg_line_2 = "00010E32. The current application will be terminated.             ";
    string msg_line_3 = "*  The computer will automatically restart to try to fix this.    ";
    string msg_line_4 = "*  If a solution to the problem is not found, contact your        ";
    string msg_line_5 = "   system administrator or technical.                             ";
    string msg_line_6 = "Rebooting ...";

    display::cleanup(BACKGROUND_BLUE| FOREGROUND_WHITE);
    display::print_centered(title, 4, BACKGROUND_LIGHTGRAY | FOREGROUND_BLUE);

    display::print_centered(msg_line_1, 6, BACKGROUND_BLUE | FOREGROUND_LIGHTGRAY);
    display::print_centered(msg_line_2, 7, BACKGROUND_BLUE | FOREGROUND_LIGHTGRAY);
    display::print_centered(msg_line_3, 9, BACKGROUND_BLUE | FOREGROUND_LIGHTGRAY);
    display::print_centered(msg_line_4, 10, BACKGROUND_BLUE | FOREGROUND_LIGHTGRAY);
    display::print_centered(msg_line_5, 11, BACKGROUND_BLUE | FOREGROUND_LIGHTGRAY);
    display::print_centered(msg_line_6, 13, BACKGROUND_BLUE | FOREGROUND_YELLOW);

    power::reboot(rebootTime);
}
