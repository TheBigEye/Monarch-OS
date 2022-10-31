#include "Keyboard.h"

#include "Screen.h"
#include "../common/Typedefs.h"
#include "../programs/Command.cpp"

bool LeftShiftPressed = false;
bool RightShiftPressed = false;
uint_8 LastScancode;

void StandardKeyboardHandler(uint_8 scanCode, uint_8 chr) {
    if (chr != 0) {
        switch (LeftShiftPressed | RightShiftPressed) {
            case true:
                PrintChar(chr - 32);
                CommandBuffer[CommandBufferIndex++] = chr - 32;
                break;
            case false:
                PrintChar(chr);
                CommandBuffer[CommandBufferIndex++] = chr;
                break;
        }
    } else {
        switch (scanCode) {
            case 0x8E: // Backspace.
                if (CommandBufferIndex > 0) {
                    SetCursorPosition(getCursorPosition() - 1);
                    PrintChar(' ');
                    SetCursorPosition(getCursorPosition() - 1);
                    CommandBuffer[--CommandBufferIndex] = 0;
                }
                break;

            case 0x2A: LeftShiftPressed = true; break; // Left shift
            case 0xAA: LeftShiftPressed = false; break; // Left shift released
            case 0x36: RightShiftPressed = true; break; // Right shift
            case 0xB6: RightShiftPressed = false; break; // Right shift released
            case 0x9C: runCommand(); break; // Enter executes a command
        }
    }
}

void KeyboardHandlerEx(uint_8 scanCode) {
    switch (LastScancode) {
        case 0x50: SetCursorPosition(getCursorPosition() + getVgaWidth()); break;
        case 0x48: SetCursorPosition(getCursorPosition() - getVgaWidth()); break;
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
