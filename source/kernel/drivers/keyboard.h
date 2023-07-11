#ifndef _DRIVER_KEYBOARD_H
#define _DRIVER_KEYBOARD_H 1

#include <stdint.h>
#include <stdbool.h>

#define BACKSPACE 0x0E
#define CAPSLOCK 0x3A
#define ENTER 0x1C
#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36
#define LEFT_CTRL 0x1D
#define RIGHT_CTRL 0xE01D
#define LEFT_ALT 0x38
#define RIGHT_ALT 0xE038
#define SPACEBAR 0x39
#define DEL 0x53
#define TAB 0x0F
#define ESC 0x01

#define KEY_F1 0x3B

/**
 * Initializes the keyboard by registering the keyboard callback function.
 */
void initKeyboard();

#endif /* _DRIVER_KEYBOARD_H */
