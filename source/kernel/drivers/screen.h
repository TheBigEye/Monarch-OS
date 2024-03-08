#ifndef DRIVER_SCREEN_H_
#define DRIVER_SCREEN_H_ 1

#include <stdint.h>

/* Text colors */
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


void configureScreen(void);

int putCharacter(const char character, int column, int row, uint8_t attribute);

void setCursor(uint8_t shape);
void clearScreen(uint8_t color);

void putString(const char *message, int column, int row, uint8_t color);
void putColor(const char character, int column, int row, uint16_t width, uint16_t height, uint8_t color);
void printString(const char *message);
void printColor(const char *message, uint8_t color);

void printCharset(void);

void printFormat(const char *format, ...);
void printOutput(const char *prompt, uint8_t promptColor, const char *format, ...);

void moveCursor(uint16_t x, uint16_t y);
char getCharacter();

#endif /* DRIVER_SCREEN_H_ */
