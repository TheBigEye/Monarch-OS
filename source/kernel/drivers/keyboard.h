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
#define TAB 0x0F
#define ESC 0x01

#define KEY_A 0x1E
#define KEY_B 0x30
#define KEY_C 0x2E
#define KEY_D 0x20
#define KEY_E 0x12
#define KEY_F 0x21
#define KEY_G 0x22
#define KEY_H 0x23
#define KEY_I 0x17
#define KEY_J 0x24
#define KEY_K 0x25
#define KEY_L 0x26
#define KEY_M 0x32
#define KEY_N 0x31
#define KEY_O 0x18
#define KEY_P 0x19
#define KEY_Q 0x10
#define KEY_R 0x13
#define KEY_S 0x1F
#define KEY_T 0x14
#define KEY_U 0x16
#define KEY_V 0x2F
#define KEY_W 0x11
#define KEY_X 0x2D
#define KEY_Y 0x15
#define KEY_Z 0x2C

#define KEY_1 0x02
#define KEY_2 0x03
#define KEY_3 0x04
#define KEY_4 0x05
#define KEY_5 0x06
#define KEY_6 0x07
#define KEY_7 0x08
#define KEY_8 0x09
#define KEY_9 0x0A
#define KEY_0 0x0B

/**
 * Initializes the keyboard by registering the keyboard callback function.
 */
void initKeyboard();

#endif /* _DRIVER_KEYBOARD_H */
