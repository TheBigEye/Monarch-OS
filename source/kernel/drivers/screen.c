#include "screen.h"

#include "../../common/sysutils.h"

#include "../CPU/ports.h"
#include "../memory/memory.h"


#define SCREEN_BUFFER (uint8_t*) 0xB8000 // VGA Video memory

#define SCREEN_HEIGHT (uint8_t) 59
#define SCREEN_WIDTH (uint8_t) 90

#define SCREEN_AREA (uint16_t) (59 * 90)

#define REGISTRY_CONTROL (uint16_t) 0x3D4
#define REGISTRY_DATA (uint16_t) 0x3D5


/* Private Kernel API functions */

/* Get the offset of a character cell in video memory based on its column and row */
int getOffset(int column, int row) {
    return 2 * (row * SCREEN_WIDTH + column);
}

/* Get the row index of a character cell based on its offset in video memory */
int getOffsetRow(int offset) {
    return offset / (2 * SCREEN_WIDTH);
}

/* Get the column index of a character cell based on its offset in video memory */
int getOffsetCol(int offset) {
    return (offset % (2 * SCREEN_WIDTH)) / 2;
}

/* Get the current cursor offset from the hardware */
int getCursorOffset() {
    /* Select the high byte of the cursor offset (14) and read it from the data port */
    writeByteToPort(REGISTRY_CONTROL, 14);
    int offset = readByteFromPort(REGISTRY_DATA) << 8;

    /* Select the low byte of the cursor offset (15) and read it from the data port */
    writeByteToPort(REGISTRY_CONTROL, 15);
    offset += readByteFromPort(REGISTRY_DATA);

    /* Return the cursor offset in bytes */
    return offset * 2;
}

/* Set the cursor offset in video memory */
void setCursorOffset(int offset) {
    /* Convert the offset in bytes to an offset in character cells */
    offset /= 2;

    /* Write the high byte of the cursor offset (14) to the data port */
    writeByteToPort(REGISTRY_CONTROL, 14);
    writeByteToPort(REGISTRY_DATA, (byte)(offset >> 8));

    /* Write the low byte of the cursor offset (15) to the data port */
    writeByteToPort(REGISTRY_CONTROL, 15);
    writeByteToPort(REGISTRY_DATA, (byte)(offset & 0xff));
}


/**
 * Print a character at the specified position with the given attribute.
 * If the position is invalid, an error character 'E' will be printed at the bottom right corner.
 * Supports newline ('\n') and backspace (0x08) characters.
 * Scrolls the screen if necessary.
 *
 * @param character The character to print.
 * @param column The column index of the position.
 * @param row The row index of the position.
 * @param attribute The attribute of the character.
 * @return The new offset of the cursor in video memory.
 */
int putCharacter(const char character, int column, int row, byte attribute) {
    byte *SCREEN_MEMORY = (byte *)SCREEN_BUFFER;

    if (!attribute) {
        attribute = BG_BLACK | FG_LTGRAY;
    }

    /* Error control: print a red 'E' if the coords aren't right */
    if (column >= SCREEN_WIDTH || row >= SCREEN_HEIGHT) {
        SCREEN_MEMORY[2 * SCREEN_AREA - 2] = 'E';
        SCREEN_MEMORY[2 * SCREEN_AREA - 1] = BG_BLACK | FG_LTRED;
        return getOffset(column, row);
    }

    int offset;
    if (column >= 0 && row >= 0) {
        offset = getOffset(column, row);
    } else {
        offset = getCursorOffset();
    }

    int spaces = 4 - (column % 4);

    switch (character) {
        case '\n': { // New line
            row = getOffsetRow(offset);
            offset = getOffset(0, row + 1);
            break;
        }

        case '\r': { // Carriage return
            row = getOffsetRow(offset);
            offset = getOffset(0, row);
            break;
        }

        case '\b': { // Backspace
            offset = getCursorOffset() - 2;
            row = getOffsetRow(offset);
            column = getOffsetCol(offset);
            SCREEN_MEMORY[offset] = ' ';
            SCREEN_MEMORY[offset + 1] = attribute;
            break;
        }

        case '\t':{ // Tab
            for (int i = 0; i < spaces; i++) {
                SCREEN_MEMORY[offset] = ' ';
                SCREEN_MEMORY[offset + 1] = attribute;
                offset += 2;
                column++;
            }
            break;
        }

        default: { // Character
            SCREEN_MEMORY[offset] = character;
            SCREEN_MEMORY[offset + 1] = attribute;
            offset += 2;
            break;
        }
    }

    /* Check if the offset is over screen size and scroll */
    if (offset >= SCREEN_AREA * 2) {
        memoryCopy(
            SCREEN_MEMORY + getOffset(0, 1),
            SCREEN_MEMORY + getOffset(0, 0),
            SCREEN_WIDTH * (SCREEN_HEIGHT - 1) * 2
        );

        /* Blank last line */
        char *last_line = (char *)(SCREEN_MEMORY + getOffset(0, SCREEN_HEIGHT - 1));
        for (uint16_t i = 0; i < SCREEN_WIDTH * 2; i++) {
            last_line[i] = 0;
        }

        offset -= 2 * SCREEN_WIDTH;
    }

    setCursorOffset(offset);
    return offset;
}


/* Public Kernel API functions */

void VGA_install() {
    uint32_t vga_registers[15] = {
        // Horizontal timing
        0x6900, // 0x69 -> 105          | 0 | Horizontal Total Register
        0x5901, // 0x59 -> 89 columns   | 1 | End Horizontal Display Register
        0x5a02, // 0x5a -> 90           | 2 | Start Horizontal Blanking Register
        0x8c03, // 0x8c -> 140          | 3 | End Horizontal Blanking Register
        0x5e04, // 0x5e -> 94           | 4 | Start Horizontal Retrace Register
        0x8a05, // 0x8a -> 138          | 5 | End Horizontal Retrace Register

        // Vertical timing
        0x0b06, // 0xb6 -> 182          | 6 | Vertical Total Register
        0x3e07, // 0x3e -> 62           | 7 | Overflow Register
        0x4709, // 0x47 -> 71           | 9 | Maximum Scan Line Register
        0xea10, // 0xea -> 234         | 10 | Start Vertical Retrace Register
        0x8c11, // 0x8c -> 140         | 11 | End Vertical Retrace Register
        0xdf12, // 0xdf -> 223         | 12 | End Vertical Display Register
        0x2d13, // 0x2d -> 45          | 13 | Offset Register
        0xe715, // 0xe7 -> 231         | 15 | Vertical Blanking Start Register
        0x0416 // 0x04 -> 4            | 16 | End Vertical Blanking Register
    };

    // set 90x60 text mode
    writeWordToPort(0x3c4, 0x100);
    writeByteToPort(0x3c2, 0xe7);
    writeWordToPort(0x3c4, 0x0300);
    writeWordToPort(0x3c4, 0x0000);
    writeWordToPort(0x3c4, 0x0101);
    writeWordToPort(0x3c4, 0x0300);
    writeWordToPort(0x3d4, 0x0e11);

    for (uint8_t i = 0; i < 15; i++) {
        writeWordToPort(0x3d4, vga_registers[i]);
    }

    readByteFromPort(0x3da);
    writeByteToPort(0x3c0, 0x33);
    writeByteToPort(0x3c0, 0x00);
}

/**
 * Set the shape of the cursor.
 *
 * @param shape The new shape of the cursor.
 */
void setCursorShape(byte shape) {
    writeByteToPort(REGISTRY_CONTROL, 10);
    writeByteToPort(REGISTRY_DATA, shape);
}

/**
 * Clear the screen with the specified color.
 *
 * @param color The color to fill the screen with.
 */
void clearScreen(byte color) {
    // Calculate the starting memory address of the screen
    byte * SCREEN_MEMORY = (byte *) SCREEN_BUFFER;
    // Calculate the ending memory address of the screen
    byte * FINAL_MEMORY = SCREEN_MEMORY + SCREEN_AREA * 2;

    // Set the character and color in memory to clear the screen
    while (SCREEN_MEMORY < FINAL_MEMORY) {
        *SCREEN_MEMORY = ' ';
        *(SCREEN_MEMORY + 1) = color;
        SCREEN_MEMORY += 2;
    }

    // Set the cursor offset to the top-left corner of the screen
    setCursorOffset(getOffset(0, 0));
}

/**
 * print a message at the specified location on the screen.
 * If col and row are negative, the current cursor position will be used.
 *
 * @param message The message to print.
 * @param column The column index of the position.
 * @param row The row index of the position.
 * @param color The color attribute of the message.
 */
void putString(const char *message, int column, int row, byte color) {
    /* Set cursor if col/row are negative */
    int offset;
    if (column >= 0 && row >= 0) {
        offset = getOffset(column, row);
    } else {
        offset = getCursorOffset();
        row = getOffsetRow(offset);
        column = getOffsetCol(offset);
    }

    /* Loop through message and print it */
    int i = 0;
    while (message[i] != 0) {
        offset = putCharacter(message[i++], column, row, color);
        /* Compute row/col for the next iteration */
        row = getOffsetRow(offset);
        column = getOffsetCol(offset);
    }
}

/**
 * Set the color attribute of characters within a specified rectangular area on the screen.
 *
 * @param character The character to match within the area.
 * @param column The starting column index of the area.
 * @param row The starting row index of the area.
 * @param width The width of the area.
 * @param height The height of the area.
 * @param color The new color attribute to set for matching characters.
 */
void putColor(const char character, int column, int row, uint16_t width, uint16_t height, byte color) {
    // Calculate the starting memory address of the screen area
    uint16_t* video_memory = (uint16_t*)(SCREEN_BUFFER + (row * SCREEN_WIDTH + column) * 2);
    // Calculate the ending memory address of the screen area
    uint16_t* final_memory = video_memory + width * height;

    // Iterate through each character in the screen area
    while (video_memory < final_memory) {
        // Check if the character matches the desired character
        if ((char)(*video_memory & 0xFF) == character) {
            // Modify the color of the character
            *video_memory = (character & 0xFF) | (color << 8);
        }
        // Move to the next character in memory
        video_memory++;
    }
}

/**
 * print a message at the current cursor position on the screen.
 *
 * @param message The message to print.
 */
void print(const char *message) {
    putString(message, -1, -1, BG_BLACK | FG_LTGRAY);
}


/**
 * print a message at the current cursor position on the screen with the specified color.
 *
 * @param message The message to print.
 * @param color The color attribute of the message.
 */
void printColor(const char *message, byte color) {
    putString(message, -1, -1, color);
}

void printSupportedChars() {
    for (uint16_t i = 32; i < 256; i++) { // Only printable chars
        int offset = getCursorOffset();
        int row = getOffsetRow(offset);
        int col = getOffsetCol(offset) + 1;

        putCharacter((char) i, col, row, BG_BLACK | FG_CYAN);
        if (col == SCREEN_WIDTH) {
            col = 0;
            if (row == SCREEN_HEIGHT - 1) {
                // Scroll if we reach the screen end
                for (uint16_t j = 0; j < SCREEN_WIDTH; j++) {
                    putCharacter(' ', j, 0, BG_BLACK | FG_CYAN);
                }
            } else {
                row++;
            }
        }
    }
    print("\n");
}


void printFormat(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;

            // Handle format specifiers
            switch (*format) {
                case 'd': case 'i': { // Integer value
                    int value = va_arg(args, int);
                    char buffer[32];
                    toString(value, buffer, 10);
                    putString(buffer, -1, -1, BG_BLACK | FG_DKGRAY);
                    break;
                }

                case 'x': { // Hex value (lowercase)
                    int value = va_arg(args, int);
                    char buffer[32];
                    toString(value, buffer, 16);
                    toLowercase(buffer);
                    putString(buffer, -1, -1, BG_BLACK | FG_DKGRAY);
                    break;
                }

                case 'X': { // Hex value (uppercase)
                    int value = va_arg(args, int);
                    char buffer[32];
                    toString(value, buffer, 16);
                    toUppercase(buffer);
                    putString(buffer, -1, -1, BG_BLACK | FG_DKGRAY);
                    break;
                }

                case 'o': { // Octal value
                    int value = va_arg(args, int);
                    char buffer[32];
                    toString(value, buffer, 8);
                    putString(buffer, -1, -1, BG_BLACK | FG_DKGRAY);
                    break;
                }

                case 's': { // String value
                    char *value = va_arg(args, char *);
                    putString(value, -1, -1, BG_BLACK | FG_DKGRAY);
                    break;
                }

                case 'c': { // Char value
                    int value = va_arg(args, int);
                    putCharacter((char)value, -1, -1, BG_BLACK | FG_DKGRAY);
                    break;
                }

                default: {
                    putCharacter('%', -1, -1, BG_BLACK | FG_LTGRAY);
                    putCharacter(*format, -1, -1, BG_BLACK | FG_LTGRAY);
                    break;
                }
            }

        } else {
            putCharacter(*format, -1, -1, BG_BLACK | FG_LTGRAY);
        }

        format++;
    }

    va_end(args);
}
