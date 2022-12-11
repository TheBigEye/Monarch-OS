#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "../../common/monarch.h"
#include "../../common/stdlib.h"

#include "../cpu/IO.h"

#define VGA_ADDRESS (uint_8*) 0xB8000 // THE VGA (TEXT MODE) MEMORY ADDRESS
#define EGA_ADDRESS (uint_8*) 0xA0000 // THE EGA/VGA (VIDEO MODE) MEMORY ADDRESS

// Also supports 80x25 video mode, but bootloader.asm will be modified for this
#define VGA_WIDTH 80
#define VGA_HEIGHT 50

// Cursor shapes
#define CURSOR_FULL_BLOCK 0x00
#define CURSOR_HALF_BLOCK 0x07
#define CURSOR_NORMAL 0x0A
#define CURSOR_UNDERLINE 0x0F

class display {
    public:
        static void initialize(uint_8 x, uint_8 y, uint_64 color = BACKGROUND_BLACK | FOREGROUND_WHITE);
        static void cleanup(uint_64 color = BACKGROUND_BLACK | FOREGROUND_WHITE);

        static void set_cursor_pos(uint_16 position);
        static void set_cursor_shape(uint_8 shape);

        static void putchar(uint_8 x, uint_8 y, char chr, uint_8 color = BACKGROUND_BLACK | FOREGROUND_WHITE);
        static char getchar(uint_8 x, uint_8 y);

        static void putstr(uint_8 x, uint_8 y, const char *str, uint_8 color  = BACKGROUND_BLACK | FOREGROUND_WHITE);

        static void putcolor(uint_8 x, uint_8 y, uint_8 color);
        static uint_8 getcolor(uint_8 x, uint_8 y);

        static void scroll();

        static void print(const char* str, uint_8 color = BACKGROUND_BLACK | FOREGROUND_WHITE);
        static void print(char chr, uint_8 color = BACKGROUND_BLACK | FOREGROUND_WHITE);
        static void print_centered(const char* str, uint_8 y, uint_8 color = BACKGROUND_BLACK | FOREGROUND_WHITE);
        static void print_centered(char chr, uint_8 y, uint_8 color = BACKGROUND_BLACK | FOREGROUND_WHITE);

        static void colorize(uint_8 x, uint_8 y, uint_16 w, uint_16 h, char chr, uint_8 color);

        static void colors();
        static void chars();
};

uint_16 get_cursor_pos();
uint_16 coords(uint_8 x, uint_8 y);

bool compare_string(const char* str1, const char* str2);

const char* hex_to_string(uint_8 value);
const char* hex_to_string(uint_16 value);
const char* hex_to_string(uint_32 value);
const char* hex_to_string(uint_64 value);
const char* hex_to_string(char value);
const char* hex_to_string(short value);
const char* hex_to_string(int value);
const char* hex_to_string(long long value);

const char* int_to_string(uint_8 value);
const char* int_to_string(uint_16 value);
const char* int_to_string(uint_32 value);
const char* int_to_string(uint_64 value);
const char* int_to_string(char value);
const char* int_to_string(short value);
const char* int_to_string(int value);
const char* int_to_string(long long value);

const char* float_to_string(float value, uint_8 decimalPlaces = 6);

#endif /* DISPLAY_H_ */
