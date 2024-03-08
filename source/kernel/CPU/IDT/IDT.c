#include "IDT.h"

#include "../../../common/sysutils.h"

/*
* IDT - Interrupt Descriptor Table, a data structure used by the operating system
* to manage and handle hardware and software interrupts
*
* @see https://wiki.osdev.org/IDT#Interrupt-Descriptor-Table
*/

void set_idt_gate(uint8_t gate, uint32_t handler) {
    idt[gate].low_offset = low_16(handler);
    idt[gate].sel = KERNEL_CS;
    idt[gate].always0 = 0;
    idt[gate].flags = 0x8E;
    idt[gate].high_offset = high_16(handler);
}

void set_idt(void) {
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;

    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
