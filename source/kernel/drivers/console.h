#ifndef DRIVER_CONSOLE_H_
#define DRIVER_CONSOLE_H_ 1

#include "../../common/common.h"


/* Foreground colors */
#define FG_BLACK            0x00 // Black foreground attribute
#define FG_BLUE             0x01 // Blue foreground attribute
#define FG_GREEN            0x02 // Green foreground attribute
#define FG_CYAN             0x03 // Cyan foreground attribute
#define FG_RED              0x04 // Red foreground attribute
#define FG_MAGENTA          0x05 // Magenta foreground attribute
#define FG_BROWN            0x06 // Brown foreground attribute
#define FG_LTGRAY           0x07 // Light gray foreground attribute
#define FG_DKGRAY           0x08 // Dark gray foreground attribute
#define FG_LTBLUE           0x09 // Light blue foreground attribute
#define FG_LTGREEN          0x0A // Light green foreground attribute
#define FG_LTCYAN           0x0B // Light cyan foreground attribute
#define FG_LTRED            0x0C // Light red foreground attribute
#define FG_LTMAGENTA        0x0D // Light magenta foreground attribute
#define FG_YELLOW           0x0E // Yellow foreground attribute
#define FG_WHITE            0x0F // White foreground attribute


/* Background colors */
#define BG_BLACK            0x00 // Black background attribute
#define BG_BLUE             0x10 // Blue background attribute
#define BG_GREEN            0x20 // Green background attribute
#define BG_CYAN             0x30 // Cyan background attribute
#define BG_RED              0x40 // Red background attribute
#define BG_MAGENTA          0x50 // Magenta background attribute
#define BG_BROWN            0x60 // Brown background attribute
#define BG_LTGRAY           0x70 // Light gray background attribute
#define BG_BKBLACK          0x80 // Blinking black background attribute
#define BG_BKBLUE           0x90 // Blinking blue background attribute
#define BG_BKGREEN          0xA0 // Blinking green background attribute
#define BG_BKCYAN           0xB0 // Blinking cyan background attribute
#define BG_BKRED            0xC0 // Blinking red background attribute
#define BG_BKMAGENTA        0xD0 // Blinking magenta background attribute
#define BG_BKYELLOW         0xE0 // Blinking yellow background attribute
#define BG_BKWHITE          0xF0 // Blinking white background attribute


/* I should put these somewhere else, but for now it's fine */
#define ERROR   "\033[91;40m[!] "
#define INFO    "\033[92;40m[i] "
#define LINE    "\033[93;40m[o] \033[0m"
#define WARN    "\033[33;40m[!] \033[0m"
#define INIT    "\033[92;40m[...] \033[0m"


/**
 * Set the shape of the cursor.
 *
 * @param shape The new shape of the cursor.
 */
void setCursor(uint8_t shape);


/**
 * Clear the text-mode screen content and change color scheme
 *
 * @param color The color to which the screen will be set
 * @note If the color is NULL, the screen will be cleaned with the default scheme
 */
void setScreen(uint8_t color);


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
int ttyPutChar(const char character, int col, int row, uint8_t color);


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
int ttyPutText(const char *string, int col, int row, uint8_t color);


/**
 * Get and print all the avialible charaters from the BIOS charset
 */
void ttyCharset(void);


/**
 * Print a string at the current cursor position on the screen.
 *
 * @param string The string to print.
 */
void ttyPrintStr(const char *string);


/**
 * Print a string at the current cursor position on the screen
 *
 * @note Support ANSI color escape sequences
 *
 * @param string The string to print.
 */
void ttyPrintLog(const char *string);


/**
 * ### Prints formatted output to the screen (printf)
 *
 * - #### Arguments:
 *     `format`: The format string containing format specifiers.
 *     `...`   : Additional arguments corresponding to the format specifiers.
 *
 * - #### Supported format specifiers:
 *     `%d` or `%i` for integers.
 *     `%f` for floats.
 *     `%x` for hexadecimal (lowercase) and `%X` for hexadecimal (uppercase).
 *     `%o` for octal.
 *     `%s` for strings.
 *     `%c` for characters.
 *
 * - #### Flags:
 *     `-` : Left-justify within the given field width.
 *     `0` : Pad with zeros instead of spaces.
 *
 * - #### Optional Width and Precision:
 *     `*` : Width or precision specified by an integer argument.
 *     `.` : Precision specified by an integer argument.
 *
 * - #### Length Specifiers:
 *     `l` : For long integers.
 *     `h` : For short integers.
 *
 * - #### NOTE:
 *     If an unsupported specifier is encountered, it prints `%` followed by the unsupported specifier.
 */
void ttyPrintFmt(const char *format, ...);


/**
 * Print a pretty formatted output to the console (printf)
 */
void ttyPrintOut(const char *prompt, const char *format, ...);


void moveCursor(uint16_t col, uint16_t row);
char getCharacter(void);

#endif /* DRIVER_SCREEN_H_ */
