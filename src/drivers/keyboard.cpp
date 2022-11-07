#include "keyboard.h"

#include "display.h"
#include "../IO.h"
#include "../common/typedefs.h"

bool LeftShiftPressed = false;
bool RightShiftPressed = false;
uint_8 LastScancode;

void StandardKeyboardHandler(uint_8 scanCode, uint_8 chr) {
    if (chr != 0) {
        switch (LeftShiftPressed | RightShiftPressed) {
            case true: display::print_char(chr - 32); break;
            case false: display::print_char(chr); break;
        }
    } else {
        switch (scanCode) {
            case 0x8E: // Backspace.
                display::set_cursor_pos(get_cursor_pos() - 1);
                display::print_char(' ');
                display::set_cursor_pos(get_cursor_pos() - 1);
                break;

            case 0x2A: LeftShiftPressed = true; break; // Left shift
            case 0xAA: LeftShiftPressed = false; break; // Left shift released
            case 0x36: RightShiftPressed = true; break; // Right shift
            case 0xB6: RightShiftPressed = false; break; // Right shift released
            case 0x9C: display::print_string("\n\r"); break; // Enter
        }
    }
}

void KeyboardHandlerEx(uint_8 scanCode) {
    switch (LastScancode) {
        case 0x50: display::set_cursor_pos(get_cursor_pos() + getVgaWidth()); break;
        case 0x48: display::set_cursor_pos(get_cursor_pos() - getVgaWidth()); break;
        default:
           break;
    }
}

void KeyboardHandler(uint_8 scanCode, uint_8 chr) {
    switch (LastScancode) {
        case 0xE0: KeyboardHandlerEx(scanCode); break;
        default:
            StandardKeyboardHandler(scanCode, chr);
    }

    LastScancode = scanCode;
}
