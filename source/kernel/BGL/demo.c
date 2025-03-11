#include "demo.h"
#include "draw.h"

#include "../binaries.h"
#include "../CPU/PIT/timer.h"
#include "../drivers/graphics.h"


void bglPlayWork(void) {
    // We create the main surface, we use existing bitmap pixels
    Surface* wallpaper = bglCreateSurfaceFrom(myhill_640, 640, 480);

    // We create another surface ...
    Surface* overlay = bglCreateSurfaceFrom(mywork_640, 640, 480);
    bglSetColorKey(overlay, PX_BLUE); // Blue color is transparency now ...
    bglSetBlendMode(overlay, BGL_BLEND_ALPHA); // Full alpha blending

    // We make a single surface blitting both surfaces
    Rect destRect = (Rect) {0, 0, 640, 640};
    bglBlit(overlay, NULL, wallpaper, &destRect); // We blit the overlay into the wallpaper

    // Now, we blit the modified wallpaper into the screen
    bglBlitToScreen(wallpaper, NULL, 0, 0);

    // And now ... we clean the objects
    bglDestroySurface(overlay); // Fist the overlay
    bglDestroySurface(wallpaper); // And then the wallpaper
}


void bglPlayDemo(void) {
    // These are our magic numbers for the demo animation
    const uint16_t SQUARE_SIZE = 64;      // A nice size for our bouncing square
    const uint16_t CIRCLE_RADIUS = 32;    // Half the size of the square for the circle
    const uint16_t MOVE_SPEED = 2;        // Not too fast, not too slow ...

    // First, we need our main canvas to draw everything on
    Surface* screen = bglCreateSurface(640, 480);
    if (!screen) return;

    // We need a pretty background for our animation
    Surface* background = bglCreateSurfaceFrom(myhill_640, 640, 480);
    if (!background) {
        bglDestroySurface(screen);    // Oops, clean up and return if we fail
        return;
    }

    // Now, we create special surfaces for our shapes
    // This way we only need to draw them once, then we can reuse them!
    Surface* squareSurface = bglCreateSurface(SQUARE_SIZE, SQUARE_SIZE);
    Surface* circleSurface = bglCreateSurface(CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2);
    if (!squareSurface || !circleSurface) {
        // If something goes wrong, we clean everything up ...
        bglDestroySurface(background);
        bglDestroySurface(screen);
        if (squareSurface) bglDestroySurface(squareSurface);
        if (circleSurface) bglDestroySurface(circleSurface);
        return;
    }

    // The circle surface needs a black background for transparency
    bglFillSurface(circleSurface, PX_BLACK);  // Fill it with black first
    bglSetColorKey(circleSurface, PX_BLACK); // We tell BGL that black is our transparency color
    bglSetBlendMode(circleSurface, BGL_BLEND_ALPHA); // Make it blend

    // Now we draw our shapes once
    Rect squareRect = (Rect) {0, 0, SQUARE_SIZE, SQUARE_SIZE};
    bglFillRect(squareSurface, &squareRect, PX_GREEN); // A green square
    bglFillCircle(circleSurface, CIRCLE_RADIUS, CIRCLE_RADIUS, CIRCLE_RADIUS, PX_RED);  // And a red circle

    // Starting positions and movement vectors for our shapes
    uint16_t square_x = 100;              // Square starts here ...
    uint16_t square_y = 100;
    int8_t square_dx = MOVE_SPEED;        // ... and moves this way
    int8_t square_dy = MOVE_SPEED;

    uint16_t circle_x = 200;              // Circle starts at a different spot ...
    uint16_t circle_y = 200;
    int8_t circle_dx = -MOVE_SPEED;       // ... and moves the opposite way
    int8_t circle_dy = MOVE_SPEED;

    // We need to keep track of what areas of the screen change
    // This makes our animation much more efficient!
    DirtyRectList* dirtyRects = bglCreateDirtyRectList(4);
    if (!dirtyRects) {
        // More cleanup if things go wrong ...
        bglDestroySurface(squareSurface);
        bglDestroySurface(circleSurface);
        bglDestroySurface(background);
        bglDestroySurface(screen);
        return;
    }

    // Let's start with a clean screen
    bglBlit(background, NULL, screen, NULL);       // Copy our background
    bglBlitToScreen(screen, NULL, 0, 0);          // Show it to the world

    uint16_t frame = 0;  // We'll run for 256 frames

    // Here comes the fun part
    while ((frame++) < 256) {
        // Start fresh each frame
        bglClearDirtyRects(dirtyRects);

        // Remember where our shapes were last frame
        Rect old_square = {square_x, square_y, SQUARE_SIZE, SQUARE_SIZE};
        Rect old_circle = {circle_x - CIRCLE_RADIUS, circle_y - CIRCLE_RADIUS, CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2};

        // Move our shapes according to their speed
        square_x += square_dx;
        square_y += square_dy;
        circle_x += circle_dx;
        circle_y += circle_dy;

        // Make our shapes bounce off the screen edges
        if (square_x <= 0 || square_x >= 640 - SQUARE_SIZE) square_dx = -square_dx;
        if (square_y <= 0 || square_y >= 480 - SQUARE_SIZE) square_dy = -square_dy;
        if (circle_x <= CIRCLE_RADIUS || circle_x >= 640 - CIRCLE_RADIUS) circle_dx = -circle_dx;
        if (circle_y <= CIRCLE_RADIUS || circle_y >= 480 - CIRCLE_RADIUS) circle_dy = -circle_dy;

        // Calculate where the shapes are now
        Rect new_square = {square_x, square_y, SQUARE_SIZE, SQUARE_SIZE};
        Rect new_circle = {circle_x - CIRCLE_RADIUS, circle_y - CIRCLE_RADIUS, CIRCLE_RADIUS * 2, CIRCLE_RADIUS * 2};

        // Mark all the areas that need updating
        bglAddDirtyRect(dirtyRects, &old_square);     // Where the square was
        bglAddDirtyRect(dirtyRects, &old_circle);     // Where the circle was
        bglAddDirtyRect(dirtyRects, &new_square);     // Where the square is going
        bglAddDirtyRect(dirtyRects, &new_circle);     // Where the circle is going

        // Clean up the old positions by restoring the background
        bglUpdateRects(screen, background, dirtyRects);

        // Draw our shapes in their new positions
        bglBlit(squareSurface, NULL, screen, &new_square);    // Place the square
        bglBlit(circleSurface, NULL, screen, &new_circle);    // Place the circle

        // Show the result to the screen
        bglBlitToScreen(screen, NULL, 0, 0);
    }

    // Cleanup resources
    bglDestroySurface(screen);
    bglDestroySurface(background);
    bglDestroySurface(squareSurface);
    bglDestroySurface(circleSurface);
    bglDestroyDirtyRectList(dirtyRects);
}


void bglPlayDemoEx(void) {
    const uint16_t SQUARE_SIZE = 240;
    const uint16_t MOVE_SPEED = 2;

    // Create main screen surface
    Surface* screen = bglCreateSurface(640, 480);
    if (!screen) return;

    // Create background surface
    Surface* background = bglCreateSurfaceFrom(myhill_640, 640, 480);
    if (!background) {
        bglDestroySurface(screen);
        return;
    }

    Surface* spritesheet = bglCreateSurfaceFrom(bigeye_480, 480, 480);
    if (!spritesheet) {
        bglDestroySurface(background);
        bglDestroySurface(screen);
        return;
    }

    Rect sprite_rect = (Rect) {0, 0, SQUARE_SIZE, SQUARE_SIZE}; // x, y, width, height
    Surface* sprite = bglCreateSprite(spritesheet, sprite_rect);
    bglSetColorKey(sprite, PX_BLACK); // Black is transparency now ...
    bglSetBlendMode(sprite, BGL_BLEND_ALPHA);

    // Initial positions for square and circle
    uint16_t square_x = 100;
    uint16_t square_y = 100;
    int8_t square_dx = MOVE_SPEED;
    int8_t square_dy = MOVE_SPEED;

    // Create dirty rect list, we only need 4 rects
    DirtyRectList* dirtyRects = bglCreateDirtyRectList(4);
    if (!dirtyRects) {
        bglDestroySurface(sprite);
        bglDestroySurface(spritesheet);
        bglDestroySurface(background);
        bglDestroySurface(screen);
        return;
    }

    // Copy background to screen initially
    bglBlit(background, NULL, screen, NULL);
    bglBlitToScreen(screen, NULL, 0, 0);

    uint16_t frame = 0;

    // Animation loop
    while ((frame++) < 256) {
        // Clear dirty rects from previous frame
        bglClearDirtyRects(dirtyRects);

        // Previous positions (for erasing)
        Rect old_square = {square_x, square_y, SQUARE_SIZE, SQUARE_SIZE};

        // Update positions
        square_x += square_dx;
        square_y += square_dy;

        // Bounce off edges
        if (square_x <= 0 || square_x >= 640 - SQUARE_SIZE) square_dx = -square_dx;
        if (square_y <= 0 || square_y >= 480 - SQUARE_SIZE) square_dy = -square_dy;

        // New positions
        Rect new_square = {square_x, square_y, SQUARE_SIZE, SQUARE_SIZE};

        // Add old and new positions to dirty rects
        bglAddDirtyRect(dirtyRects, &old_square);
        bglAddDirtyRect(dirtyRects, &new_square);

        // Restore background in dirty regions
        bglUpdateRects(screen, background, dirtyRects);

        bglBlit(sprite, &sprite_rect, screen, &new_square);

        // Update screen
        bglBlitToScreen(screen, NULL, 0, 0);

        // Simple delay
        //timerSleep(1);
    }

    // Cleanup
    bglDestroySurface(screen);
    bglDestroySurface(background);
    bglDestroySurface(spritesheet);
    bglDestroySurface(sprite);
    bglDestroyDirtyRectList(dirtyRects);
}
