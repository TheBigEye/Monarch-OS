#include "screen.h"

#include "../../common/sysutils.h"

#include "../CPU/PIT/timer.h"
#include "../CPU/ports.h"
#include "../memory/memory.h"

// Physical VGA Video memory
#define SCREEN_BUFFER (uint8_t*) 0xB8000

#define SCREEN_WIDTH  (uint8_t) 80
#define SCREEN_HEIGHT (uint8_t) 25

#define SCREEN_AREA (uint16_t) (80 * 25)


/* Get the offset of a character cell in video memory based on its column and row */
static inline int getOffset(int column, int row) {
    return 2 * (row * SCREEN_WIDTH + column);
}

/* Get the row index of a character cell based on its offset in video memory */
static inline int getOffsetRow(int offset) {
    return offset / (SCREEN_WIDTH << 1);
}

/* Get the column index of a character cell based on its offset in video memory */
static inline int getOffsetCol(int offset) {
    return (offset % (SCREEN_WIDTH << 1)) / 2;
}

/* Get the current cursor offset from the hardware */
static inline int getCursorOffset() {
    /* Select the high byte of the cursor offset (14) and read it from the data port */
    writeByteToPort(0x3D4, 14);
    int offset = readByteFromPort(0x3D5) << 8;

    /* Select the low byte of the cursor offset (15) and read it from the data port */
    writeByteToPort(0x3D4, 15);
    offset += readByteFromPort(0x3D5);

    /* Return the cursor offset in bytes */
    return offset * 2;
}

/* Set the cursor offset in video memory */
static void setCursorOffset(int offset) {
    /* Convert the offset in bytes to an offset in character cells */
    offset /= 2;

    /* Write the high byte of the cursor offset (14) to the data port */
    writeByteToPort(0x3D4, 14);
    writeByteToPort(0x3D5, (uint8_t)(offset >> 8));

    /* Write the low byte of the cursor offset (15) to the data port */
    writeByteToPort(0x3D4, 15);
    writeByteToPort(0x3D5, (uint8_t)(offset & 0xff));
}

static void setColorPalette(uint8_t index, uint32_t RGB) {
    // Write the color index to port 0x3c8
    writeByteToPort(0x3c8, index);

    // For each color component (R, G, B)
    for (uint16_t i = 0; i < 3; i++) {
        // Shift the color bits and take only the last 6 bits
        uint8_t colorComponent = (RGB >> (i * 6)) & 0x3f;

        // Write the color component to port 0x3c9
        writeByteToPort(0x3c9, colorComponent);
    }
}


/** Initializes the custom color palette for the screen **/
void configureScreen(void) {
    uint32_t palette[16] = {
        0x000000, // Black
        0x2a0000, // Dark Red
        0x002a00, // Dark Green
        0x2a2a00, // Brown
        0x00002a, // Dark Blue
        0x2a002a, // Magenta
        0x002a2a, // Dark Cyan
        0x2a2a2a, // Dark Gray

        0x151515, // Light Gray
        0x00003f, // Light Blue
        0x003f00, // Light Green
        0x003f3f, // Light Cyan
        0x3f0000, // Light Red
        0x3f003f, // Light Pink
        0x3f3f00, // Light Yellow
        0x3f3f3f, // Light White
    };

    for (uint8_t i = 0; i < 16; i++) {
        setColorPalette(i, palette[i]);
    }
}

/**
 * Set the shape of the cursor.
 *
 * @param shape The new shape of the cursor.
 */
void setCursor(uint8_t shape) {
    writeByteToPort(0x3D4, 10);
    writeByteToPort(0x3D5, shape);
}

/**
 * Clear the screen with the specified color.
 *
 * @param color The color to fill the screen with.
 */
void clearScreen(uint8_t color) {
    // Calculate the starting memory address of the screen, uint8_t
    uint8_t * SCREEN_MEMORY = (uint8_t *) SCREEN_BUFFER;
    // Calculate the ending memory address of the screen, uint8_t
    uint8_t * FINAL_MEMORY = SCREEN_MEMORY + SCREEN_AREA * 2;

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
 * Print a character at the specified position with the given attribute
 *
 * @note - Supports newline,tabs and backspace characters
 * @note - Scrolls the screen if necessary
 * @exception - If the position is invalid, an error character 'E' will be printed at the bottom right corner
 *
 * @param character         The character to print
 * @param column            The column index of the position
 * @param row               The row index of the position
 * @param attribute         The attribute of the character
 *
 * @return The new offset of the cursor in video memory.
 */
int putCharacter(const char character, int column, int row, uint8_t attribute) {
    uint8_t *SCREEN_MEMORY = (uint8_t *)SCREEN_BUFFER;

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
    if (offset >= (SCREEN_AREA << 1)) {
        memoryCopy(
            SCREEN_MEMORY + getOffset(0, 1),
            SCREEN_MEMORY + getOffset(0, 0),
            SCREEN_WIDTH * (SCREEN_HEIGHT - 1) * 2
        );

        /* Blank last line */
        char *last_line = (char *)(SCREEN_MEMORY + getOffset(0, SCREEN_HEIGHT - 1));
        for (uint16_t i = 0; i < (SCREEN_WIDTH << 1); i++) {
            last_line[i] = 0;
        }

        offset -= 2 * SCREEN_WIDTH;
    }

    setCursorOffset(offset);
    return offset;
}

/**
 * Print a message at the specified location on the screen
 *
 * @note - If column and row are negative, the current cursor position will be used
 *
 * @param message The message to print.
 * @param column The column index of the position.
 * @param row The row index of the position.
 * @param color The color attribute of the message.
 */
void putString(const char *message, int column, int row, uint8_t color) {
    int offset = (column >= 0 && row >= 0) ? getOffset(column, row) : getCursorOffset();
    row = getOffsetRow(offset);
    column = getOffsetCol(offset);

    while (*message != '\0') {
        offset = putCharacter(*message++, column, row, color);
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
void putColor(const char character, int column, int row, uint16_t width, uint16_t height, uint8_t color) {
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
 * Print a message at the current cursor position on the screen.
 *
 * @param message The message to print.
 */
void printString(const char *message) {
    putString(message, -1, -1, BG_BLACK | FG_LTGRAY);
}


/**
 * Print a message at the current cursor position on the screen with the specified color.
 *
 * @param message The message to print.
 * @param color The color attribute of the message.
 */
void printColor(const char *message, uint8_t color) {
    putString(message, -1, -1, color);
}

void printCharset(void) {
    for (uint16_t i = 32; i < 256; i++) { // Only printable chars
        timerSleep(1);

        int offset = getCursorOffset();
        int column = getOffsetCol(offset) + 1;
        int row = getOffsetRow(offset);

        putCharacter((char) i, column, row, BG_BLACK | FG_CYAN);
        if (column == SCREEN_WIDTH) {
            column = 0;
            row = (row == SCREEN_HEIGHT - 1) ? 0 : row + 1;
            for (uint16_t j = 0; j < SCREEN_WIDTH; j++) {
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


void printOutput(const char *prompt, uint8_t promptColor, const char *format, ...) {
    printColor(prompt, promptColor);

    va_list args;
    va_start(args, format);

    vprintFormat(format, args);

    va_end(args);
}

void moveCursor(uint16_t x, uint16_t y) {
    writeByteToPort(0x3D4, 0x0F);
    uint16_t pos = readByteFromPort(0x3D5);
    writeByteToPort(0x3D4, 0x0E);
    pos |= ((uint16_t)readByteFromPort(0x3D5)) << 8;

    uint16_t cy = (pos / SCREEN_WIDTH) + y;
    uint16_t cx = (pos % SCREEN_WIDTH) + x;

    if (cx > (SCREEN_WIDTH - 1) || cy > (SCREEN_HEIGHT - 1)) {
        return;
    }

    uint16_t newpos = cy * SCREEN_WIDTH + cx;
    writeByteToPort(0x3D4, 0x0F);
    writeByteToPort(0x3D5, (uint8_t) (newpos & 0xFF));
    writeByteToPort(0x3D4, 0x0E);
    writeByteToPort(0x3D5, (uint8_t) ((newpos >> 8) & 0xFF));
}


char getCharacter() {
    writeByteToPort(0x3D4, 0x0F);
    uint16_t pos = readByteFromPort(0x3D5);
    writeByteToPort(0x3D4, 0x0E);
    pos |= ((uint16_t)readByteFromPort(0x3D5)) << 8;
    return ((uint16_t*)SCREEN_BUFFER)[pos] & 0xFF;
}
