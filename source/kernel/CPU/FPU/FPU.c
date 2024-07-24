#include "FPU.h"

#include "../ISR/ISR.h"
#include "../BIOS.h"
#include "../HAL.h"

#include "../../../common/sysutils.h"
#include "../../drivers/console.h"
#include "../../memory/memory.h"

static inline void setControlWord(const uint16_t cw) {
    __asm__ __volatile__ ("fldcw %0" ::"m"(cw));
}

/**
 * Timer interrupt callback function.
 *
 * @param regs  The interrupt's caller registers.
 */
static void coprocessorCallback(registers_t *regs) {
    uint32_t cr4;
    __asm__ __volatile__ ("mov %%cr4, %0" :"=r"(cr4));
    // set 9th bit to 1 in cr4
    cr4 |= 0x200;
    __asm__ __volatile__ ("mov %0, %%cr4" ::"r"(cr4));
    setControlWord(0x37F); // fpu init
    setControlWord(0x37E); // invalid operand exceptions
    setControlWord(0x37A); // divide by zero

    UNUSED(regs);
}


void initializeCoprocessor(void) {
    ttyPrintOut(INIT, "Initializing FPU handler at IRQ13 ...\n");
    registerInterruptHandler(IRQ13, coprocessorCallback);
}
