#include "GDT.h"

#include "../HAL.h"

/* Setup a descriptor in the Global Descriptor Table */
void gdtSetGate(int gate, unsigned long base, unsigned long limit, uint8_t access, uint8_t gran) {
    /* Setup the descriptor base address */
    gdt[gate].base_low = (base & 0xFFFF);
    gdt[gate].base_middle = (base >> 16) & 0xFF;
    gdt[gate].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[gate].limit_low = (limit & 0xFFFF);
    gdt[gate].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[gate].granularity |= (gran & 0xF0);
    gdt[gate].access = access;
}

/*
 * Should be called by main. This will setup the special GDT
 * pointer, set up the first 3 entries in our GDT, and then
 * finally call gdtDoFlush() in our assembler file in order
 * to tell the processor where the new GDT is and update the
 * new segment registers
 */
void gdtDoInstall() {
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = &gdt;

    /* Our NULL descriptor */
    gdtSetGate(0, 0, 0, 0, 0);

    /* The second entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor.
    *  Please check the table above in the tutorial in order
    *  to see exactly what each value means */
    gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* The third entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
    gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    /* Flush out the old GDT and install the new changes! */
    gdtDoFlush();

    // Wait some milli-seconds
    operationSleep(2);
}
