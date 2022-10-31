#include "MemoryMap.h"

#include "common/Typedefs.h"
#include "drivers/Screen.h"

uint_8 UsableMemoryRegionCount;
MemoryMapEntry* UsableMemoryRegions[10];

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position) {
    SetCursorPosition(position);

    // Usable (normal) RAM
    PrintString("Memory Base: "); PrintString(IntegerToString(memoryMap -> BaseAddress));
    SetCursorPosition(position + 80);

    // Reserved - unusable
    PrintString("Memory Length: "); PrintString(IntegerToString(memoryMap -> RegionLength));
    SetCursorPosition(position + 160);

    // ACPI reclaimable memory
    PrintString("Memory Type: "); PrintString(IntegerToString(memoryMap -> RegionType));
    SetCursorPosition(position + 240);

    // Area containing bad memory
    PrintString("Memory Attributes: "); PrintString(HexToString(memoryMap -> ExtendedAttributes));
    SetCursorPosition(position + 400);
}

bool MemoryRegionGot = false;

MemoryMapEntry** GetUsableMemoryRegions() {
    if (MemoryRegionGot) {
        return UsableMemoryRegions;
    }

    uint_8 UsableRegionIndex;
    for (uint_8 i = 0; i < MemoryRegionCount; i++) {
        MemoryMapEntry* memMap = (MemoryMapEntry*)0x5000;
        memMap += i;
        if (memMap -> RegionType == 1) {
            UsableMemoryRegions[UsableRegionIndex] = memMap;
            UsableRegionIndex++;
        }
    }
    UsableMemoryRegionCount = UsableRegionIndex;

    MemoryRegionGot = true;
    return UsableMemoryRegions;
}
