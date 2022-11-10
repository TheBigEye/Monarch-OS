#include "logging.h"

#include "typedefs.h"
#include "../drivers/display.h"

void logger::log(const char* decorator, uint_8 dec_color, const char* message, uint_8 msg_color) {
    display::print_string(decorator, dec_color);
    display::print_string(message, msg_color);
    display::print_string("\n\r");
}

void logger::success(const char* message) {
    display::print_string("[OK] ", BACKGROUND_BLACK | FOREGROUND_GREEN);
    display::print_string(message);
    display::print_string("\n\r");
}

void logger::error(const char* message) {
    display::print_string("[ERROR] ", BACKGROUND_BLACK | FOREGROUND_RED);
    display::print_string(message);
    display::print_string("\n\r");
}

void logger::warning(const char* message) {
    display::print_string("[WARNING] ", BACKGROUND_BLACK | FOREGROUND_YELLOW);
    display::print_string(message);
    display::print_string("\n\r");
}

void logger::info(const char* message) {
    display::print_string("[INFO] ", BACKGROUND_BLACK | FOREGROUND_WHITE);
    display::print_string(message);
    display::print_string("\n\r");
}

void logger::process(const char* message) {
    display::print_string(message, BACKGROUND_BLACK | FOREGROUND_YELLOW);
    display::print_string(" ...", BACKGROUND_BLACK | FOREGROUND_YELLOW);
    display::print_string("\n\r");
}
