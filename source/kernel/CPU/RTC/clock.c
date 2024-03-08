#include "clock.h"

#include "../../../common/sysutils.h"
#include "../../drivers/screen.h"
#include "../../memory/memory.h"

#include "../CMOS/CMOS.h"
#include "../ISR/ISR.h"
#include "../ports.h"

/*
* RTC - Real Time Clock, keeps track of current time and date even when the
* system is powered off.
*
* @see https://wiki.osdev.org/CMOS#Getting-Current-Date-and-Time-from-RTC
*/

/* The current time stored in the Real Time Clock */
time_t global_time;

/* Flag indicating whether the RTC data type is BCD (Binary Coded Decimal) */
int32_t bcd;

/*
  The function reads the current time from the Real Time Clock
  to the time_t parameter.

  Input:
    time - A pointer to the time object to read the time to.
  Output:
    None.
*/
void getTime(time_t *time) {
    memoryCopy((uint8_t*)&global_time, (uint8_t*)time, sizeof(time_t));
}

/**
 * Handler for the Real Time Clock interrupt.
 * Updates the global_time variable based on the clock.
 *
 * @param regs  The registers of the interrupt caller
 */
static void clockCallback(reg_t *regs) {
    if (readRegisterValue(0x0C) & 0x10) {
        if (bcd) {
            global_time.second = getBCD(readRegisterValue(0x00));
            global_time.minute = getBCD(readRegisterValue(0x02));
            global_time.hour   = getBCD(readRegisterValue(0x04));
            global_time.day_of_week  = getBCD(readRegisterValue(0x06));
            global_time.day_of_month = getBCD(readRegisterValue(0x07));
            global_time.month  = getBCD(readRegisterValue(0x08));
            global_time.year   = getBCD(readRegisterValue(0x09));
            global_time.century = getBCD(readRegisterValue(0x32)); // yeah, this is real son
        } else {
            global_time.second = readRegisterValue(0x00);
            global_time.minute = readRegisterValue(0x02);
            global_time.hour   = readRegisterValue(0x04);
            global_time.day_of_week  = readRegisterValue(0x06);
            global_time.day_of_month = readRegisterValue(0x07);
            global_time.month  = readRegisterValue(0x08);
            global_time.year   = readRegisterValue(0x09);
            global_time.century = readRegisterValue(0x32);
        }
    }
    UNUSED(regs);
}

/**
 * Initialize the Real Time Clock chip and add its handler to the IDT.
 */
void initializeClock(void) {
    uint8_t status;

    status = readRegisterValue(0x0B);
    status |= 0x02;             // 24 hour clock
    status |= 0x10;             // update ended interrupts
    status &= (uint8_t)~0x20;   // no alarm interrupts
    status &= (uint8_t)~0x40;   // no periodic interrupt
    bcd = !(status & 0x04);     // check if data type is BCD

    writeRegisterValue(0x0B, status);
    readRegisterValue(0x0C);

    printColor("[-] ", BG_BLACK | FG_LTGREEN); printString("Initializing RTC handler at IRQ8 ...\n");
    registerInterruptHandler(IRQ8, clockCallback);
}

void terminateClock(void) {
    printColor("[-] ", BG_BLACK | FG_LTRED); printString("Terminating and cleaning RTC handler at IRQ8 ...\n");
    unregisterInterruptHandler(IRQ8);
}
