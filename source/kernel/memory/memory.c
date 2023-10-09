#include "memory.h"

#include "../bugfault.h"

/* This should be computed at link time, but a hardcoded
 * value is fine for now. Remember that our kernel starts
 * at 0x1000 as defined in the Makefile */
uint32_t freeMemoryAddress = 0x10000;

/**
 * Copy a block of memory from the source to the destination. (memcpy)
 *
 * @param source    Pointer to the source memory block
 * @param destination   Pointer to the destination memory block
 * @param nbytes    Number of bytes to copy
 */
void memoryCopy(uint8_t *source, uint8_t *destination, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        *(destination + i) = *(source + i);
    }
}

/**
 * Set a block of memory with a specified value. (memset)
 *
 * @param destination   Pointer to the memory block to be set
 * @param value     Value to set each byte in the memory block to
 * @param len   Length of the memory block in bytes
 */
void memorySet(uint8_t *destination, uint8_t value, uint32_t len) {
    uint8_t *temp = (uint8_t *)destination;
    for (; len != 0; len--) {
        *temp++ = value;
    }
}

/**
 * Allocate a block of memory with a specified size and alignment. (malloc)
 *
 * @param size  Size of the memory block to allocate in bytes
 * @param align Alignment of the memory block (1 for 4K alignment)
 * @param physicalAddress   Pointer to store the physical address of the allocated memory block
 * @return  The starting address of the allocated memory block
 */
uint32_t memoryAllocate(size_t size, int align, uint32_t *physicalAddress) {
    ASSERT(align <= PAGE_SIZE);

    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (freeMemoryAddress & ~0xFFF)) {
        freeMemoryAddress += 0xFFF;
        freeMemoryAddress &= ~0xFFF;
    }

    /* Save also the physical address */
    if (physicalAddress) *physicalAddress = freeMemoryAddress;

    uint32_t ret = freeMemoryAddress;
    freeMemoryAddress += size; /* Remember to increment the pointer */
    return ret;
}
