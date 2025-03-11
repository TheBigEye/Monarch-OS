#include "heap.h"
#include "memory.h"

#include "../drivers/console.h"
#include "../drivers/COM/serial.h"
#include "../bugfault.h"

static uint32_t last_alloc = 0;
static uint32_t heap_end = 0;
static uint32_t heap_begin = 0;
static uint32_t pheap_begin = 0;
static uint32_t pheap_end = 0;
static uint8_t *pheap_desc = 0;
static uint32_t memory_used = 0;

void initializeMemory(uint32_t kernel_end) {

    // If kernel_end isnt upper to zero ...
    BUG_FATAL((kernel_end > 0), "Invalid 'kernel_end' value!");

    last_alloc = kernel_end + 0x1000;
    heap_begin = last_alloc;
    pheap_end = 0x1000000;  // Set to 16MB
    pheap_begin = pheap_end - (MAX_PAGE_ALIGNED_ALLOCS * 4096);
    heap_end = pheap_begin;

    memorySet((char *) heap_begin, 0, heap_end - heap_begin);
    pheap_desc = (uint8_t *) memoryAllocateBlock(MAX_PAGE_ALIGNED_ALLOCS);

    comPrintFmt("[i] Kernel heap starts at %X\n\n", last_alloc);
}

void memoryGetStatus(void) {
    ttyPrintLog(INFO "Memory Heap Status:\n");
    ttyPrintFmt(" * Memory used: %d bytes\n", memory_used);
    ttyPrintFmt(" * Memory free: %d bytes\n\n", heap_end - heap_begin - memory_used);

    ttyPrintFmt(" * Memory-Heap size: %d bytes\n", heap_end - heap_begin);
    ttyPrintFmt(" * Memory-Heap head: %X\n", heap_begin);
    ttyPrintFmt(" * Memory-Heap tail: %X\n\n", heap_end);

    ttyPrintFmt(" * Paging-Heap size: %d bytes\n", pheap_end - pheap_begin);
    ttyPrintFmt(" * Paging-Heap head: %X\n", pheap_begin);
    ttyPrintFmt(" * Paging-Heap tail: %X\n\n", pheap_end);
}


void memoryFreeBlock(void *mem) {
    BUG_CHECK((mem == NULL), "Attempting to free NULL pointer-block!");

    alloc_t *alloc = ((alloc_t *)((uint8_t *) mem - sizeof(alloc_t)));
    // uint32_t before = memory_used;

    memory_used -= (alloc->size + sizeof(alloc_t));
    alloc->status = 0;

    // comPrintFmt("[DEBUG] Free %d bytes from %x to %x, before -> %d bytes, now -> %d bytes\n", alloc->size, (uint32_t)alloc + sizeof(alloc_t), last_alloc, before, memory_used);
}

void memoryFreePages(void *mem) {
    BUG_CHECK((mem == NULL), "Attempting to free NULL pointer-page!");

    BUG_CHECK(
        ((uint32_t) mem < pheap_begin) || ((uint32_t) mem > pheap_end),

        "Memory address out of paging heap range!"
    );

    /* Determine which page is it */
    uint32_t ad = ((uint32_t) mem);
    ad -= pheap_begin;
    ad /= 4096;

    /* Now, ad has the id of the page */
    pheap_desc[ad] = 0;
    return;
}

char* memoryAllocatePages(uint32_t size) {
    /* Loop through the avail_list */
    for (int i = 0; i < MAX_PAGE_ALIGNED_ALLOCS; i++) {
        if (pheap_desc[i]) {
            continue;
        }

        pheap_desc[i] = 1;

        comPrintFmt("[DEBUG] Page allocated from %X to %X\n", pheap_begin + i*4096, pheap_begin + (i+1)*4096);

        return ((char *)(pheap_begin + i * 4096));
    }
    // ttyPrintFmt("memoryAllocatePages: FATAL: failure!\n");
    return 0;
}

char* memoryAllocateBlock(uint32_t size) {
    if (!size) return NULL;

    /* Loop through blocks and find a block sized the same or bigger */
    uint8_t *mem = (uint8_t *) heap_begin;

    while ((uint32_t) mem < last_alloc) {
        alloc_t *a = (alloc_t *) mem;

        /* If the alloc has no size, we have reaced the end of allocation */
        // comPrintFmt("[DEBUG] mem=%X a={.status=%d, .size=%d}\n", mem, a->status, a->size);
        if (!a->size) {
            goto nalloc;
        }

        /* If the alloc has a status of 1 (allocated), then add its size
         * and the sizeof alloc_t to the memory and continue looking.
         */
        if (a->status) {
            mem += a->size;
            mem += sizeof(alloc_t);
            mem += 4;
            continue;
        }

        /* If the is not allocated, and its size is bigger or equal to the
         * requested size, then adjust its size, set status and return the location.
         */
        if (a->size >= size) {
            /* Set to allocated */
            a->status = 1;

            // ttyPrintFmt("RE:Allocated %d bytes from %X to %X\n", size, mem + sizeof(alloc_t), mem + sizeof(alloc_t) + size);

            memorySet((mem + sizeof(alloc_t)), 0, size);
            memory_used += (size + sizeof(alloc_t));
            return (char *)(mem + sizeof(alloc_t));
        }

        /* If it isn't allocated, but the size is not good, then
         * add its size and the sizeof alloc_t to the pointer and
         * continue;
         */
        mem += a->size;
        mem += sizeof(alloc_t);
        mem += 4;
    }

    nalloc:;
    if ((last_alloc + size + sizeof(alloc_t)) >= heap_end) {
        comPrintFmt("[ERROR] Cannot allocate %d bytes!\n", size);
        THROW("Out of memory!\n");
    }

    alloc_t *alloc = ((alloc_t *) last_alloc);
    alloc->status = 1;
    alloc->size = size;

    last_alloc += size;
    last_alloc += sizeof(alloc_t);
    last_alloc += 4;

    // comPrintFmt("[DEBUG] Allocated %d bytes from %x to %x\n", size, (uint32_t)alloc + sizeof(alloc_t), last_alloc);

    memory_used += (size + 4 + sizeof(alloc_t));
    memorySet((char *)((uint32_t) alloc + sizeof(alloc_t)), 0, size);
    return (char *)((uint32_t) alloc + sizeof(alloc_t));
}
