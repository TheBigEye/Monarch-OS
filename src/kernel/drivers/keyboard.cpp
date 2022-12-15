#include "keyboard.h"
#include "display.h"

#include "../../common/monarch.h"
#include "../CPU/IO.h"

bool LeftShiftPressed = false;
bool RightShiftPressed = false;
uint_8 LastScancode;

// We use a buffer for avoid delete the printed text before the KeyboardHandler initialization
char KeyboardBuffer[256];
uint_8 KeyboardBufferIndex = 0;

void StandardKeyboardHandler(uint_8 scanCode, uint_8 chr) {
    if (chr != 0) {
        // handle printable characters
        chr = (LeftShiftPressed | RightShiftPressed) ? chr - 32 : chr;
        display::print(chr);
        KeyboardBuffer[KeyboardBufferIndex++] = chr;
    } else {
        // handle non-printable characters
        switch (scanCode) {
            case 0x8E: // Backspace
                if (KeyboardBufferIndex > 0) {
                    display::set_cursor_pos(get_cursor_pos() - 1);
                    display::print(' ');
                    display::set_cursor_pos(get_cursor_pos() - 1);
                    KeyboardBuffer[--KeyboardBufferIndex] = 0;
                }
                break;
            case 0x2A: LeftShiftPressed = true; break; // Left shift
            case 0xAA: LeftShiftPressed = false; break; // Left shift released
            case 0x36: RightShiftPressed = true; break; // Right shift
            case 0xB6: RightShiftPressed = false; break; // Right shift released
            case 0x9C: // Enter
                display::print("\n\r");
                KeyboardBuffer[KeyboardBufferIndex] = 0;
                KeyboardBufferIndex = 0;
                break;
        }
    }
    LastScancode = scanCode;
}

void KeyboardHandlerEx(uint_8 scanCode) {
    switch (LastScancode) {
        case 0x50: display::set_cursor_pos(get_cursor_pos() + VGA_HEIGHT); break;
        case 0x48: display::set_cursor_pos(get_cursor_pos() - VGA_WIDTH); break;
        default:
           break;
    }
    LastScancode = scanCode;
}

void KeyboardHandler(uint_8 scanCode, uint_8 chr) {
    switch (LastScancode) {
        case 0xE0: KeyboardHandlerEx(scanCode); break;
        default:
            StandardKeyboardHandler(scanCode, chr);
    }

}

const char *getLastKeyPressed() {
    const char *currentKey = " ";

    enum KEYS {
        ENTER = 0x9C,
        BACKSPACE = 0x8E,
        LEFT_SHIFT = 0x2A,
        RIGHT_SHIFT = 0x36,
        LEFT_ALT = 0x38,
        RIGHT_ALT = 0xB8,
        LEFT_CTRL = 0x1D,
        RIGHT_CTRL = 0x9D,
        CAPS_LOCK = 0x3A,
        F1 = 0x3B,
        F2 = 0x3C,
        F3 = 0x3D,
        F4 = 0x3E,
        F5 = 0x3F,
        F6 = 0x40,
        F7 = 0x41,
        F8 = 0x42,
        F9 = 0x43,
        F10 = 0x44,
        F11 = 0x57,
        F12 = 0x58,
        TAB = 0x0F,
        ESCAPE = 0x01,
        SPACE = 0x39,
        UP = 0x48,
        DOWN = 0x50,
        LEFT = 0x4B,
        RIGHT = 0x4D,
        INSERT = 0x52,
        DELETE = 0x53,
        A = 0x1E,
        B = 0x30,
        C = 0x2E,
        D = 0x20,
        E = 0x12,
        F = 0x21,
        G = 0x22,
        H = 0x23,
        I = 0x17,
        J = 0x24,
        K = 0x25,
        L = 0x26,
        M = 0x32,
        N = 0x31,
        O = 0x18,
        P = 0x19,
        Q = 0x10,
        R = 0x13,
        S = 0x1F,
        T = 0x14,
        U = 0x16,
        V = 0x2F,
        W = 0x11,
        X = 0x2D,
        Y = 0x15,
        Z = 0x2C
    };

    switch (LastScancode) {
        case ENTER: currentKey = "[ENTER]"; break;
        case BACKSPACE: currentKey = "[BACKSPACE]"; break;
        case LEFT_SHIFT: currentKey = "[LEFT_SHIFT] "; break;
        case RIGHT_SHIFT: currentKey = "[RIGHT_SHIFT] "; break;
        case LEFT_ALT: currentKey = "[LEFT_ALT] "; break;
        case RIGHT_ALT: currentKey = "[RIGHT_ALT] "; break;
        case LEFT_CTRL: currentKey = "[LEFT_CTRL] "; break;
        case RIGHT_CTRL: currentKey = "[RIGHT_CTRL] "; break;
        case CAPS_LOCK: currentKey = "[CAPS_LOCK] "; break;
        case F1: currentKey = "[F1] "; break;
        case F2: currentKey = "[F2] "; break;
        case F3: currentKey = "[F3] "; break;
        case F4: currentKey = "[F4] "; break;
        case F5: currentKey = "[F5] "; break;
        case F6: currentKey = "[F6] "; break;
        case F7: currentKey = "[F7] "; break;
        case F8: currentKey = "[F8] "; break;
        case F9: currentKey = "[F9] "; break;
        case F10: currentKey = "[F10] "; break;
        case F11: currentKey = "[F11] "; break;
        case F12: currentKey = "[F12] "; break;
        case TAB: currentKey = "[TAB] "; break;
        case ESCAPE: currentKey = "[ESCAPE] "; break;
        case SPACE: currentKey = "[SPACE] "; break;
        case UP: currentKey = "[UP] "; break;
        case DOWN: currentKey = "[DOWN] "; break;
        case LEFT: currentKey = "[LEFT] "; break;
        case RIGHT: currentKey = "[RIGHT] "; break;
        case INSERT: currentKey = "[INSERT] "; break;
        case DELETE: currentKey = "[DELETE] "; break;

        case A: currentKey = "[A] "; break;
        case B: currentKey = "[B] "; break;
        case C: currentKey = "[C] "; break;
        case D: currentKey = "[D] "; break;
        case E: currentKey = "[E] "; break;
        case F: currentKey = "[F] "; break;
        case G: currentKey = "[G] "; break;
        case H: currentKey = "[H] "; break;
        case I: currentKey = "[I] "; break;
        case J: currentKey = "[J] "; break;
        case K: currentKey = "[K] "; break;
        case L: currentKey = "[L] "; break;
        case M: currentKey = "[M] "; break;
        case N: currentKey = "[N] "; break;
        case O: currentKey = "[O] "; break;
        case P: currentKey = "[P] "; break;
        case Q: currentKey = "[Q] "; break;
        case R: currentKey = "[R] "; break;
        case S: currentKey = "[S] "; break;
        case T: currentKey = "[T] "; break;
        case U: currentKey = "[U] "; break;
        case V: currentKey = "[V] "; break;
        case W: currentKey = "[W] "; break;
        case X: currentKey = "[X] "; break;
        case Y: currentKey = "[Y] "; break;
        case Z: currentKey = "[Z] "; break;
    }

    return currentKey;
}
