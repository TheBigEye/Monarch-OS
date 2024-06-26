#include "ISR.h"

#include "../../drivers/console.h"
#include "../../drivers/keyboard.h"

#include "../../bugfault.h"

#include "../FPU/FPU.h"
#include "../IDT/IDT.h"
#include "../PIT/timer.h"
#include "../RTC/clock.h"
#include "../HAL.h"


/*
* ISR - Interrupt Service Routines, code routines that are executed in response to an interrupt
* signal, handling specific events or tasks triggered by hardware or software interrupts
*
* @see https://wiki.osdev.org/ISR#Interrupt-Service-Routines
*/

interrupt_t interrupt_handlers[256];

/* Can't do this with a loop because we need the address of the function names */

void isrDoInstall() {

    // TODO: actually we *can* do this with a loop if you used a macro to generate all
    // the stubs, because you can also generate the addresses while you're at it

    /* Set the first 32 entries in the IDT to the first 32 ISRs */
    idtSetGate(0,  (uint32_t) ISR_0);    idtSetGate(1,  (uint32_t) ISR_1);
    idtSetGate(2,  (uint32_t) ISR_2);    idtSetGate(3,  (uint32_t) ISR_3);
    idtSetGate(4,  (uint32_t) ISR_4);    idtSetGate(5,  (uint32_t) ISR_5);
    idtSetGate(6,  (uint32_t) ISR_6);    idtSetGate(7,  (uint32_t) ISR_7);
    idtSetGate(8,  (uint32_t) ISR_8);    idtSetGate(9,  (uint32_t) ISR_9);
    idtSetGate(10, (uint32_t) ISR_10);   idtSetGate(11, (uint32_t) ISR_11);
    idtSetGate(12, (uint32_t) ISR_12);   idtSetGate(13, (uint32_t) ISR_13);
    idtSetGate(14, (uint32_t) ISR_14);   idtSetGate(15, (uint32_t) ISR_15);
    idtSetGate(16, (uint32_t) ISR_16);   idtSetGate(17, (uint32_t) ISR_17);
    idtSetGate(18, (uint32_t) ISR_18);   idtSetGate(19, (uint32_t) ISR_19);
    idtSetGate(20, (uint32_t) ISR_20);   idtSetGate(21, (uint32_t) ISR_21);
    idtSetGate(22, (uint32_t) ISR_22);   idtSetGate(23, (uint32_t) ISR_23);
    idtSetGate(24, (uint32_t) ISR_24);   idtSetGate(25, (uint32_t) ISR_25);
    idtSetGate(26, (uint32_t) ISR_26);   idtSetGate(27, (uint32_t) ISR_27);
    idtSetGate(28, (uint32_t) ISR_28);   idtSetGate(29, (uint32_t) ISR_29);
    idtSetGate(30, (uint32_t) ISR_30);   idtSetGate(31, (uint32_t) ISR_31);

    /* Remap the PIC - IRQ0 to IRQ15 have to be remapped to IDT entries 32 to 47 */

    // starts the initialization sequence
    writeByteToPort(0x20, 0x11); writeByteToPort(0xA0, 0x11);

    // Define the PIC vectors
    writeByteToPort(0x21, 0x20); // Set offset of Master PIC to 0x20 (32): Entry 32-39
    writeByteToPort(0xA1, 0x28); // Set offset of Slave  PIC to 0x28 (40): Entry 40-47

    // Continue initialization sequence
    writeByteToPort(0x21, 0x04); writeByteToPort(0xA1, 0x02);
    writeByteToPort(0x21, 0x01); writeByteToPort(0xA1, 0x01);
    writeByteToPort(0x21, 0x00); writeByteToPort(0xA1, 0x00);

    /* Install the IRQs - the appropriate ISRs are connected to the correct entries in the IDT */
    idtSetGate(32, (uint32_t) IRQ_0);    idtSetGate(33, (uint32_t) IRQ_1);
    idtSetGate(34, (uint32_t) IRQ_2);    idtSetGate(35, (uint32_t) IRQ_3);
    idtSetGate(36, (uint32_t) IRQ_4);    idtSetGate(37, (uint32_t) IRQ_5);
    idtSetGate(38, (uint32_t) IRQ_6);    idtSetGate(39, (uint32_t) IRQ_7);
    idtSetGate(40, (uint32_t) IRQ_8);    idtSetGate(41, (uint32_t) IRQ_9);
    idtSetGate(42, (uint32_t) IRQ_10);   idtSetGate(43, (uint32_t) IRQ_11);
    idtSetGate(44, (uint32_t) IRQ_12);   idtSetGate(45, (uint32_t) IRQ_13);
    idtSetGate(46, (uint32_t) IRQ_14);   idtSetGate(47, (uint32_t) IRQ_15);

    idtDoInstall(); // Load with ASM

    // Wait some milli-seconds
    operationSleep(2);
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

void ISR_handler(registers_t *registers) {
    if (registers->int_no < 32) {
        kernelException(getExceptionMessage(registers->int_no), registers->int_no, registers->ds, registers);
    }
}

/* Implement a custom IRQ handler for the given IRQ */
void registerInterruptHandler(uint8_t irq, interrupt_t handler) {
    printOutput("[...] ", BG_BLACK | FG_GREEN, "Registering IRQ %d to handler %p\n\n", irq, handler);
    interrupt_handlers[irq] = handler;
}

/* Clear the custom IRQ handler */
void unregisterInterruptHandler(uint8_t irq) {
    printOutput("[...] ", BG_BLACK | FG_RED, "Unregistering IRQ %d\n\n", irq);
    interrupt_handlers[irq] = 0;
}

void IRQ_handler(registers_t *registers) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */

    if (registers->int_no >= 40) { /* slave */
        writeByteToPort(0xA0, 0x20);
    }
    writeByteToPort(0x20, 0x20); /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[registers->int_no] != 0) {
        interrupt_t handler = interrupt_handlers[registers->int_no];
        handler(registers);
    }
}

void irqDoInstall() {
    /* Enable interruptions */
    __asm__ __volatile__ ("sti");

    initializeTimer(100);     /* IRQ0: timer PIT */
    operationSleep(2);        // Wait some milli-seconds

    initializeKeyboard();     /* IRQ1: keyboard */
    operationSleep(2);        // Wait some milli-seconds

    initializeClock();        /* IRQ8: clock RTC */
    operationSleep(2);        // Wait some milli-seconds

    initializeCoprocessor();  /* IRQ13: Float Point Unit */
    operationSleep(2);        // Wait some milli-seconds
}

void IRQ_uninstall() {
    /* Enable interruptions */
    __asm__ __volatile__ ("sti");

    terminateTimer();        /* IRQ0: timer PIT */
    operationSleep(2);       // Wait some milli-seconds

    terminateKeyboard();     /* IRQ1: keyboard */
    operationSleep(2);       // Wait some milli-seconds

    terminateClock();        /* IRQ8: clock RTC */
    operationSleep(2);       // Wait some milli-seconds

    terminateCoprocessor();  /* IRQ13: Float Point Unit */
    operationSleep(2);       // Wait some milli-seconds
}
