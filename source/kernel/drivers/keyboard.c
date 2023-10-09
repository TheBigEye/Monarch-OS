#include "keyboard.h"

#include "../../common/sysutils.h"
#include "../../common/sysutils.h"

#include "../CPU/ISR/ISR.h"
#include "../CPU/ports.h"
#include "../kernel.h"

#include "screen.h"
#include "sound.h"

#define BUFFER_SIZE 256

static char key_buffer[BUFFER_SIZE];
static bool caps_status = false;

/* US Keyboard layout */
const char layout[128][2] = {
    /* {U ,  L}  | CAPS OFF          | CAPS ON           | SC   */
    {'?', '?'}, /* Unknown           | Unknown           | 0x00 */

    {'?', '?'}, /* Escape            | None              | 0x01 */
    {'1', '!'}, /* 1 num             | Exclamation       | 0x02 */
    {'2', '@'}, /* 2 num             | At Sign           | 0x03 */
    {'3', '#'}, /* 3 num             | Pound Sign        | 0x04 */
    {'4', '$'}, /* 4 num             | Dollar Sign       | 0x05 */
    {'5', '%'}, /* 5 num             | Percent Sign      | 0x06 */
    {'6', '^'}, /* 6 num             | Caret             | 0x07 */
    {'7', '&'}, /* 7 num             | Ampersand         | 0x08 */
    {'8', '*'}, /* 8 num             | Asterisk          | 0x09 */
    {'9', '('}, /* 9 num             | L Parenthesis     | 0x0A */
    {'0', ')'}, /* 0 num             | R Parenthesis     | 0x0B */
    {'-', '_'}, /* Minus             | Underscore        | 0x0C */
    {'=', '+'}, /* Equal             | Plus              | 0x0D */
    {'\b', '\b'}, /* Backspace       | Backspace         | 0x0E */

    {'\t', '\t'}, /* Tab             | Tab               | 0x0F */
    {'Q', 'q'}, /* Q upper key       | q lower key       | 0x10 */
    {'W', 'w'}, /* W upper key       | w lower key       | 0x11 */
    {'E', 'e'}, /* E upper key       | e lower key       | 0x12 */
    {'R', 'r'}, /* R upper key       | r lower key       | 0x13 */
    {'T', 't'}, /* T upper key       | t lower key       | 0x14 */
    {'Y', 'y'}, /* Y upper key       | y lower key       | 0x15 */
    {'U', 'u'}, /* U upper key       | u lower key       | 0x16 */
    {'I', 'i'}, /* I upper key       | i lower key       | 0x17 */
    {'O', 'o'}, /* O upper key       | o lower key       | 0x18 */
    {'P', 'p'}, /* P upper key       | p lower key       | 0x19 */
    {'[', '{'}, /* L Bracket         | L Brace           | 0x1A */
    {']', '}'}, /* R Bracket         | R Brace           | 0x1B */
    {'\n', '\n'}, /* Enter           | Enter             | 0x1C */

    {'?', '?'}, /* Caps Lock         | None              | 0x3A */
    {'A', 'a'}, /* A upper key       | a lower key       | 0x1E */
    {'S', 's'}, /* S upper key       | s lower key       | 0x1F */
    {'D', 'd'}, /* D upper key       | d lower key       | 0x20 */
    {'F', 'f'}, /* F upper key       | f lower key       | 0x21 */
    {'G', 'g'}, /* G upper key       | g lower key       | 0x22 */
    {'H', 'h'}, /* H upper key       | h lower key       | 0x23 */
    {'J', 'j'}, /* J upper key       | j lower key       | 0x24 */
    {'K', 'k'}, /* K upper key       | k lower key       | 0x25 */
    {'L', 'l'}, /* L upper key       | l lower key       | 0x26 */
    {';', ':'}, /* Semicolon         | Colon             | 0x27 */
    {'\'', '\"'}, /* Quote           | Double Quote      | 0x28 */
    {'`', '`'}, /* Backquote         | Tilde             | 0x29 */
    {'?', '?'}, /* L Shift           | L Shift           | 0x2A */

    {'\\', '|'}, /* Backslash        | Pipe              | 0x2B */
    {'Z', 'z'}, /* Z upper key       | z lower key       | 0x2C */
    {'X', 'x'}, /* X upper key       | x lower key       | 0x2D */
    {'C', 'c'}, /* C upper key       | c lower key       | 0x2E */
    {'V', 'v'}, /* V upper key       | v lower key       | 0x2F */
    {'B', 'b'}, /* B upper key       | b lower key       | 0x30 */
    {'N', 'n'}, /* N upper key       | n lower key       | 0x31 */
    {'M', 'm'}, /* M upper key       | m lower key       | 0x32 */
    {',', '<'}, /* Comma             | Less Than         | 0x33 */
    {'.', '>'}, /* Period            | Greater Than      | 0x34 */
    {'/', '?'}, /* Slash             | Question Mark     | 0x35 */
    {'?', '?'}, /* R Shift           | R Shift           | 0x36 */
    {'*', '*'}, /* N Asterisk        | None              | 0x37 */
    {'?', '?'}, /* Alt left          | None              | 0x38 */
    {' ', ' '}, /* Spacebar          | Spacebar          | 0x39 */
    {'?', '?'}, /* Caps Lock         | Caps Lock         | 0x3A */

    {'?', '?'}, /* F1 key            | None              | 0x3B */
    {'?', '?'}, /* F2 key            | None              | 0x3C */
    {'?', '?'}, /* F3 key            | None              | 0x3D */
    {'?', '?'}, /* F4 key            | None              | 0x3E */
    {'?', '?'}, /* F5 key            | None              | 0x3F */
    {'?', '?'}, /* F6 key            | None              | 0x40 */
    {'?', '?'}, /* F7 key            | None              | 0x41 */
    {'?', '?'}, /* F8 key            | None              | 0x42 */
    {'?', '?'}, /* F9 key            | None              | 0x43 */
    {'?', '?'}, /* F10 key           | None              | 0x44 */

    {'?', '?'}, /* Num Lock          | None              | 0x45 */
    {'?', '?'}, /* Scroll Lock       | None              | 0x46 */
    {'?', '?'}, /* Home              | None              | 0x47 */
    {'?', '?'}, /* Up Arrow          | None              | 0x48 */
    {'?', '?'}, /* Page Up           | None              | 0x49 */
    {'-', '-'}, /* Minus             | None              | 0x4A */
    {'?', '?'}, /* Left Arrow        | None              | 0x4B */
    {'?', '?'}, /* Center            | None              | 0x4C */
    {'?', '?'}, /* Right Arrow       | None              | 0x4D */
    {'+', '+'}, /* Plus              | None              | 0x4E */
    {'?', '?'}, /* End               | None              | 0x4F */
    {'?', '?'}, /* Down Arrow        | None              | 0x50 */
    {'?', '?'}, /* Page Down         | None              | 0x51 */
    {'?', '?'}, /* Insert            | None              | 0x52 */
    {'?', '?'}, /* Delete            | None              | 0x53 */

    {'?', '?'}, /* Unknown           | Unknown           | 0x54 */
    {'?', '?'}, /* Unknown           | Unknown           | 0x55 */
    {'?', '?'}, /* Unknown           | Unknown           | 0x56 */

    {'?', '?'}, /* F11               | None              | 0x57 */
    {'?', '?'}, /* F12               | None              | 0x58 */
};

/** @note If you are on a Virtual Machine, probablily this doesn't works */
void setKeyboardLeds(bool caps, bool num, bool scroll) {
    uint8_t data = 0;

    /* Caps lock LED is bit 2 */
    if (caps) {
        data |= (1 << 2);
    }

    /* Num lock LED is bit 1 */
    if (num) {
        data |= (1 << 1);
    }

    /* Scroll lock LED is bit 0 */
    if (scroll) {
        data |= (1 << 0);
    }

    /* Send the command to change the LEDs */
    writeByteToPort(0x64, 0xED);
    writeByteToPort(0x60, data);
}

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

    // If caps enabled, we use the second column, else, the fisrt column in the layout
    char letter = caps_status ? layout[(int)scancode][1] : layout[(int)scancode][0];

    /* Remember that print only accepts char[] */
    char key[4] = {letter, '\0'};

    if (scancode == BACKSPACE) {
        if (backspace(key_buffer)) {
            printColor(key, BG_BLACK | FG_LTGRAY);
        }

    /* F1 - Clear the screen, like an Commodore*/
    } else if (scancode == KEY_F1) {
        clearScreen(BG_BLACK | FG_WHITE);
        playBeep(255, 100);
        printColor("/Monarch OS/[@] ", BG_BLACK | FG_GREEN);

    } else if (scancode == ENTER) {
        printColor(key, BG_BLACK | FG_LTGRAY);
        user_input(key_buffer);
        key_buffer[0] = '\0';

    } else if (scancode == TAB) {
        if (lengthString(key_buffer) + 1 < BUFFER_SIZE) {
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

        /* Update the keyboard LEDs */
        setKeyboardLeds(caps_status, false, false);

    } else {
        // Check if there is enough space in the buffer to append the letter
        if (lengthString(key_buffer) + 1 < BUFFER_SIZE) {
            appendChar(key_buffer, letter);

            // Finally we print the char into the screen
            printColor(key, BG_BLACK | FG_LTGRAY);
        }
    }
    UNUSED(regs);
}

/**
 * Initializes the keyboard by registering the keyboard callback function.
 */
void initializeKeyboard() {
    printColor("[-] ", BG_BLACK | FG_LTGREEN); print("Initializing keyboard handler at IRQ1 ...\n");
    registerInterruptHandler(IRQ1, keyboardCallback);
}

/**
 * Terminate the keyboard by unregistering the keyboard callback function.
 */
void terminateKeyboard() {
    printColor("[-] ", BG_BLACK | FG_LTRED); print("Terminating and cleaning keyboard handler at IRQ1 ...\n");
    unregisterInterruptHandler(IRQ1);
}
