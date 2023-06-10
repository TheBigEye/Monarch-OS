#include "keyboard.h"

#include "../../common/ctypes.h"
#include "../../common/cstring.h"

#include "../CPU/ISR.h"
#include "../CPU/ports.h"
#include "../kernel.h"

#include "screen.h"
#include "sound.h"

#define KEY_BUFFER_SIZE 256

static char key_buffer[KEY_BUFFER_SIZE];
static bool caps_status = false;

#define SCANCODE_MAXIMUM 58

const char scancode_ascii[] = {
    '?',
    '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    ' ', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '?',
    '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
    '?', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/',
    '?', '?', '?', ' ', '?'
};


/**
 * Callback function for keyboard interrupt.
 *
 * @param regs  Register state at the time of interrupt.
 */
static void keyboardCallback(reg_t *regs) {
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = readByteFromPort(0x60);

    if (scancode >= sizeof(scancode_ascii) / sizeof(scancode_ascii[0])) {
        return;
    }

    if (scancode == BACKSPACE) {
        if (backspace(key_buffer)) {
            printBackspace();
        }
    } else if (scancode == TAB) {
        char str[2] = {' ', '\0'};
        if (strlen(key_buffer) + 1 < KEY_BUFFER_SIZE) {
            for (uint8_t i = 0; i < 5; i++) {
                append(key_buffer, ' ');
                print(str);
            }
        }

    } else if (scancode == ENTER) {
        print("\n");
        user_input(key_buffer); /* kernel-controlled function */
        key_buffer[0] = '\0';
        //playBeep(440, 100);
    } else if (scancode == CAPSLOCK) {
        /* Toggle caps lock */
        if (caps_status)
            caps_status = false;
        else
            caps_status = true;
    } else {
        char letter = scancode_ascii[(int)scancode];
        if (!caps_status && letter >= 'A' && letter <= 'Z')
            letter += 32; // to lowercase

        //playBeep((int)scancode * 100, 50);
        /* Remember that print only accepts char[] */
        char str[2] = {letter, '\0'};

        // Check if there is enough space in the buffer to append the letter
        if (strlen(key_buffer) + 1 < KEY_BUFFER_SIZE) {
            append(key_buffer, letter);
            printColor(str, BG_BLACK | FG_LTGRAY);
        }
    }
    UNUSED(regs);
}

/**
 * Initializes the keyboard by registering the keyboard callback function.
 */
void initKeyboard() {
    registerInterruptHandler(IRQ1, keyboardCallback);
}
