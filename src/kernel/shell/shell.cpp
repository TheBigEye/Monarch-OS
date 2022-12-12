#include "shell.h"

#include "../../common/monarch.h"
#include "../drivers/display.h"

// Include the splash.txt
extern const char Splash[];

void shell::initialize() {
    logger::process("Initializing shell"); sleep(1000);
    display::initialize(0, 0, BACKGROUND_BLACK | FOREGROUND_WHITE);

    display::set_cursor_pos(coords(0, 1));  display::print(Splash,                                            BACKGROUND_BLACK | FOREGROUND_YELLOW);

    display::colorize(0, 0, 65, 41, '*',                                                                      BACKGROUND_BLACK | FOREGROUND_BROWN);
    display::colorize(0, 0, 65, 41, '#',                                                                      BACKGROUND_BLACK | FOREGROUND_BROWN);
    display::colorize(0, 0, 65, 41, '%',                                                                      BACKGROUND_BLACK | FOREGROUND_BROWN);
    display::colorize(0, 0, 65, 41, '+',                                                                      BACKGROUND_BLACK | FOREGROUND_DARKGRAY);

    display::print_centered("Monarch OS - Under building", 34,                                                BACKGROUND_BLACK | FOREGROUND_YELLOW);
    display::print_centered("  (C)2021-2022, TheBigEye  ", 35,                                                BACKGROUND_BLACK | FOREGROUND_YELLOW);
    display::set_cursor_pos(coords(0, 38));  display::print("TIP:",                                           BACKGROUND_BLACK | FOREGROUND_YELLOW);
    display::set_cursor_pos(coords(5, 38));  display::print("Type 'help' to get an availible commands list",  BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
    display::set_cursor_pos(coords(0, 40));  display::print("[$] ",                                           BACKGROUND_BLACK | FOREGROUND_GREEN);
}
