#ifndef _KERNEL_HEAP_H
#define _KERNEL_HEAP_H 1

#include "../../common/sysutils.h"

#define MAX_PAGE_ALIGNED_ALLOCS 32

typedef struct {
    uint8_t status;
    uint32_t size;
} alloc_t;

void initializeMemory(uint32_t kernel_end);
void memoryGetStatus(void);

char* memoryAllocatePages(uint32_t size); /* page aligned alloc */
char* memoryAllocateBlock(uint32_t size);
void memoryFreeBlock(void *mem);

#endif /* _KERNEL_HEAP_H */
