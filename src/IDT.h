#ifndef IDT_H_
#define IDT_H_

#include "IO.h"
#include "common/Typedefs.h"
#include "common/Keycodes.h"
#include "drivers/Screen.h"

struct IDT64 {
    uint_16 offset_low;
    uint_16 selector;
    uint_8 ist;
    uint_8 types_attr;
    uint_16 offset_mid;
    uint_32 offset_high;
    uint_32 zero;
};

extern void(*MainKeyboardHandler)(uint_8 scanCode, uint_8 chr);
void InitializeIDT();

#endif /* IDT_H_ */
