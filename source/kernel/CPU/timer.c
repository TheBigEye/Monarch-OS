#include "timer.h"

#include "../../common/ctypes.h"

#include "ISR.h"
#include "ports.h"

/*
* PIT - Programmable Interval Timer. The PIT is like a stopwatch with periodic alarm
*
* @see https://wiki.osdev.org/PIT#Programmable-Interval-Timer
*/

uint32_t tick = 0;

/**
 * Timer interrupt callback function.
 *
 * @param regs  The interrupt's caller registers.
 */
static void timerCallback(reg_t *regs) {
    tick++;
    UNUSED(regs);
}

/**
 * Initializes the timer with the specified frequency.
 *
 * @param frequency  The frequency at which the timer should generate interrupts.
 */
void initTimer(uint32_t frequency) {
    // Get the PIT value: hardware clock at 1193180 Hz
    uint32_t divisor = 1193180 / frequency;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    // Install the timerCallback function
    registerInterruptHandler(IRQ0, timerCallback);

    // Send the command
    writeByteToPort(0x43, 0x36); // Command port
    writeByteToPort(0x40, low);
    writeByteToPort(0x40, high);
}

/**
 * Sleeps for the specified number of milliseconds.
 *
 * @param milliseconds  The number of milliseconds to sleep for.
 */
void sleepTimer(uint32_t milliseconds) {
    // Calculate the number of iterations needed to sleep for the specified number of milliseconds
    uint32_t iterations = milliseconds * 1000;

    // Sleep for the specified number of iterations by executing a busy-wait loop
    for (uint32_t i = 0; i < iterations; i++) {
        __asm__ __volatile__("outb %%al, $0x80" : : "a"(0));
    }
}

uint32_t getElapsedTimer() {
    return tick;
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
uint32_t getTimerSeconds() {
    return (tick / 100) % 60;
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
uint32_t getTimerMinutes() {
    return (tick / (100 * 60)) % 60;
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
uint32_t getTimerHours() {
    return tick / (100 * 60 * 60);
}
