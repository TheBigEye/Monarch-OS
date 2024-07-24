#ifndef DRIVER_GRAPHICS_H_
#define DRIVER_GRAPHICS_H_ 1

#include "../../common/sysutils.h"

/**
* Theoretically there are 16 colors, but it seems that
* there can be different shades if a few bits are changed
*/

#define PX_BLACK            0x00 // Black pixel color
#define PX_BLUE             0x11 // Blue pixel color
#define PX_GREEN            0x22 // Green pixel color
#define PX_CYAN             0x33 // Cyan pixel color
#define PX_RED              0x44 // Red pixel color
#define PX_MAGENTA          0x55 // Magenta pixel color
#define PX_BROWN            0x66 // Brown pixel color
#define PX_LTGRAY           0x77 // Light gray pixel color
#define PX_DKGRAY           0x88 // Dark gray pixel color
#define PX_LTBLUE           0x99 // Light blue pixel color
#define PX_LTGREEN          0xAA // Light green pixel color
#define PX_LTCYAN           0xBB // Light cyan pixel color
#define PX_LTRED            0xCC // Light red pixel color
#define PX_LTMAGENTA        0xDD // Light magenta pixel color
#define PX_YELLOW           0xEE // Yellow pixel color
#define PX_WHITE            0xFF // White pixel color


/**
 * Sets the color of a single pixel on the screen.
 *
 * @param color The color of the pixel. This is an 8-bit value,
 *              where each bit represents a different color plane.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 */
void plotPixel(uint8_t color, uint16_t x, uint16_t y);


/**
 * Reads the color of a pixel from the screen memory buffer.
 *
 * @param x The x-coordinate of the pixel
 * @param y The y-coordinate of the pixel
 * @return The color value of the pixel (0-15), or 0 if the coordinates are out of bounds
 */
uint8_t readPixel(uint16_t x, uint16_t y);


/**
 * Fills the entire screen with a specified color.
 *
 * @param color The color to fill the screen with.
 */
void fillScreen(uint8_t color);


/**
 * @brief Draws a bitmap on the screen plane by plane.
 *
 * @note Supports up to 4 color planes, allowing 16-color graphics (4 bits per pixel).
 *
 * @param pixels    Pointer to the bitmap pixel data.
 * @param x         X coordinate of the top left corner of the bitmap.
 * @param y         Y coordinate of the top left corner of the bitmap.
 * @param w         Width of the bitmap in pixels.
 * @param h         Height of the bitmap in pixels.
 *
 * @deprecated This is slow, we should use the fast double-bufferd version
 */
void drawBitmap(uint8_t pixels[], uint16_t x, uint16_t y, uint16_t w, uint16_t h);


/**
 * @brief Draws a bitmap on the screen (fast version).
 *
 * @note Uses double-buffering to copy the bitmap completely in each plane.
 *
 * @param pixels    Pointer to the bitmap pixel data.
 * @param x         X coordinate of the top left corner of the bitmap.
 * @param y         Y coordinate of the top left corner of the bitmap.
 * @param w         Width of the bitmap in pixels.
 * @param h         Height of the bitmap in pixels.
 */
void drawBitmapFast(uint8_t pixels[], uint16_t x, uint16_t y, uint16_t w, uint16_t h);


void drawLine(uint8_t color, uint16_t fx, uint16_t fy, uint16_t sx, uint16_t sy);


void drawEmptyRect(uint8_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);


void drawSolidRect(uint8_t color, uint16_t x, uint16_t y, uint16_t w, uint16_t h);


void drawEmptyCircle(uint8_t color, uint16_t cx, uint16_t cy, uint16_t r);


void drawSolidCircle(uint8_t color, uint16_t cx, uint16_t cy, uint16_t r);


void drawCharacter(unsigned char character, uint16_t x, uint16_t y, uint8_t color);


void drawString(const char *string, uint16_t x, uint16_t y, uint8_t color);


void drawCharset(void);


#endif /* DRIVER_GRAPHICS_H_ */
