#include "display.h"

#include "../IO.h"
#include "../common/typedefs.h"
#include "../common/colors.h"

/**
 * @brief Make a new clean display
 * @param cursor_x The new cursor's x position
 * @param cursor_y The new cursor's y position
 * @param bg_color The new background color
*/
void display::initialize(uint_8 cursor_x, uint_8 cursor_y, uint_64 bg_color) {
    display::clear_display(bg_color);
    display::set_cursor_pos(coords(cursor_x, cursor_y));
    display::set_cursor_shape(CURSOR_NORMAL);
}

/**
 * @brief Clears the display with the specified background color.
 * @param clear_color Screen colors to be used to clear the display.
 * @example clear_display(BACKGROUND_BLACK | FOREGROUND_WHITE);
*/
void display::clear_display(uint_64 clear_color) {
    uint_64 value = 0;

    // TODO: optimize this using memset
    value += clear_color << 8;
    value += clear_color << 24;
    value += clear_color << 40;
    value += clear_color << 56;

    for (uint_64* i = (uint_64*) VGA_MEMORY; i < (uint_64*)(VGA_MEMORY + 4000); i++) {
        *i = value;
    }
}

/**
 * @brief Sets or changes the shape/size of the screen cursor.
 * @param shape Hexadecimal cursor shape value.
 * @note The cursor's shape varies in 4 bits values ​​from 0 to 15 (hexadecimal).
 * @see https://wiki.osdev.org/Text_Mode_Cursor
*/
void display::set_cursor_shape(uint_8 shape) {
    IO::outb(0x3D4, 0xA); // low cursor shape register
    IO::outb(0x3D5, shape); // bits 6-7 unused, bit 5 disables the cursor, bits 0-4 control the cursor shape
}

/**
 * @brief Sets the cursor position.
 * @param position Cursor position.
 * @note The cursor position is a 2-byte value, where the first byte is the X coordinate and the second byte is the Y coordinate.
 * @see https://wiki.osdev.org/Text_Mode_Cursor
*/
void display::set_cursor_pos(uint_16 position) {
    IO::outb(0x3D4, 0x0F); IO::outb(0x3D5, (uint_8)(position & 0xFF));
    IO::outb(0x3D4, 0x0E); IO::outb(0x3D5, (uint_8)((position >> 8) & 0xFF));
}

/**
 * @brief Gets the cursor position.
 * @return Cursor position.
 * @see https://wiki.osdev.org/Text_Mode_Cursor#Get_Cursor_Position
*/
uint_16 get_cursor_pos() {
    uint_16 position = 0;
    IO::outb(0x3D4, 0x0F); position |= IO::inb(0x3D5);
    IO::outb(0x3D4, 0x0E); position |= ((uint_16) IO::inb(0x3D5)) << 8;
    return position;
}

/**
 * @brief Traslates x and y coordinates to the cursor position format
 * @param x X coordinate, 0 to 127.
 * @param y Y coordinate, 0 to 127.
 * @return Cursor position.
 * @see https://wiki.osdev.org/Text_mode#Video_Memory
*/
uint_16 coords(uint_8 x, uint_8 y) {
    return y * VGA_WIDTH + x;
}

/**
 * @brief Put a character on the screen.
 * @param x X coordinate, 0 to 127.
 * @param y Y coordinate, 0 to 127.
 * @param chr Character to be displayed.
 * @param color Character color.
*/
void display::putchar(uint_8 x, uint_8 y, char chr, uint_8 color) {
    uint_16 position = coords(x, y);
    *(VGA_MEMORY + position * 2) = chr;
    *(VGA_MEMORY + position * 2 + 1) = color;
}

/**
 * @brief Gets a character from a position on the screen.
 * @param x X coordinate, 0 to 127.
 * @param y Y coordinate, 0 to 127.
 * @return Character.
*/
char display::getchar(uint_8 x, uint_8 y) {
    uint_16 position = coords(x, y);
    return (char) *(VGA_MEMORY + position * 2);
}

/**
 * @brief Put a color attribute on the screen.
 * @param x X coordinate, 0 to 127.
 * @param y Y coordinate, 0 to 127.
 * @param color Color values to be displayed in the position
*/
void display::putcolor(uint_8 x, uint_8 y, uint_8 color) {
    uint_16 position = coords(x, y);
    *(VGA_MEMORY + position * 2 + 1) = color;
}

/**
 * @brief Gets a color attribute from a position on the screen.
 * @param x X coordinate, 0 to 127.
 * @param y Y coordinate, 0 to 127.
 * @return Color values.
*/
uint_8 display::getcolor(uint_8 x, uint_8 y) {
    uint_16 position = coords(x, y);
    return *(VGA_MEMORY + position * 2 + 1);
}

/**
 * @brief Scrolls the screen up.
 * @see https://wiki.osdev.org/Text_mode#Scrolling
*/
void display::scroll_display() {
    // TODO: if the text goes out of the buffer, the screen should scroll
    // move the cursor to a new line and continue the typing

    // Move the screen up
    for (uint_8 y = 1; y < VGA_HEIGHT; y++) {
        for (uint_8 x = 0; x < VGA_WIDTH; x++) {
            putchar(x, y - 1, getchar(x, y));
            putcolor(x, y - 1, getcolor(x, y));
        }
    }
    // Clear the last line
    for (uint_8 x = 0; x < VGA_WIDTH; x++) {
        putchar(x, VGA_HEIGHT - 1, 32); // 32 is space
        putcolor(x, VGA_HEIGHT - 1, getcolor(x, VGA_HEIGHT - 1));
    }
}

void display::print_string(const char* str, uint_8 color) {
    uint_8* char_pointer = (uint_8*) str;
    uint_16 position = get_cursor_pos();

    // TODO: simplify this with IFs instead
    while (*char_pointer != 0) {
        switch (*char_pointer) {
            case 10: // \n
                position += VGA_WIDTH;
                if (position>= VGA_WIDTH * VGA_HEIGHT) {
                    scroll_display();
                    position -= VGA_WIDTH;
                }
                break;
            case 13: position -= position % VGA_WIDTH; break; // \r
            case 9: position += 8 - (position % 8); break; // \t
            case 8:
                position--;
                putchar(position % VGA_WIDTH, position / VGA_WIDTH, ' ');
                break; // \b UNUSED FOR NOW
            default:
                putchar(position % VGA_WIDTH, position / VGA_WIDTH, *char_pointer);
                putcolor(position % VGA_WIDTH, position / VGA_WIDTH, color);
                position++;

                // If the cursor is out of the screen, scroll the screen
                if (position >= VGA_WIDTH * VGA_HEIGHT) {
                    scroll_display();
                    position -= VGA_WIDTH;
                }
        }
        char_pointer++;
    }
    set_cursor_pos(position);
}

void display::print_string_centered(const char* str, uint_8 y, uint_8 color) {
    set_cursor_pos(coords((VGA_WIDTH - length(str)) / 2, y));
    print_string(str, color);
}


void display::print_char(char chr, uint_8 color) {
    uint_16 cursor_pos = get_cursor_pos();
    *(VGA_MEMORY + cursor_pos * 2) = chr;
    *(VGA_MEMORY + cursor_pos * 2 + 1) = color;

    set_cursor_pos(cursor_pos + 1);
}

void display::testColors() {
    for (uint_16 i = 0; i < 256; i++) {
        print_string("A", i);
    }
}

void display::testChars() {
    for (uint_16 i = 0; i < 256; i++) {
        print_string(int_to_string(i), FOREGROUND_YELLOW);
        print_string(" ", FOREGROUND_YELLOW);
        print_char(i, FOREGROUND_WHITE);
        print_string("  ", FOREGROUND_YELLOW);
    }
}

char hex_to_stringOutput[128];
template<typename T>
const char* hex_to_string(T value) {
    T* valPtr = &value;
    uint_8 size = (sizeof(T)) * 2 - 1;

    for (uint_8 i = 0; i < size; i++) {
        uint_8* ptr = ((uint_8*)valPtr + i);
        uint_8 temp = ((*ptr & 0xF0) >> 4);

        hex_to_stringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
        temp = ((*ptr & 0x0F));
        hex_to_stringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
    }

    hex_to_stringOutput[size + 1] = 0;
    return hex_to_stringOutput;
}

const char* hex_to_string(uint_8 value)     { return hex_to_string<uint_8>(value); }
const char* hex_to_string(uint_16 value)    { return hex_to_string<uint_16>(value); }
const char* hex_to_string(uint_32 value)    { return hex_to_string<uint_32>(value); }
const char* hex_to_string(uint_64 value)    { return hex_to_string<uint_64>(value); }
const char* hex_to_string(char value)       { return hex_to_string<char>(value); }
const char* hex_to_string(short value)      { return hex_to_string<short>(value); }
const char* hex_to_string(int value)        { return hex_to_string<int>(value); }
const char* hex_to_string(long long value)  { return hex_to_string<long long>(value); }

char int_to_stringOutput[128];
template<typename T>
const char* int_to_string(T value) {

    uint_8 isNegative = 0;

    if (value < 0) {
        isNegative = 1;
        value *= -1;
        int_to_stringOutput[0] = '-';
    }

    uint_8 size = 0;
    uint_64 sizeTester = (uint_64) value;
    while (sizeTester / 10 > 0) {
        sizeTester /= 10;
        size++;
    }

    uint_8 index = 0;
    uint_64 newValue = (uint_64) value;
    while (newValue / 10 > 0) {
        uint_8 remainder = newValue % 10;
        newValue /= 10;
        int_to_stringOutput[isNegative + size - index] = remainder + 48;
        index++;
    }

    uint_8 remainder = newValue % 10;
    int_to_stringOutput[isNegative + size - index] = remainder + 48;
    int_to_stringOutput[isNegative + size + 1] = 0;

    return int_to_stringOutput;
}

const char* int_to_string(uint_8 value)     { return int_to_string<uint_8>(value); }
const char* int_to_string(uint_16 value)    { return int_to_string<uint_16>(value); }
const char* int_to_string(uint_32 value)    { return int_to_string<uint_32>(value); }
const char* int_to_string(uint_64 value)    { return int_to_string<uint_64>(value); }
const char* int_to_string(char value)       { return int_to_string<char>(value); }
const char* int_to_string(short value)      { return int_to_string<short>(value); }
const char* int_to_string(int value)        { return int_to_string<int>(value); }
const char* int_to_string(long long value)  { return int_to_string<long long>(value); }

char float_to_stringOputput[128];
const char* float_to_string(float value, uint_8 decimalPlaces) {
    char* intPtr = (char*) int_to_string((int)value);
    char* floatPtr = float_to_stringOputput;

    if (value < 0) {
        value *= -1;
    }

    while (*intPtr != 0) {
        *floatPtr = *intPtr;
        intPtr++;
        floatPtr++;
    }
    *floatPtr = '.';
    floatPtr++;

    float newValue = value - (int)value;

    for (uint_8 i = 0; i < decimalPlaces; i++) {
        newValue *= 10;
        *floatPtr = (int)newValue + 48;
        newValue -= (int)newValue;
        floatPtr++;
    }

    *floatPtr = 0;

    return float_to_stringOputput;
}

bool compare_string(const char* str1, const char* str2) {
    while (*str1 != 0 && *str2 != 0) {
        if (*str1 != *str2) {
            return false;
        }
        str1++;
        str2++;
    }
    return *str1 == *str2;
}


int length(const char* str) {
    int length = 0;
    while (*str != 0) {
        length++;
        str++;
    }
    return length;
}
