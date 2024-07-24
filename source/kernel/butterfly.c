#include "butterfly.h"

#include "CPU//GDT/GDT.h"
#include "CPU//ISR/ISR.h"

#include "drivers/COM/serial.h"
#include "drivers/VGA/video.h"
#include "drivers/console.h"

#include "memory/heap.h"
#include "memory/paging.h"

/* The kernel start, 'kernel_head' section */
extern uint32_t kernel_head;

/* The kernel end, 'kernel_tail' section */
extern uint32_t kernel_tail;

static multiboot_info_t butterfly_info;

void initializeKernel(multiboot_info_t* multiboot_header) {

    // Save the multiboot information for later :D
    butterfly_info = *multiboot_header;

    /* Get modules */
    // uint32_t initrd_pos = *((uint32_t*) multiboot_header->mods_addr);
    // uint32_t initrd_end = *((uint32_t*)(multiboot_header->mods_addr + 4));

    // Initialize debug serial port
    initializeCOM();

    // We init the screen in text mode
    initializeVGA(text_mode);

    // Clear the screen's trash
    setScreen(NULL);

    initializeGDT(); // Install the General Descriptor Table
    initializeISR(); // Intall exception handlers
    initializeIRQ(); // Install drivers

    // Clear debug messages ...
    setScreen(NULL);

    /* Initialize heap and paging */
    initializeMemory(&kernel_tail);
    initializePaging();


}

void multibootGetInfo(void) {
    ttyPrintLog(INFO "Multiboot Headers:\n");
    ttyPrintFmt(" * Mutiboot flags: %X\n", butterfly_info.flags);
    ttyPrintFmt(" * Lower memory: %d KB\n", butterfly_info.mem_lower);
    ttyPrintFmt(" * Upper memory: %d KB\n\n", butterfly_info.mem_upper);

    ttyPrintFmt(" * Boot device: %X\n", butterfly_info.boot_device);
    ttyPrintFmt(" * Command line: %s\n\n", (char *) butterfly_info.cmdline);

    ttyPrintFmt(" * Memory map length: %d\n", butterfly_info.mmap_length);
    ttyPrintFmt(" * Memory map Address: %X\n", butterfly_info.mmap_addr);
    ttyPrintFmt(" * Drives length: %d\n", butterfly_info.drives_length);
    ttyPrintFmt(" * Drives Address: %X\n\n", butterfly_info.drives_addr);

    ttyPrintFmt(" * Config table: %X\n", butterfly_info.config_table);
    ttyPrintFmt(" * Bootloader name: %s\n", (char *) butterfly_info.boot_loader_name);
    ttyPrintFmt(" * APM table: %X\n\n", butterfly_info.apm_table);
}
