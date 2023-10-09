#include "ISR.h"

#include "../../../common/sysutils.h"

#include "../../drivers/screen.h"
#include "../../drivers/keyboard.h"

#include "../../bugfault.h"

#include "../IDT/IDT.h"
#include "../PIT/timer.h"
#include "../RTC/clock.h"
#include "../ports.h"


/*
* ISR - Interrupt Service Routines, code routines that are executed in response to an interrupt
* signal, handling specific events or tasks triggered by hardware or software interrupts
*
* @see https://wiki.osdev.org/ISR#Interrupt-Service-Routines
*/

isr_t interrupt_handlers[256];

/* Can't do this with a loop because we need the address of the function names */
void ISR_install() {

    // TODO: actually we *can* do this with a loop if you used a macro to generate all
    // the stubs, because you can also generate the addresses while you're at it

    /* Set the first 32 entries in the IDT to the first 32 ISRs */
    set_idt_gate(0,  (uint32_t) ISR_0);    set_idt_gate(1,  (uint32_t) ISR_1);
    set_idt_gate(2,  (uint32_t) ISR_2);    set_idt_gate(3,  (uint32_t) ISR_3);
    set_idt_gate(4,  (uint32_t) ISR_4);    set_idt_gate(5,  (uint32_t) ISR_5);
    set_idt_gate(6,  (uint32_t) ISR_6);    set_idt_gate(7,  (uint32_t) ISR_7);
    set_idt_gate(8,  (uint32_t) ISR_8);    set_idt_gate(9,  (uint32_t) ISR_9);
    set_idt_gate(10, (uint32_t) ISR_10);   set_idt_gate(11, (uint32_t) ISR_11);
    set_idt_gate(12, (uint32_t) ISR_12);   set_idt_gate(13, (uint32_t) ISR_13);
    set_idt_gate(14, (uint32_t) ISR_14);   set_idt_gate(15, (uint32_t) ISR_15);
    set_idt_gate(16, (uint32_t) ISR_16);   set_idt_gate(17, (uint32_t) ISR_17);
    set_idt_gate(18, (uint32_t) ISR_18);   set_idt_gate(19, (uint32_t) ISR_19);
    set_idt_gate(20, (uint32_t) ISR_20);   set_idt_gate(21, (uint32_t) ISR_21);
    set_idt_gate(22, (uint32_t) ISR_22);   set_idt_gate(23, (uint32_t) ISR_23);
    set_idt_gate(24, (uint32_t) ISR_24);   set_idt_gate(25, (uint32_t) ISR_25);
    set_idt_gate(26, (uint32_t) ISR_26);   set_idt_gate(27, (uint32_t) ISR_27);
    set_idt_gate(28, (uint32_t) ISR_28);   set_idt_gate(29, (uint32_t) ISR_29);
    set_idt_gate(30, (uint32_t) ISR_30);   set_idt_gate(31, (uint32_t) ISR_31);

    /* Remap the PIC - IRQ0 to IRQ15 have to be remapped to IDT entries 32 to 47 */

    // starts the initialization sequence
    writeByteToPort(0x20, 0x11); writeByteToPort(0xA0, 0x11);

    // define the PIC vectors
    writeByteToPort(0x21, 0x20); // Set offset of Master PIC to 0x20 (32): Entry 32-39
    writeByteToPort(0xA1, 0x28); // Set offset of Slave  PIC to 0x28 (40): Entry 40-47

    // continue initialization sequence
    writeByteToPort(0x21, 0x04); writeByteToPort(0xA1, 0x02);
    writeByteToPort(0x21, 0x01); writeByteToPort(0xA1, 0x01);
    writeByteToPort(0x21, 0x00); writeByteToPort(0xA1, 0x00);

    /* Install the IRQs - the appropriate ISRs are connected to the correct entries in the IDT */
    set_idt_gate(32, (uint32_t) IRQ_0);    set_idt_gate(33, (uint32_t) IRQ_1);
    set_idt_gate(34, (uint32_t) IRQ_2);    set_idt_gate(35, (uint32_t) IRQ_3);
    set_idt_gate(36, (uint32_t) IRQ_4);    set_idt_gate(37, (uint32_t) IRQ_5);
    set_idt_gate(38, (uint32_t) IRQ_6);    set_idt_gate(39, (uint32_t) IRQ_7);
    set_idt_gate(40, (uint32_t) IRQ_8);    set_idt_gate(41, (uint32_t) IRQ_9);
    set_idt_gate(42, (uint32_t) IRQ_10);   set_idt_gate(43, (uint32_t) IRQ_11);
    set_idt_gate(44, (uint32_t) IRQ_12);   set_idt_gate(45, (uint32_t) IRQ_13);
    set_idt_gate(46, (uint32_t) IRQ_14);   set_idt_gate(47, (uint32_t) IRQ_15);

    set_idt(); // Load with ASM
}

/* To print the message which defines every IRQ exception */
const char *getExceptionMessage(int exceptionNumber) {
    switch (exceptionNumber) {
        case 0:  return "Division By Zero";
        case 1:  return "Debug";
        case 2:  return "Non Maskable Interrupt";
        case 3:  return "Breakpoint";
        case 4:  return "Into Detected Overflow";
        case 5:  return "Out of Bounds";
        case 6:  return "Invalid Opcode";
        case 7:  return "No Coprocessor";

        case 8:  return "Double Fault";
        case 9:  return "Coprocessor Segment Overrun";
        case 10: return "Bad TSS";
        case 11: return "Segment Not Present";
        case 12: return "Stack Fault";
        case 13: return "General Protection Fault";
        case 14: return "Page Fault";
        case 15: return "Unknown Interrupt";

        case 16: return "Coprocessor Fault";
        case 17: return "Alignment Check";
        case 18: return "Machine Check";

        default:
            return "Reserved";
    }
}

void ISR_handler(reg_t *registers) {
    if (registers->int_no < 32) {
        kernelException(getExceptionMessage(registers->int_no), registers->int_no, registers->ds, registers);
    }
}

/* Implement a custom IRQ handler for the given IRQ */
void registerInterruptHandler(uint8_t irq, isr_t handler) {
    printColor("[-] ", BG_BLACK | FG_GREEN); print("Registering IRQ ");
    printColor(itoa(irq), BG_BLACK | FG_DKGRAY); print(" to handler ");
    printColor(htoa((uint32_t)handler), BG_BLACK | FG_DKGRAY); print("\n\n");

    interrupt_handlers[irq] = handler;
}

/* Clear the custom IRQ handler */
void unregisterInterruptHandler(uint8_t irq) {
    printColor("[-] ", BG_BLACK | FG_RED); print("Unregistering IRQ ");
    printColor(itoa(irq), BG_BLACK | FG_DKGRAY); print("\n\n");

    interrupt_handlers[irq] = 0;
}

void IRQ_handler(reg_t *registers) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */

    if (registers->int_no >= 40) { /* slave */
        writeByteToPort(0xA0, 0x20);
    }
    writeByteToPort(0x20, 0x20); /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[registers->int_no] != 0) {
        isr_t handler = interrupt_handlers[registers->int_no];
        handler(registers);
    }
}

void IRQ_install() {
    /* Enable interruptions */
    __asm__ __volatile__("sti");

    initializeTimer(100);  /* IRQ0: timer PIT */
    initializeKeyboard();  /* IRQ1: keyboard */
    initializeClock();     /* IRQ8: clock RTC */
}

void IRQ_uninstall() {
    /* Enable interruptions */
    __asm__ __volatile__("sti");

    terminateTimer();     /* IRQ0: timer PIT */
    terminateKeyboard();  /* IRQ1: keyboard */
    terminateClock();     /* IRQ8: clock RTC */
}
