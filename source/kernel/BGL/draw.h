#ifndef _KERNEL_GRAPHICS_DRAW_H
#define _KERNEL_GRAPHICS_DRAW_H 1

#include "surface.h"

// Maximum number of dirty rectangles to track
#define MAX_DIRTY_RECTS 32

// Dirty rectangle tracking structure
typedef struct {
    Rect rects[MAX_DIRTY_RECTS];
    uint16_t count;
    uint16_t max;
} DirtyRectList;

// Dirty Rectangle Management
DirtyRectList* bglCreateDirtyRectList(uint16_t max_rects);
void bglDestroyDirtyRectList(DirtyRectList* list);
void bglAddDirtyRect(DirtyRectList* list, Rect* rect);
void bglClearDirtyRects(DirtyRectList* list);
void bglUpdateRects(Surface* dst, Surface* src, DirtyRectList* list);

// Drawing Functions
void bglDrawPixel(Surface* surface, uint16_t x, uint16_t y, uint8_t color);

void bglFlipSurfaceVertical(Surface* surface, Rect* rect);
void bglFlipSurfaceHorizontal(Surface* surface, Rect* rect);
void bglCopySurfaceRegion(Surface* src, Rect* srcrect, Surface* dst, Rect* dstrect);
void bglScrollSurface(Surface* surface, Rect* rect, int16_t dx, int16_t dy);

/// Surface Region Management
void bglFillCircle(Surface* surface, uint16_t x, uint16_t y, uint16_t radius, uint8_t color);
void bglDrawCircle(Surface* surface, uint16_t x, uint16_t y, uint16_t radius, uint8_t color);
void bglFillRect(Surface* surface, Rect* rect, uint8_t color);
void bglDrawRect(Surface* surface, Rect* rect, uint8_t color);
void bglDrawLine(Surface* surface, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);

#endif /* _KERNEL_GRAPHICS_DRAW_H */
