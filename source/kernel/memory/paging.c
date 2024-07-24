#include "paging.h"

#include "../drivers/console.h"

static uint32_t* page_directory = 0;
static uint32_t page_dir_loc = 0;
static uint32_t* last_page = 0;

/*
 * Paging now will be really simple
 * we reserve 0-16MB for kernel stuff
 * heap will be from approx 1mb to 8mb
 * and paging stuff will be from 8mb
 */

void initializePaging(void) {
    ttyPrintFmt("Setting up paging ...\n");
    page_directory = (uint32_t *) 0x800000;
    page_dir_loc = (uint32_t) page_directory;
    last_page = (uint32_t *) 0x808000;

    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0 | 2;
    }

    memoryPagingMap(0, 0);
    memoryPagingMap(0x800000, 0x800000);

    // Enable paging
    __asm__ __volatile__ ("mov %%eax, %%cr3": :"a"(page_dir_loc));
    __asm__ __volatile__ ("mov %cr0, %eax");
    __asm__ __volatile__ ("orl $0x80000000, %eax");
    __asm__ __volatile__ ("mov %eax, %cr0");

    ttyPrintFmt("Paging was successfully enabled!\n");
}

void memoryPagingMap(uint32_t virtual, uint32_t physical) {
    uint16_t id = virtual >> 22;

    for (int i = 0; i < 1024; i++) {
        last_page[i] = physical | 3;
        physical += PAGE_SIZE;
    }

    page_directory[id] = ((uint32_t)last_page) | 3;
    last_page = (uint32_t *)(((uint32_t) last_page) + PAGE_SIZE);
    ttyPrintFmt("Mapping %X (%d) to %X ...\n", virtual, id, physical);
}
