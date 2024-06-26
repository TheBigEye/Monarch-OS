#include "console.h"
#include "VGA/VGA.h"

#include "../CPU/PIT/timer.h"
#include "../CPU/HAL.h"
#include "../memory/memory.h"

/* Private functions */

/* Get the offset of a character cell in video memory based on its column and row */
static inline int getOffset(int col, int row) {
    return 2 * (row * TEXTMODE_WIDTH + col);
}

/* Get the row index of a character cell based on its offset in video memory */
static inline int getOffsetRow(int offset) {
    return offset / (TEXTMODE_WIDTH << 1);
}

/* Get the column index of a character cell based on its offset in video memory */
static inline int getOffsetCol(int offset) {
    return (offset % (TEXTMODE_WIDTH << 1)) / 2;
}

/* Get the current cursor offset from the hardware */
static inline int getCursorOffset(void) {
    /* Select the high byte of the cursor offset (14) and read it from the data port */
    writeByteToPort(CATHODERAY_INDEX, 14);
    int offset = readByteFromPort(CATHODERAY_DATA) << 8;

    /* Select the low byte of the cursor offset (15) and read it from the data port */
    writeByteToPort(CATHODERAY_INDEX, 15);
    offset += readByteFromPort(CATHODERAY_DATA);

    /* Return the cursor offset in bytes */
    return offset * 2;
}

/* Set the cursor offset in video memory */
static void setCursorOffset(int offset) {
    /* Convert the offset in bytes to an offset in character cells */
    offset /= 2;

    /* Write the high byte of the cursor offset (14) to the data port */
    writeByteToPort(CATHODERAY_INDEX, 14);
    writeByteToPort(CATHODERAY_DATA, (uint8_t)(offset >> 8));

    /* Write the low byte of the cursor offset (15) to the data port */
    writeByteToPort(CATHODERAY_INDEX, 15);
    writeByteToPort(CATHODERAY_DATA, (uint8_t)(offset & 0xff));
}

/* Public functions */

/**
 * Set the shape of the cursor.
 *
 * @param shape The new shape of the cursor.
 */
void setCursor(uint8_t shape) {
    writeByteToPort(CATHODERAY_INDEX, 0x0A);
    writeByteToPort(CATHODERAY_DATA, shape);
}


/**
 * Clear the text-mode screen content and change color scheme
 */
void setScreen(uint8_t color) {
    // Fill the entire screen memory
    fastWideMemorySet(
        (uint16_t *) TEXTMODE_BUFFER, // 0xB8000
        (color << 8) | ' ', // Combine color and space character
        TEXTMODE_SIZE
    );

    // Set the cursor offset to the top-left corner of the screen
    setCursorOffset(getOffset(0, 0));
}

/**
 * Clear the text-mode screen content
 */
void clearScreen() {
    setScreen(BG_BLACK | FG_WHITE);
}

/**
 * Print a character at the specified position with the given attribute
 *
 * @note - Supports newline, tabs and backspace characters
 * @note - Scrolls the screen if necessary
 * @exception - If the position is invalid, an error character 'E' will be printed at the bottom right corner
 *
 * @param character         The character to print
 * @param col               The column index of the position
 * @param row               The row index of the position
 * @param color             The color of the character
 *
 * @return The new offset of the cursor in video memory.
 */
int putCharacter(char character, int col, int row, uint8_t color) {
    uint8_t *SCREEN_MEMORY = (uint8_t *) TEXTMODE_BUFFER;

    if (!color) {
        color = BG_BLACK | FG_LTGRAY;
    }

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= TEXTMODE_WIDTH || row >= TEXTMODE_HEIGHT) {
        SCREEN_MEMORY[2 * TEXTMODE_SIZE - 2] = 'E';
        SCREEN_MEMORY[2 * TEXTMODE_SIZE - 1] = BG_BLACK | FG_LTRED;
        return getOffset(col, row);
    }

    int offset = (col >= 0 && row >= 0) ? getOffset(col, row) : getCursorOffset();

    // Scape secuence chars handler
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
            col = getOffsetCol(offset);
            SCREEN_MEMORY[offset] = ' ';
            SCREEN_MEMORY[offset + 1] = color;
            break;
        }

        case '\t': { // Tab
            uint8_t spaces = 4 - (col % 4);
            for (uint8_t i = 0; i < spaces; i++) {
                SCREEN_MEMORY[offset] = ' ';
                SCREEN_MEMORY[offset + 1] = color;
                offset += 2;
                col++;
            }
            break;
        }

        default: { // Character
            SCREEN_MEMORY[offset] = character;
            SCREEN_MEMORY[offset + 1] = color;
            offset += 2;
            break;
        }
    }

    /* Check if the offset is over screen size and scroll */
    if (offset >= (TEXTMODE_SIZE << 1)) {
        fastFastMemoryCopy(
            SCREEN_MEMORY + getOffset(0, 0),
            SCREEN_MEMORY + getOffset(0, 1),
            TEXTMODE_WIDTH * (TEXTMODE_HEIGHT - 1) * 2
        );

        /* Blank last line */
        fastFastMemorySet((char *)(SCREEN_MEMORY + getOffset(0, TEXTMODE_HEIGHT - 1)), 0, (TEXTMODE_WIDTH << 1));

        offset -= (TEXTMODE_WIDTH << 1);
    }

    setCursorOffset(offset);
    return offset;
}

/**
 * Print a string at the specified location on the screen
 *
 * @note - If column and row are negative, the current cursor position will be used
 *
 * @param string  The string to print.
 * @param col     The column index of the position.
 * @param row     The row index of the position.
 * @param color   The color attribute of the message.
 */
int putString(const char *string, int col, int row, uint8_t color) {
    int offset = (col >= 0 && row >= 0) ? getOffset(col, row) : getCursorOffset();
    row = getOffsetRow(offset);
    col = getOffsetCol(offset);

    while (*string != '\0') {
        offset = putCharacter(*string++, col, row, color);
        row = getOffsetRow(offset);
        col = getOffsetCol(offset);
    }

    setCursorOffset(offset);
    return offset;
}


/**
 * Print a string at the current cursor position on the screen.
 *
 * @param string The string to print.
 */
void printString(const char *string) {
    putString(string, -1, -1, BG_BLACK | FG_LTGRAY);
}


/**
 * Print a string at the current cursor position on the screen with the specified color.
 *
 * @param string The string to print.
 * @param color  The color attribute of the message.
 */
void printColor(const char *string, uint8_t color) {
    putString(string, -1, -1, color);
}

/**
 * Get and print all the avialible charaters from the BIOS charset
 */
void printCharset() {
    for (uint16_t i = 32; i < 256; i++) { // Only printable chars
        timerSleep(1);

        int offset = getCursorOffset();
        int column = getOffsetCol(offset) + 1;
        int row = getOffsetRow(offset);

        putCharacter((char) i, column, row, BG_BLACK | FG_CYAN);
        if (column == TEXTMODE_WIDTH) {
            column = 0;
            row = (row == TEXTMODE_HEIGHT - 1) ? 0 : row + 1;
            for (uint16_t j = 0; j < TEXTMODE_WIDTH; j++) {
                putCharacter(' ', j, 0, BG_BLACK | FG_CYAN);
            }
        }
    }
    printString("\n");
}

void vprintFormat(const char *format, va_list args) {
    while (*format != '\0') {
        if (*format == '%') {
            format++;

            // Handle format specifiers
            switch (*format) {
                case 'd': case 'i': {
                    int value = va_arg(args, int);
                    char buffer[32];
                    toString(value, buffer, 10);
                    putString(buffer, -1, -1, BG_BLACK | FG_DKGRAY);
                    break;
                }

                case 'f': { // Float value
                    float value = va_arg(args, double); // floats are promoted to double when passed through ...
                    char *buffer = ftoa(value); // Use the ftoa function to convert float to string
                    putString(buffer, -1, -1, BG_BLACK | FG_DKGRAY);
                    break;
                }

                case 'x': { // Hex value (lowercase)
                    int value = va_arg(args, int);
                    char buffer[32];
                    toString(value, buffer, 16);
                    toLowercase(buffer);
                    putString("0x", -1, -1, BG_BLACK | FG_DKGRAY); // Print the "0x" prefix
                    putString(buffer, -1, -1, BG_BLACK | FG_DKGRAY);
                    break;
                }

                case 'X': { // Hex value (uppercase)
                    int value = va_arg(args, int);
                    char buffer[32];
                    toString(value, buffer, 16);
                    toUppercase(buffer);
                    putString("0x", -1, -1, BG_BLACK | FG_DKGRAY); // Print the "0x" prefix
                    putString(buffer, -1, -1, BG_BLACK | FG_DKGRAY);
                    break;
                }

                case 'p': { // Pointer value (lowercase)
                    void *value = va_arg(args, void *);
                    uintptr_t addr = (uintptr_t)value;
                    char buffer[32];
                    toString(addr, buffer, 16); // Convert the address to a hexadecimal string
                    toLowercase(buffer);
                    putString("0x", -1, -1, BG_BLACK | FG_DKGRAY); // Print the "0x" prefix
                    putString(buffer, -1, -1, BG_BLACK | FG_DKGRAY);
                    break;
                }

                case 'o': { // Octal value
                    int value = va_arg(args, int);
                    char buffer[16];
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
}

/**
 * Prints formatted output to the screen (printf)
 *
 * @note - '%d' or '%i' for integers
 * @note - '%f' for floats
 * @note - '%x' for hexadecimal lowercase, and '%X' for hexadecimal uppercase
 * @note - '%o' for octal, '%s' for string, and '%c' for character
 * @exception - If an unsupported specifier is encountered, it prints the '%' character followed by the unsupported specifier
 */
void printFormat(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vprintFormat(format, args);

    va_end(args);
}

/**
 * Print a pretty formatted output to the console (printf)
 */
void printOutput(const char *prompt, uint8_t promptColor, const char *format, ...) {
    printColor(prompt, promptColor);

    va_list args;
    va_start(args, format);

    vprintFormat(format, args);

    va_end(args);
}

/*
 * Updates the cursor position on screen
 */
void moveCursor(uint16_t col, uint16_t row) {
    writeByteToPort(0x3D4, 0x0F);
    uint16_t pos = readByteFromPort(0x3D5);
    writeByteToPort(0x3D4, 0x0E);
    pos |= ((uint16_t) readByteFromPort(0x3D5)) << 8;

    uint16_t cy = (pos / TEXTMODE_WIDTH) + row;
    uint16_t cx = (pos % TEXTMODE_WIDTH) + col;

    if (cx > (TEXTMODE_WIDTH - 1) || cy > (TEXTMODE_HEIGHT - 1)) {
        return;
    }

    uint16_t newpos = cy * TEXTMODE_WIDTH + cx;
    writeByteToPort(0x3D4, 0x0F);
    writeByteToPort(0x3D5, (uint8_t) (newpos & 0xFF));
    writeByteToPort(0x3D4, 0x0E);
    writeByteToPort(0x3D5, (uint8_t) ((newpos >> 8) & 0xFF));
}

/*
 * Get the last character from the video buffer
 */
char getCharacter() {
    writeByteToPort(0x3D4, 0x0F);
    uint16_t pos = readByteFromPort(0x3D5);
    writeByteToPort(0x3D4, 0x0E);
    pos |= ((uint16_t) readByteFromPort(0x3D5)) << 8;
    return ((uint16_t *) TEXTMODE_BUFFER)[pos] & 0xFF;
}
