#ifndef _KERNEL_MEMORY_H
#define _KERNEL_MEMORY_H 1

#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096

/**
 * Copy a block of memory from the source to the destination. (memcpy)
 *
 * @param source    Pointer to the source memory block
 * @param destination   Pointer to the destination memory block
 * @param nbytes    Number of bytes to copy
 */
void memoryCopy(uint8_t *source, uint8_t *destination, int nbytes);

/**
 * Set a block of memory with a specified value. (memset)
 *
 * @param destination   Pointer to the memory block to be set
 * @param value     Value to set each byte in the memory block to
 * @param len   Length of the memory block in bytes
 */
void memorySet(uint8_t *destination, uint8_t value, uint32_t len);

/* At this stage there is no 'free' implemented. */

/**
 * Allocate a block of memory with a specified size and alignment. (malloc)
 *
 * @param size  Size of the memory block to allocate in bytes
 * @param align Alignment of the memory block (1 for 4K alignment)
 * @param physicalAddress   Pointer to store the physical address of the allocated memory block
 * @return  The starting address of the allocated memory block
 */
uint32_t memoryAllocate(size_t size, int align, uint32_t *physicalAddress);

#endif /* KERNEL_MEMORY_H */
