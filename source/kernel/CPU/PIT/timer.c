#include "timer.h"

#include "../../../common/sysutils.h"
#include "../../drivers/console.h"

#include "../ISR/ISR.h"
#include "../HAL.h"

/*
* PIT - Programmable Interval Timer. The PIT is like a stopwatch with periodic alarm
*
* @see https://wiki.osdev.org/PIT#Programmable-Interval-Timer
*/

// FIX FIX THIS SHIT!: this will overflow after about an hour of runtime, use uint64_t and either link against libgcc or write a 64bit os
volatile uint32_t timerTicks;

uint32_t timerGetTicks() {
    return timerTicks;
}

/**
 * Get the elapsed seconds based on the PIT tick count.
 *
 * @attention The returned value lacks precision and depends solely on the hardware speed.
 * It should not be used for exact measurements. For accurate timekeeping, consider using
 * the real-time clock (RTC).
 *
 * @return The elapsed PIT seconds.
 */
uint32_t timerGetSeconds() {
    return (timerTicks / 100) % 60;
}

/**
 * Get the elapsed minutes based on the PIT tick count.
 *
 * @attention The returned value lacks precision and depends solely on the hardware speed.
 * It should not be used for exact measurements. For accurate timekeeping, consider using
 * the real-time clock (RTC).
 *
 * @return The elapsed PIT minutes.
 */
uint32_t timerGetMinutes() {
    return (timerTicks / (100 * 60)) % 60;
}

/**
 * Get the elapsed hours based on the PIT tick count.
 *
 * @attention The returned value lacks precision and depends solely on the hardware speed.
 * It should not be used for exact measurements. For accurate timekeeping, consider using
 * the real-time clock (RTC).
 *
 * @return The elapsed PIT hours.
 */
uint32_t timerGetHours() {
    return timerTicks / (100 * 60 * 60);
}

/**
 * Timer interrupt callback function.
 *
 * @param regs  The interrupt's caller registers.
 */
static void timerCallback(registers_t *regs) {
    timerTicks++;
    UNUSED(regs);
}

/**
 * Initializes the timer with the specified frequency.
 *
 * @param frequency  The frequency at which the timer should generate interrupts.
 */
void initializeTimer(uint32_t frequency) {
    timerTicks = 0;

    // Get the PIT value: hardware clock at 1193180 Hz
    uint32_t divisor = 1193180 / frequency;

    writeByteToPort(0x43, 0x36); // Command port

    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    printOutput("[...] ", BG_BLACK | FG_LTGREEN, "Initializing PIT handler at IRQ0 ...\n");

    // Install the timerCallback function
    registerInterruptHandler(IRQ0, timerCallback);

    // Send the command
    writeByteToPort(0x40, low);
    writeByteToPort(0x40, high);
}

void terminateTimer() {
    printOutput("[...] ", BG_BLACK | FG_LTRED, "Terminating and cleaning PIT handler at IRQ0 ...\n");

    // Unnstall the timerCallback function
    unregisterInterruptHandler(IRQ0);
}

/**
 * Sleep an operation for shot time.
 *
 * @param time  The time to sleep in milliseconds.
 */
void timerSleep(uint32_t time) {
    uint32_t target = timerTicks + time;
    while (timerTicks < target) {
        __asm__ __volatile__ ("sti//hlt//cli");
    }
}
