#include "console.h"
#include "../VGA/video.h"

#include "../../CPU/HAL.h"
#include "../../memory/memory.h"


/* TODO: OPTIMIZE THE SIZE FOR THIS FILE! */

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
    //return offset * 2;
    return offset + offset;
}


/* Set the cursor offset in video memory */
static void setCursorOffset(int offset) {
    /* Convert the offset in bytes to an offset in character cells */
    offset /= 2;

    /* Write the low byte of the cursor offset (15) to the data port */
    writeByteToPort(CATHODERAY_INDEX, 15);
    writeByteToPort(CATHODERAY_DATA, LOWER_BYTE(offset));

    /* Write the high byte of the cursor offset (14) to the data port */
    writeByteToPort(CATHODERAY_INDEX, 14);
    writeByteToPort(CATHODERAY_DATA, UPPER_BYTE(offset));
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
 *
 * @param color The color to which the screen will be set
 * @note If the color is NULL, the screen will be cleaned with the default scheme
 */
void setScreen(uint8_t color) {
    if (!color) {
        color = (BG_BLACK | FG_WHITE);
    }

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
int ttyPutChar(char character, int col, int row, uint8_t color) {
    uint8_t *SCREEN_MEMORY = (uint8_t *) TEXTMODE_BUFFER;

    if (!color) {
        color = (BG_BLACK | FG_LTGRAY);
    }

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= TEXTMODE_WIDTH || row >= TEXTMODE_HEIGHT) {
        SCREEN_MEMORY[TEXTMODE_SIZE - 2] = 'E';
        SCREEN_MEMORY[TEXTMODE_SIZE - 1] = (BG_BLACK | FG_LTRED);
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
    if (offset >= TEXTMODE_SIZE) {
        fastFastMemoryCopy(
            SCREEN_MEMORY + getOffset(0, 0),
            SCREEN_MEMORY + getOffset(0, 1),
            TEXTMODE_WIDTH * (TEXTMODE_HEIGHT - 1) * 2
        );

        /* Blank last line */
        fastFastMemorySet(
            (char *)(SCREEN_MEMORY + getOffset(0, TEXTMODE_HEIGHT - 1)), 0, (TEXTMODE_WIDTH << 1)
        );

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
int ttyPutText(const char *string, int col, int row, uint8_t color) {
    int offset = (col >= 0 && row >= 0) ? getOffset(col, row) : getCursorOffset();

    while (*string != '\0') {
        offset = ttyPutChar(*string++, getOffsetCol(offset), getOffsetRow(offset), color);
    }

    setCursorOffset(offset);
    return offset;
}


/* Parse an ANSI scape sequence string and return the color */
static uint8_t parseScapeSequence(const char **string) {
    uint8_t background = BG_BLACK;
    uint8_t foreground = FG_LTGRAY;

    // Skip escape character and '['
    (*string) += 2;

    // Parse ANSI color codes
    while (**string && **string != 'm') {
        // Skip delimiters
        if (**string == ';') {
            (*string)++;
            continue;
        }

        // Parse color code
        if (**string >= '0' && **string <= '9') {
            uint8_t code = 0;

            // Accumulate the color code
            while (**string >= '0' && **string <= '9') {
                code = code * 10 + (**string - '0');
                (*string)++;
            }

            /* THIS SUCKS :( */

            switch (code) {
                // Dark colors
                case 30: foreground = FG_BLACK; break;
                case 31: foreground = FG_RED; break;
                case 32: foreground = FG_GREEN; break;
                case 33: foreground = FG_BROWN; break;
                case 34: foreground = FG_BLUE; break;
                case 35: foreground = FG_MAGENTA; break;
                case 36: foreground = FG_CYAN; break;
                case 37: foreground = FG_LTGRAY; break;

                case 40: background = BG_BLACK; break;
                case 41: background = BG_RED; break;
                case 42: background = BG_GREEN; break;
                case 43: background = BG_BKYELLOW; break;
                case 44: background = BG_BLUE; break;
                case 45: background = BG_MAGENTA; break;
                case 46: background = BG_CYAN; break;
                case 47: background = BG_LTGRAY; break;

                // Bight colors
                case 90: foreground = FG_DKGRAY; break;
                case 91: foreground = FG_LTRED; break;
                case 92: foreground = FG_LTGREEN; break;
                case 93: foreground = FG_YELLOW; break;
                case 94: foreground = FG_LTBLUE; break;
                case 95: foreground = FG_LTMAGENTA; break;
                case 96: foreground = FG_LTCYAN; break;
                case 97: foreground = FG_WHITE; break;

                case 100: background = BG_BKBLACK; break;
                case 101: background = BG_BKRED; break;
                case 102: background = BG_BKGREEN; break;
                case 103: background = BG_BKYELLOW; break;
                case 104: background = BG_BKBLUE; break;
                case 105: background = BG_BKMAGENTA; break;
                case 106: background = BG_BKCYAN; break;
                case 107: background = BG_BKWHITE; break;

                default:
                    break; // Ignore unknown codes
            }
        } else {
            (*string)++;
        }
    }

    // Skip 'm' character
    if (**string == 'm') {
        (*string)++;
    }

    return (background | foreground);
}


/**
 * Get and print all the avialible charaters from the BIOS charset
 */
void ttyCharset(void) {
    for (uint8_t i = 32; i != 255; i++) { // Only printable chars
        int offset = getCursorOffset();
        int column = getOffsetCol(offset) + 1;
        int row = getOffsetRow(offset);

        // Print the character at the current cursor position
        ttyPutChar((char) i, column, row, BG_BLACK | FG_CYAN);

        // Check if we've reached the end of the line
        if (column == TEXTMODE_WIDTH) {
            column = 0; // Reset column to the start

            // Move to the next row, wrap around if at the bottom
            row = (row == TEXTMODE_HEIGHT - 1) ? 0 : row + 1;

            // Clear the new line before printing
            for (uint16_t j = 0; j < TEXTMODE_WIDTH; j++) {
                ttyPutChar(' ', j, row, BG_BLACK | FG_CYAN);
            }
        }
    }

    ttyPrintStr("\n"); // Print a newline after finishing the loop
}


/**
 * Print a string at the current cursor position on the screen.
 *
 * @param string The string to print.
 */
void ttyPrintStr(const char *string) {
    ttyPutText(string, -1, -1, (BG_BLACK | FG_LTGRAY));
}


/**
 * Print a string at the current cursor position on the screen
 *
 * @note Support ANSI color escape sequences
 *
 * @param string The string to print.
 */
void ttyPrintLog(const char *string) {
    uint8_t *SCREEN_MEMORY = (uint8_t *) TEXTMODE_BUFFER;
    uint8_t color = (BG_BLACK | FG_LTGRAY);

    int offset = getCursorOffset();

    while (*string) {

        // Start of an ANSI escape sequence
        if (*string == '\033') {
            color = parseScapeSequence(&string);

        } else {
            if (*string == '\n') {
                int row = getOffsetRow(offset);
                offset = getOffset(0, row + 1);

            } else if (*string == '\r') {
                int row = getOffsetRow(offset);
                offset = getOffset(0, row);

            } else if (*string == '\b') {
                offset -= 2;
                if (offset < 0) offset = 0;
                SCREEN_MEMORY[offset] = ' ';
                SCREEN_MEMORY[offset + 1] = color;

            } else if (*string == '\t') {
                uint8_t spaces = 4 - (getOffsetCol(offset) % 4);
                for (uint8_t i = 0; i < spaces; i++) {
                    SCREEN_MEMORY[offset] = ' ';
                    SCREEN_MEMORY[offset + 1] = color;
                    offset += 2;
                }

            } else {
                SCREEN_MEMORY[offset] = *string;
                SCREEN_MEMORY[offset + 1] = color;
                offset += 2;
            }

            string++;
        }

        if (offset >= TEXTMODE_SIZE) {
            // Scroll screen if needed
            fastFastMemoryCopy(
                SCREEN_MEMORY + getOffset(0, 0),
                SCREEN_MEMORY + getOffset(0, 1),
                TEXTMODE_WIDTH * (TEXTMODE_HEIGHT - 1) * 2
            );

            fastFastMemorySet(
                (char *)(SCREEN_MEMORY + getOffset(0, TEXTMODE_HEIGHT - 1)), 0, (TEXTMODE_WIDTH << 1)
            );

            offset -= (TEXTMODE_WIDTH << 1);
        }
    }

    setCursorOffset(offset);
}




/**
 * @brief Moves the hardware cursor to the specified position in the text mode console
 *
 * @param col Number of columns to move right from current position (X offset)
 * @param row Number of rows to move down from current position (Y offset)
 *
 * @note Position is bound-checked against TEXTMODE_WIDTH and TEXTMODE_HEIGHT.
 * If the resulting position would be out of bounds, the function returns without
 * moving the cursor.
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
