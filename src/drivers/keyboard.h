#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "display.h"
#include "../common/typedefs.h"

/*
#define KEY_ESC 0x01
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
#define KEY_MINUS 0x0C
#define KEY_EQUAL 0x0D
#define KEY_BACKSPACE 0x0E
#define KEY_TAB 0x0F
#define KEY_Q 0x10
#define KEY_W 0x11
#define KEY_E 0x12
#define KEY_R 0x13
#define KEY_T 0x14
#define KEY_Y 0x15
#define KEY_U 0x16
#define KEY_I 0x17
#define KEY_O 0x18
#define KEY_P 0x19
#define KEY_LEFTBRACE 0x1A
#define KEY_RIGHTBRACE 0x1B
#define KEY_ENTER 0x1C
#define KEY_LEFTCTRL 0x1D
#define KEY_A 0x1E
#define KEY_S 0x1F
#define KEY_D 0x20
#define KEY_F 0x21*/

extern bool LeftShiftPressed;
extern bool RightShiftPressed;
extern uint_8 LastScancode;

bool is_key_pressed();

void StandardKeyboardHandler(uint_8 scanCode, uint_8 chr);
void KeyboardHandlerEx(uint_8 scanCode);
void KeyboardHandler(uint_8 scanCode, uint_8 chr);


#endif /* KEYBOARD_H_ */
