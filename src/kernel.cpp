#include "drivers/display.h"
#include "drivers/keyboard.h"

#include "IDT.h"
#include "memory/map.h"
#include "memory/heap.h"

extern const char Splash[];

extern "C" void _start() {
    InitializeIDT();
    MainKeyboardHandler = KeyboardHandler;

    MemoryMapEntry** UsableMemoryMaps = GetUsableMemoryRegions();

    InitializeHeap(0x100000, 0x100000);

    /*uint_64* TestAddress = (uint_64*)aligned_alloc(0x4000, 0x08);
    PrintString(HexToString((uint_64)TestAddress));
    PrintString("\n\r");
    free(TestAddress);

    uint_64* TestAddress2 = (uint_64*)malloc(0x4000);
    PrintString(HexToString((uint_64)TestAddress2));
    PrintString("\n\r");*/

    ClearScreen(BACKGROUND_BLACK | FOREGROUND_WHITE);
    SetCursorPosition(PositionFromCoords(0, 0));
    PrintString(Splash, BACKGROUND_BLACK | FOREGROUND_BROWN);

    PrintString("$", BACKGROUND_BLACK | FOREGROUND_GREEN);

    return;
}
