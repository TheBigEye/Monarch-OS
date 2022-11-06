#include "display.h"

#include "../IO.h"
#include "../common/typedefs.h"
#include "../common/colors.h"

uint_16 cursor_pos;

void display::initialize(uint_8 cursor_x, uint_8 cursor_y, uint_64 bg_color) {
    clear_display(bg_color);
    set_cursor_pos(coords(cursor_x, cursor_y));
    set_cursor_shape(CURSOR_NORMAL);

}

void display::set_cursor_shape(uint_8 shape) {
    IO::outb(0x3D4, 0xA); // low cursor shape register
    IO::outb(0x3D5, shape); // bits 6-7 unused, bit 5 disables the cursor, bits 0-4 control the cursor shape
}

void display::clear_display(uint_64 clear_color) {
    uint_64 value = 0;
    value += clear_color << 8;
    value += clear_color << 24;
    value += clear_color << 40;
    value += clear_color << 56;
    for (uint_64* i = (uint_64*) VGA_MEMORY; i < (uint_64*)(VGA_MEMORY + 4000); i++) {
        *i = value;
    }
}

void display::set_cursor_pos(uint_16 position) {
    IO::outb(0x3D4, 0x0F);
    IO::outb(0x3D5, (uint_8)(position & 0xFF));
    IO::outb(0x3D4, 0x0E);
    IO::outb(0x3D5, (uint_8)((position >> 8) & 0xFF));

    cursor_pos = position;
}


uint_16 get_cursor_pos() {
    return cursor_pos;
}

uint_16 coords(uint_8 x, uint_8 y) {
    return y * VGA_WIDTH + x;
}

void display::scroll_display() {
    for (uint_16 i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
        *(VGA_MEMORY + i * 2) = *(VGA_MEMORY + (i + VGA_WIDTH) * 2);
        *(VGA_MEMORY + i * 2 + 1) = *(VGA_MEMORY + (i + VGA_WIDTH) * 2 + 1);
    }
    for (uint_16 i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
        *(VGA_MEMORY + i * 2) = 0;
        *(VGA_MEMORY + i * 2 + 1) = BACKGROUND_BLACK | FOREGROUND_WHITE;
    }
}

void display::print_string(const char* str, uint_8 color) {
    uint_8* char_pointer = (uint_8*) str;
    uint_16 index = cursor_pos;

    while (*char_pointer != 0) {
        switch (*char_pointer) {
            case 10: // \n
                index += VGA_WIDTH;
                if (index >= VGA_WIDTH * VGA_HEIGHT) {
                    scroll_display();
                    index -= VGA_WIDTH;
                }
                break;
            case 13: index -= index % VGA_WIDTH; break; // \r
            default:
                *(VGA_MEMORY + index * 2) = *char_pointer;
                *(VGA_MEMORY + index * 2 + 1) = color;
                index++;
        }
        char_pointer++;
    }

    set_cursor_pos(index);
}
void display::print_string_centered(const char* str, uint_8 y, uint_8 color) {
    set_cursor_pos(coords((VGA_WIDTH - length(str)) / 2, y));
    print_string(str, color);
}


void display::print_char(char chr, uint_8 color) {
    *(VGA_MEMORY + cursor_pos * 2) = chr;
    *(VGA_MEMORY + cursor_pos * 2 + 1) = color;

    set_cursor_pos(cursor_pos + 1);
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

int getVgaWidth() {
    return VGA_WIDTH;
}

int getVgaHeight() {
    return VGA_HEIGHT;
}
