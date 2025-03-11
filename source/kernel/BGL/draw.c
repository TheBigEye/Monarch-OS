#include "draw.h"
#include "../memory/memory.h"
#include "../memory/heap.h"

// Helper function to merge overlapping rectangles
static void mergeRects(Rect* r1, Rect* r2) {
    uint16_t x1 = MIN(r1->x, r2->x);
    uint16_t y1 = MIN(r1->y, r2->y);
    uint16_t x2 = MAX(r1->x + r1->w, r2->x + r2->w);
    uint16_t y2 = MAX(r1->y + r1->h, r2->y + r2->h);

    r1->x = x1;
    r1->y = y1;
    r1->w = x2 - x1;
    r1->h = y2 - y1;
}

DirtyRectList* bglCreateDirtyRectList(uint16_t max_rects) {
    if (max_rects > MAX_DIRTY_RECTS) max_rects = MAX_DIRTY_RECTS;
    DirtyRectList* list = (DirtyRectList*) memoryAllocateBlock(sizeof(DirtyRectList));
    if (list) {
        list->count = 0;
        list->max = max_rects;
    }
    return list;
}

void bglDestroyDirtyRectList(DirtyRectList* list) {
    if (list) memoryFreeBlock(list);
}

void bglAddDirtyRect(DirtyRectList* list, Rect* rect) {
    if (!list || !rect || list->count >= list->max) return;

    // Try to merge with existing rectangles
    for (uint16_t i = 0; i < list->count; i++) {
        Rect* current = &list->rects[i];
        // Check if rectangles overlap
        if (!(current->x + current->w < rect->x ||
              current->x > rect->x + rect->w ||
              current->y + current->h < rect->y ||
              current->y > rect->y + rect->h)) {
            mergeRects(current, rect);
            return;
        }
    }

    // Add as new rectangle if no merge possible
    list->rects[list->count++] = *rect;
}


void bglClearDirtyRects(DirtyRectList* list) {
    if (list) list->count = 0;
}


void bglUpdateRects(Surface* dst, Surface* src, DirtyRectList* list) {
    if (!dst || !src || !list) return;

    for (uint16_t i = 0; i < list->count; i++) {
        Rect* rect = &list->rects[i];
        bglBlit(src, rect, dst, rect);
    }
}


inline void bglDrawPixel(Surface* surface, uint16_t x, uint16_t y, uint8_t color) {
    if (!surface || x >= surface->w || y >= surface->h) return;

    uint32_t offset = (y * surface->pitch) + (x >> 1);

    if (x & 1) {
        surface->pixels[offset] = (surface->pixels[offset] & 0xF0) | color;
    } else {
        surface->pixels[offset] = (surface->pixels[offset] & 0x0F) | (color << 4);
    }
}


void bglDrawLine(Surface* surface, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color) {
    uint16_t dx = ABS((x2 - x1));
    uint16_t dy = ABS((y2 - y1));
    int8_t sx = x1 < x2 ? 1 : -1;
    int8_t sy = y1 < y2 ? 1 : -1;
    uint16_t err = dx - dy;

    uint16_t e2 = 0;

    while (1) {
        bglDrawPixel(surface, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;

        e2 = err << 1;

        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}


void bglDrawRect(Surface* surface, Rect* rect, uint8_t color) {
    if (!surface || !rect) return;
    bglDrawLine(surface, rect->x, rect->y, rect->x + rect->w, rect->y, color);
    bglDrawLine(surface, rect->x, rect->y + rect->h, rect->x + rect->w, rect->y + rect->h, color);
    bglDrawLine(surface, rect->x, rect->y, rect->x, rect->y + rect->h, color);
    bglDrawLine(surface, rect->x + rect->w, rect->y, rect->x + rect->w, rect->y + rect->h, color);
}


void bglFillRect(Surface* surface, Rect* rect, uint8_t color) {
    if (!surface || !rect) return;

    uint16_t start_x = MAX(rect->x, 0);
    uint16_t start_y = MAX(rect->y, 0);
    uint16_t end_x = MIN(rect->x + rect->w, surface->w);
    uint16_t end_y = MIN(rect->y + rect->h, surface->h);

    for (uint16_t y = start_y; y < end_y; y++) {
        for (uint16_t x = start_x; x < end_x; x++) {
            bglDrawPixel(surface, x, y, color);
        }
    }
}


// Drawing circles without sqrt - using midpoint circle algorithm
void bglDrawCircle(Surface* surface, uint16_t x0, uint16_t y0, uint16_t radius, uint8_t color) {
    int16_t x = radius;
    int16_t y = 0;
    int16_t err = 0;

    while (x >= y) {
        bglDrawPixel(surface, x0 + x, y0 + y, color);
        bglDrawPixel(surface, x0 + y, y0 + x, color);
        bglDrawPixel(surface, x0 - y, y0 + x, color);
        bglDrawPixel(surface, x0 - x, y0 + y, color);
        bglDrawPixel(surface, x0 - x, y0 - y, color);
        bglDrawPixel(surface, x0 - y, y0 - x, color);
        bglDrawPixel(surface, x0 + y, y0 - x, color);
        bglDrawPixel(surface, x0 + x, y0 - y, color);

        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}


void bglFillCircle(Surface* surface, uint16_t x, uint16_t y, uint16_t radius, uint8_t color) {
    for (int16_t i = -radius; i <= radius; i++) {
        for (int16_t j = -radius; j <= radius; j++) {
            if (((j * j) + (i * i)) < ((radius * radius) - radius)) {
                bglDrawPixel(surface, x + j, y + i, color);
            }
        }
    }
}


// Surface transformation functions
void bglScrollSurface(Surface* surface, Rect* rect, int16_t dx, int16_t dy) {
    if (!surface || !rect) return;

    Surface* temp = bglCreateSurface(rect->w, rect->h);
    if (!temp) return;

    // Copy region to temp surface
    bglBlit(surface, rect, temp, NULL);

    // Clear original region
    uint8_t zero = 0;
    bglFillRect(surface, rect, zero);

    // Calculate new position
    Rect dst = {
        rect->x + dx,
        rect->y + dy,
        rect->w,
        rect->h
    };

    // Copy back with offset
    bglBlit(temp, NULL, surface, &dst);
    bglDestroySurface(temp);
}
