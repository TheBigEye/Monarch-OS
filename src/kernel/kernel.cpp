#include "kernel.h"

#include "../common/monarch.h"
#include "../common/stdlib.h"

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
