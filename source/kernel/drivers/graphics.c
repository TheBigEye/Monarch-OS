#include "graphics.h"
#include "VGA/VGA.h"

#include "../CPU/HAL.h"
#include "../memory/heap.h"
#include "../memory/memory.h"

/**
 * Sets the color of a single pixel on the screen.
 *
 * @param color The color of the pixel. This is an 8-bit value,
 *              where each bit represents a different color plane.
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 */
void plotPixel(uint8_t color, uint32_t x, uint32_t y) {
    if (x >= GRAPHMODE_WIDTH || y >= GRAPHMODE_HEIGHT) return;

    // Pointer to the start of the screen memory
    uint8_t *SCREEN_MEMORY = (uint8_t *) GRAPHMODE_BUFFER;

    // Calculate the offset into the screen memory for the pixel
    // The screen width is divided by 8 because each byte represents 8 pixels
    SCREEN_MEMORY += (y * GRAPHMODE_WIDTH + x) >> 3;

    // Calculate the bitmask within the byte that represents the pixel
    uint8_t bit = 1 << (7 - (x & 7));

    // Loop over each color plane
    for (uint8_t plane = 0; plane < 4; plane++) {
        uint8_t mask = 1 << plane;

        // Select the memory plane. The VGA hardware uses a technique called
        // "planar" memory, where each color plane is stored separately
        // We need to select the correct plane before we can write to it
        writeByteToPort(GRAPHICS_INDEX, REG_GRAPHICS_MAP_READ);
        writeByteToPort(GRAPHICS_DATA, plane);

        // set bitmask
        writeByteToPort(SEQUENCER_INDEX, REG_SEQUENCER_MASK);
        writeByteToPort(SEQUENCER_DATA, mask);

        // If the corresponding bit in the color parameter is set,
        // set the bit in the pixel, otherwise clear it
        if (color & mask) {
            *SCREEN_MEMORY |= bit;
        } else {
            *SCREEN_MEMORY &=~ bit;
        }
    }
}

/**
 * Fills the entire screen with a specified color.
 *
 * @param color The color to fill the screen with. This is an 8-bit value,
 *              where each bit represents a different color plane.
 */
void fillScreen(uint8_t color) {
    // Pointer to the start of the screen memory
    uint8_t *SCREEN_MEMORY = (uint8_t *) GRAPHMODE_BUFFER;

    // The size of the screen memory in bytes
    uint32_t SCREEN_SIZE = (uint32_t) GRAPHMODE_SIZE >> 2;

    // Loop over each color plane
    for (uint8_t plane = 0; plane < 4; plane++) {
        uint8_t mask = 1 << plane;

        // Select the current color plane
        writeByteToPort(GRAPHICS_INDEX, REG_GRAPHICS_MAP_READ);
        writeByteToPort(GRAPHICS_DATA, plane);

        // set bitmask
        writeByteToPort(SEQUENCER_INDEX, REG_SEQUENCER_MASK);
        writeByteToPort(SEQUENCER_DATA, mask);

        // Fill the entire plane with the color. If the corresponding bit in
        // the color parameter is set, fill the plane with 0xFF (white),
        // otherwise fill it with 0x00 (black)
        fastFastMemorySet(SCREEN_MEMORY, (color & mask) ? 0xFF : 0x00, SCREEN_SIZE);
    }
}

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
void drawBitmap(uint8_t pixels[], uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    // Pointer to the start of the screen memory
    uint8_t *SCREEN_MEMORY = (uint8_t *) GRAPHMODE_BUFFER;

    // Calculate the starting offset into the screen memory for the pixel
    SCREEN_MEMORY += (y * GRAPHMODE_WIDTH + x) >> 3;

    // Loop over each color plane
    for (uint8_t plane = 0; plane < 4; plane++) {
        uint8_t mask = 1 << plane;

        // Select the current color plane
        writeByteToPort(GRAPHICS_INDEX, REG_GRAPHICS_MAP_READ);
        writeByteToPort(GRAPHICS_DATA, plane);

        // set bitmask
        writeByteToPort(SEQUENCER_INDEX, REG_SEQUENCER_MASK);
        writeByteToPort(SEQUENCER_DATA, mask);

        // Loop over each row
        for (uint32_t row = 0; row < h; row++) {
            // Calculate the starting offset into the pixels array for the current row
            uint32_t row_offset = row * w;

            // Calculate the starting offset into the screen memory for the current row
            uint8_t *screen_row = SCREEN_MEMORY + ((row + y) * GRAPHMODE_WIDTH + x) / 8;

            // Copy row of pixels from the pixels array to the screen memory
            for (uint32_t col = 0; col < w; col++) {
                uint8_t color = pixels[row_offset + col];
                uint8_t bit = 1 << (7 - ((col + x) & 7));

                if (color & mask) {
                    *(screen_row + (col >> 3)) |= bit;
                } else {
                    *(screen_row + (col >> 3)) &= ~bit;
                }
            }
        }
    }
}



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
void drawBitmapFast(uint8_t pixels[], uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    // Pointer to the start of the screen memory
    uint8_t *SCREEN_MEMORY = (uint8_t *) GRAPHMODE_BUFFER;

    // Create a temporary buffer for the bitmap
    uint8_t *BITMAP_BUFFER = (uint8_t *) memoryAllocateBlock((w * h) / 8);

    // Calculate the starting offset into the screen memory for the pixel
    SCREEN_MEMORY += (y * GRAPHMODE_WIDTH + x) >> 3;

    // Loop over each color plane
    for (uint8_t plane = 0; plane < 4; plane++) {
        uint8_t mask = 1 << plane;

        // Select the current color plane
        writeByteToPort(GRAPHICS_INDEX, REG_GRAPHICS_MAP_READ);
        writeByteToPort(GRAPHICS_DATA, plane);

        // Set bitmask for the current plane
        writeByteToPort(SEQUENCER_INDEX, REG_SEQUENCER_MASK);
        writeByteToPort(SEQUENCER_DATA, mask);

        // Copy row of pixels from the pixels array to the temporary buffer
        for (uint32_t row = 0; row < h; row++) {
            // Calculate the starting offset into the pixels array for the current row
            uint32_t row_offset = row * w;

            // Calculate the starting offset into the temporary buffer for the current row
            uint8_t *screen_row = BITMAP_BUFFER + row_offset / 8;

            // Copy pixels from the pixels array to the temporary buffer
            for (uint32_t column = 0; column < w; column++) {
                uint8_t color = pixels[row_offset + column];
                uint8_t bit = 1 << (7 - (column & 7));

                // Check if the pixel should be set in the current color plane
                if (color & mask) {
                    *(screen_row + (column >> 3)) |= bit;  // Set bit in buffer
                } else {
                    *(screen_row + (column >> 3)) &= ~bit; // Clear bit in buffer
                }
            }
        }

        // Calculate the starting offset into the screen memory for the first row of the bitmap
        uint8_t *screen_offset = SCREEN_MEMORY + ((y * GRAPHMODE_WIDTH + x) >> 3);

        // Copy the entire temporary buffer to the corresponding location in screen memory
        for (uint32_t row = 0; row < h; row++) {
            uint8_t *destination = screen_offset + (row * GRAPHMODE_WIDTH) / 8;
            uint8_t *source = BITMAP_BUFFER + (row * w) / 8;
            fastMemoryCopy(destination, source, w >> 3);
        }
    }

    // Free the memory from the buffer
    memoryFreeBlock(BITMAP_BUFFER);
}


void drawLine(uint8_t color, uint32_t fx, uint32_t fy, uint32_t sx, uint32_t sy) {
    int32_t dx = ABS((sx - fx)); // delta X
    int32_t dy = -ABS((sy - fy)); // delta Y
    int32_t ix = (fx < sx) ? 1 : -1; // sign of X direction
    int32_t iy = (fy < sy) ? 1 : -1; // sign of Y direction

    // errors
    int32_t fe = dx + dy;
    int32_t se;

    while (1) {
        plotPixel(color, fx, fy);
        if (fx == sx && fy == sy) break;

        se = fe * 2;
        if (se >= dy) {
            fe += dy;
            fx += ix;
        }
        if (se <= dx) {
            fe += dx;
            fy += iy;
        }
    }
}


void drawRect(uint8_t color, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    for (uint32_t row = 0; row < h; row++) {
        for (uint32_t col = 0; col < w; col++) {
            plotPixel(color, x + col, y + row);
        }
    }
}

void drawEmptyCircle(uint8_t color, uint32_t cx, uint32_t cy, uint32_t r) {
    int32_t x = 0;
    int32_t y = r;
    int32_t p = 1 - r;

    while (x <= y) {
        // Draw symmetric points
        plotPixel(color, cx + x, cy + y);
        plotPixel(color, cx - x, cy + y);
        plotPixel(color, cx + x, cy - y);
        plotPixel(color, cx - x, cy - y);
        plotPixel(color, cx + y, cy + x);
        plotPixel(color, cx - y, cy + x);
        plotPixel(color, cx + y, cy - x);
        plotPixel(color, cx - y, cy - x);

        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}

void drawSolidCircle(uint8_t color, uint16_t cx, uint16_t cy, uint16_t r) {
    for (int16_t y = -r; y <= r; y++) {
        for (int16_t x = -r; x <= r; x++) {
            if ((x*x + y*y) < (r*r - r)) {
                plotPixel(color, cx + x, cy + y);
            }
        }
    }
}



/**
 * Draws a character on the screen.
 *
 * @param c The character to draw.
 * @param x The x-coordinate where the character will be drawn.
 * @param y The y-coordinate where the character will be drawn.
 * @param color The color of the character. This is an 8-bit value,
 *              where the lower 4 bits represent the foreground color
 *              and the upper 4 bits represent the background color.
 */
void drawCharacter(unsigned char character, uint32_t x, uint32_t y, uint8_t color) {
    // Extract the foreground and background colors from the color parameter.
    uint8_t fgcolor = color & 0x0F;
    uint8_t bgcolor = (color >> 4) & 0x0F;
    uint8_t chrsize = 8; // We use 8x8 font

    // Get the glyph for the character from the font. The glyph is an 8x8 bitmap that represents the character.
    // Each byte in the glyph represents one row of 8 pixels in the character.
    uint8_t *glyph = small_font + (int)character * chrsize;

    // Loop over each row in the glyph.
    for (uint8_t cy = 0; cy < chrsize; cy++) {
        // Get the current row from the glyph.
        uint8_t row = glyph[cy];

        // Loop over each column in the row.
        for (uint8_t cx = 0; cx < chrsize; cx++) {
            // Create a mask for the current pixel. The mask is a byte where only one bit is set.
            // The position of the set bit corresponds to the position of the pixel in the row.
            // For example, if cx is 0, the mask will be 10000000 in binary. If cx is 7, the mask will be 00000001.
            uint8_t mask = 1 << (7 - cx);

            // Draw the pixel at the corresponding position.
            // If the corresponding bit in the row is set (i.e., the bitwise AND of the row and the mask is not zero),
            // then the pixel is drawn with the foreground color. Otherwise, it is drawn with the background color.
            plotPixel((row & mask) ? fgcolor : bgcolor, x + cx, y + cy);
        }
    }
}


void drawString(const char *string, uint32_t x, uint32_t y, uint8_t color) {
    uint32_t orig_x = x;
    uint32_t width = GRAPHMODE_WIDTH >> 3;
    uint32_t height = GRAPHMODE_HEIGHT >> 3;

    while (*string) {
        if (*string == '\n') {
            y += 8;
            x = orig_x;
        } else if (*string == '\t') {
            x += 8 * 4;
        } else if (*string == '\r') {
            x = orig_x;
        } else {
            drawCharacter(*string, x, y, color);
            x += 8;
        }

        if (x >= width << 3) {
            x = orig_x;
            y += 8;
        }

        if (y >= height << 3) {
            break;
        }

        string++;
    }
}
