#include "butterfly.h"

#include "drivers/console.h"

multiboot_info_t butterfly_info;

void configureKernel(multiboot_info_t* multiboot_header) {
    // Save the multiboot information for later :D
    butterfly_info = *multiboot_header;
}

void butterflyGetStatus() {
    printColor("[i] Multiboot Headers:\n", BG_BLACK | FG_LTGREEN);
    printFormat(" * Mutiboot flags: %X\n", butterfly_info.flags);
    printFormat(" * Lower memory: %d KB\n", butterfly_info.mem_lower);
    printFormat(" * Upper memory: %d KB\n\n", butterfly_info.mem_upper);

    printFormat(" * Boot device: %X\n", butterfly_info.boot_device);
    printFormat(" * Command line: %s\n\n", (char *) butterfly_info.cmdline);

    printFormat(" * Memory map length: %d\n", butterfly_info.mmap_length);
    printFormat(" * Memory map Address: %X\n", butterfly_info.mmap_addr);
    printFormat(" * Drives length: %d\n", butterfly_info.drives_length);
    printFormat(" * Drives Address: %X\n\n", butterfly_info.drives_addr);

    printFormat(" * Config table: %X\n", butterfly_info.config_table);
    printFormat(" * Bootloader name: %s\n", (char *) butterfly_info.boot_loader_name);
    printFormat(" * APM table: %X\n", butterfly_info.apm_table);
}
