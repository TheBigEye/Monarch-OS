#include "keyboard.h"

#include "../../common/sysutils.h"
#include "../../common/sysutils.h"

#include "../CPU/ISR/ISR.h"
#include "../CPU/ports.h"
#include "../kernel.h"

#include "screen.h"
#include "sound.h"

#define KEY_BUFFER_SIZE 256

static char key_buffer[KEY_BUFFER_SIZE];
static bool caps_status = false;

#define SCANCODE_MAXIMUM 58

/* US Keyboard layout */
const char layout[] = {
    '?' /* Escape     0x01 */,

    '?',
    '1' /* 1 num      0x02 */,
    '2' /* 2 num      0x03 */,
    '3' /* 3 num      0x04 */,
    '4' /* 4 num      0x05 */,
    '5' /* 5 num      0x06 */,
    '6' /* 6 num      0x07 */,
    '7' /* 7 num      0x08 */,
    '8' /* 8 num      0x09 */,
    '9' /* 9 num      0x0A */,
    '0' /* 0 num      0x0B */,
    '-' /* Minus      0x0C */,
    '=' /* Equal      0x0D */,
    '\b' /* Backspace  0x0E */,

    '\t'/* Tab        0x0F */,
    'Q' /* Q key      0x10 */,
    'W' /* W key      0x11 */,
    'E' /* E key      0x12 */,
    'R' /* R key      0x13 */,
    'T' /* T key      0x14 */,
    'Y' /* Y key      0x15 */,
    'U' /* U key      0x16 */,
    'I' /* I key      0x17 */,
    'O' /* O key      0x18 */,
    'P' /* P key      0x19 */,
    '[' /* L Bracket  0x1A */,
    ']' /* R Bracket  0x1B */,
    '\n' /* Enter      0x1C */,

    '?' /* Caps Lock  0x3A */,
    'A' /* A key      0x1E */,
    'S' /* S key      0x1F */,
    'D' /* D key      0x20 */,
    'F' /* F key      0x21 */,
    'G' /* G key      0x22 */,
    'H' /* H key      0x23 */,
    'J' /* J key      0x24 */,
    'K' /* K key      0x25 */,
    'L' /* L key      0x26 */,
    ';' /* Semicolon  0x27 */,
    '\''/* Quote      0x28 */,
    '`' /* Backquote  0x29 */,

    '?' /* L Shift    0x2A */,
    '\\'/* Backslash  0x2B */,
    'Z' /* Z key      0x2C */,
    'X' /* X key      0x2D */,
    'C' /* C key      0x2E */,
    'V' /* V key      0x2F */,
    'B' /* B key      0x30 */,
    'N' /* N key      0x31 */,
    'M' /* M key      0x32 */,
    ',' /* Comma      0x33 */,
    '.' /* Period     0x34 */,
    '/' /* Slash      0x35 */,

    '?' /* Control    0x36 */,
    '?' /* Super      0x37 */,
    '?' /* Alt left   0x38 */,
    ' ' /* Spacebar   0x39 */,
    '?' /* Caps Lock  0x3A */,

    '?' /* F1 key     0x3B */,
    '?' /* F2 key     0x3C */,
    '?' /* F3 key     0x3D */,
    '?' /* F4 key     0x3E */,
    '?' /* F5 key     0x3F */,
    '?' /* F6 key     0x40 */,
    '?' /* F7 key     0x41 */,
    '?' /* F8 key     0x42 */,
    '?' /* F9 key     0x43 */,
    '?' /* F10 key    0x44 */
};

/**
 * Callback function for keyboard interrupt.
 *
 * @param regs  Register state at the time of interrupt.
 */
static void keyboardCallback(reg_t *regs) {
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = readByteFromPort(0x60);

    /* Check if the key was mapped in the layout */
    if (scancode >= sizeof(layout) / sizeof(layout[0])) {
        return;
    }

    // Get the scancode character
    char letter = layout[(int) scancode];

    // Convert it to lowercaase if capslock disabled
    if (!caps_status) {
        letter = toLower(letter);
    }

    /* Remember that print only accepts char[] */
    char key[2] = {letter, '\0'};


    if (scancode == BACKSPACE) {
        if (backspace(key_buffer)) {
            printColor(key, BG_BLACK | FG_LTGRAY);
        }

    } else if (scancode == KEY_F1) {
        clearScreen(BG_BLACK | FG_WHITE);
        playBeep(255, 100);
        printColor("/Monarch OS/[@] ", BG_BLACK | FG_GREEN);

    } else if (scancode == ENTER) {
        printColor(key, BG_BLACK | FG_LTGRAY);

        user_input(key_buffer);
        key_buffer[0] = '\0';

    } else if (scancode == TAB) {
        if (lengthString(key_buffer) + 1 < KEY_BUFFER_SIZE) {
            for (uint8_t i = 0; i < 4; i++) {
                appendChar(key_buffer, ' ');
            }

            printColor(key, BG_BLACK | FG_LTGRAY);
        }

    /* Toggle caps lock */
    } else if (scancode == CAPSLOCK) {
        if (caps_status) {
            caps_status = false;
            playBeep(440, 100);
        } else {
            caps_status = true;
            playBeep(220, 100);
        }

    } else {
        // Check if there is enough space in the buffer to append the letter
        if (lengthString(key_buffer) + 1 < KEY_BUFFER_SIZE) {
            appendChar(key_buffer, letter);
            printColor(key, BG_BLACK | FG_LTGRAY);
        }
    }
    UNUSED(regs);
}

/**
 * Initializes the keyboard by registering the keyboard callback function.
 */
void initKeyboard() {
    printColor("[-] ", BG_BLACK | FG_YELLOW); print("Initializing keyboard handler at IRQ1 ...\n");
    registerInterruptHandler(IRQ1, keyboardCallback);
}
