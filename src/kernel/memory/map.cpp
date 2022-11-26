#include "map.h"

#include "../../common/typedefs.h"
#include "../drivers/display.h"

uint_8 UsableMemoryRegionCount = 0;
MemoryMapEntry* UsableMemoryRegions[10];

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position) {
    display::set_cursor_pos(position);

    // Usable (normal) RAM
    display::print("Memory Base: "); display::print(int_to_string(memoryMap -> BaseAddress));
    display::set_cursor_pos(position + 80);

    // Reserved - unusable
    display::print("Memory Length: "); display::print(int_to_string(memoryMap -> RegionLength));
    display::set_cursor_pos(position + 160);

    // ACPI reclaimable memory
    display::print("Memory Type: "); display::print(int_to_string(memoryMap -> RegionType));
    display::set_cursor_pos(position + 240);

    // Area containing bad memory
    display::print("Memory Attributes: "); display::print(hex_to_string(memoryMap -> ExtendedAttributes));
    display::set_cursor_pos(position + 400);
}

bool MemoryRegionGot = false;

MemoryMapEntry** GetUsableMemoryRegions() {
    if (MemoryRegionGot) {
        return UsableMemoryRegions;
    }

    uint_8 UsableRegionIndex = 0;
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
