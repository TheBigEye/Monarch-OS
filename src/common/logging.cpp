#include "logging.h"

#include "typedefs.h"
#include "../kernel/drivers/display.h"

void logger::log(const char* decorator, uint_8 dec_color, const char* message, uint_8 msg_color) {
    display::print(decorator, dec_color);
    display::print(message, msg_color);
    display::print("\n\r");
}

void logger::success(const char* message) {
    display::print("[OK] ", BACKGROUND_BLACK | FOREGROUND_GREEN);
    display::print(message);
    display::print("\n\r");
}

void logger::error(const char* message) {
    display::print("[ERROR] ", BACKGROUND_BLACK | FOREGROUND_RED);
    display::print(message);
    display::print("\n\r");
}

void logger::warning(const char* message) {
    display::print("[WARNING] ", BACKGROUND_BLACK | FOREGROUND_YELLOW);
    display::print(message);
    display::print("\n\r");
}

void logger::info(const char* message) {
    display::print("[INFO] ", BACKGROUND_BLACK | FOREGROUND_WHITE);
    display::print(message);
    display::print("\n\r");
}

void logger::process(const char* message) {
    display::print(message, BACKGROUND_BLACK | FOREGROUND_YELLOW);
    display::print(" ...", BACKGROUND_BLACK | FOREGROUND_YELLOW);
    display::print("\n\r");
}
