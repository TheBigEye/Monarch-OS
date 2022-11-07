#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "../IO.h"
#include "../common/typedefs.h"
#include "../common/colors.h"

#define VGA_MEMORY (uint_8*) 0xb8000 // THE VGA MEMORY ADDRESS
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Cursor
#define CURSOR_FULL_BLOCK 0x00
#define CURSOR_HALF_BLOCK 0x07
#define CURSOR_NORMAL 0x0A
#define CURSOR_UNDERLINE 0x0F

uint_16 get_cursor_pos();
uint_16 coords(uint_8 x, uint_8 y);
bool compare_string(const char* str1, const char* str2);
int length(const char* str);
int getVgaWidth();



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

const char* float_to_string(float value, uint_8 decimalPlaces);

class display {
    public:
        static void initialize(uint_8 cursor_x, uint_8 cursor_y, uint_64 bg_color);
        static void clear_display(uint_64 clear_color = BACKGROUND_BLACK | FOREGROUND_WHITE);
        static void set_cursor_pos(uint_16 position);
        static void set_cursor_shape(uint_8 shape);
        static void scroll_display();

        static void print_string(const char* str, uint_8 color = BACKGROUND_BLACK | FOREGROUND_WHITE);
        static void print_string_centered(const char* str, uint_8 y, uint_8 color = BACKGROUND_BLACK | FOREGROUND_WHITE);
        static void print_char(char chr, uint_8 color = BACKGROUND_BLACK | FOREGROUND_WHITE);

        static void testColors();
        static void testChars();
};

#endif /* DISPLAY_H_ */
