#ifndef DRIVER_GRAPHICS_H_
#define DRIVER_GRAPHICS_H_ 1

#include "../../common/sysutils.h"

#define PX_BLACK            0x00 // Black foreground attribute
#define PX_BLUE             0x11 // Blue foreground attribute
#define PX_GREEN            0x22 // Green foreground attribute
#define PX_CYAN             0x33 // Cyan foreground attribute
#define PX_RED              0x44 // Red foreground attribute
#define PX_MAGENTA          0x55 // Magenta foreground attribute
#define PX_BROWN            0x66 // Brown foreground attribute
#define PX_LTGRAY           0x77 // Light gray foreground attribute
#define PX_DKGRAY           0x88 // Dark gray foreground attribute
#define PX_LTBLUE           0x99 // Light blue foreground attribute
#define PX_LTGREEN          0xAA // Light green foreground attribute
#define PX_LTCYAN           0xBB // Light cyan foreground attribute
#define PX_LTRED            0xCC // Light red foreground attribute
#define PX_LTMAGENTA        0xDD // Light magenta foreground attribute
#define PX_YELLOW           0xEE // Yellow foreground attribute
#define PX_WHITE            0xFF // White foreground attribute

void plotPixel(uint8_t color, uint32_t x, uint32_t y);
void fillScreen(uint8_t color);

void drawBitmap(uint8_t pixels[], uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void drawBitmapFast(uint8_t pixels[], uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void drawRect(uint8_t color, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void drawLine(uint8_t color, uint32_t fx, uint32_t fy, uint32_t sx, uint32_t sy);
void drawEmptyCircle(uint8_t color, uint32_t cx, uint32_t cy, uint32_t r);
void drawSolidCircle(uint8_t color, uint16_t cx, uint16_t cy, uint16_t r);

void drawCharacter(unsigned char character, uint32_t x, uint32_t y, uint8_t color);
void drawString(const char *string, uint32_t x, uint32_t y, uint8_t color);

#endif /* DRIVER_GRAPHICS_H_ */
