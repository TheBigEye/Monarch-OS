#include "FPU.h"

#include "../../../common/sysutils.h"
#include "../../drivers/screen.h"
#include "../../memory/memory.h"

#include "../CMOS/CMOS.h"
#include "../ISR/ISR.h"
#include "../ports.h"

bool coprocessorInstalled = false;

void setControlWord(const uint16_t cw) {
    __asm__ __volatile__ ("fldcw %0" ::"m"(cw));
}

/**
 * Timer interrupt callback function.
 *
 * @param regs  The interrupt's caller registers.
 */
static void coprocessorCallback(reg_t *regs) {
    if (!coprocessorInstalled) {
        uint32_t cr4;
        __asm__ __volatile__ ("mov %%cr4, %0" :"=r"(cr4));
        // set 9th bit to 1 in cr4
        cr4 |= 0x200;
        __asm__ __volatile__ ("mov %0, %%cr4" ::"r"(cr4));
        setControlWord(0x37F); // fpu init
        setControlWord(0x37E); // invalid operand exceptions
        setControlWord(0x37A); // divide by zero
        coprocessorInstalled = true;
    }
    UNUSED(regs);
}


void initializeCoprocessor(void){
    printColor("[-] ", BG_BLACK | FG_LTGREEN); printString("Initializing FPU handler at IRQ13 ...\n");
    registerInterruptHandler(IRQ13, coprocessorCallback);
}

void terminateCoprocessor(void) {
    printColor("[-] ", BG_BLACK | FG_LTRED); printString("Terminating and cleaning FPU handler at IRQ13 ...\n");
    unregisterInterruptHandler(IRQ13);
}
