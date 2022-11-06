#include "map.h"

#include "../common/typedefs.h"
#include "../drivers/display.h"

uint_8 UsableMemoryRegionCount = 0;
MemoryMapEntry* UsableMemoryRegions[10];

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position) {
    display::set_cursor_pos(position);

    // Usable (normal) RAM
    display::print_string("Memory Base: "); display::print_string(int_to_string(memoryMap -> BaseAddress));
    display::set_cursor_pos(position + 80);

    // Reserved - unusable
    display::print_string("Memory Length: "); display::print_string(int_to_string(memoryMap -> RegionLength));
    display::set_cursor_pos(position + 160);

    // ACPI reclaimable memory
    display::print_string("Memory Type: "); display::print_string(int_to_string(memoryMap -> RegionType));
    display::set_cursor_pos(position + 240);

    // Area containing bad memory
    display::print_string("Memory Attributes: "); display::print_string(hex_to_string(memoryMap -> ExtendedAttributes));
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
