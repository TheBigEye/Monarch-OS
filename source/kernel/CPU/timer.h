#ifndef _CPU_TIMER_H
#define _CPU_TIMER_H 1

#include <stdint.h>

/**
 * Initializes the timer with the specified frequency.
 *
 * @param frequency  The frequency at which the timer should generate interrupts.
 */
void initTimer(uint32_t frequency);

/**
 * Sleeps for the specified number of milliseconds.
 *
 * @param milliseconds  The number of milliseconds to sleep for.
 */
void sleepTimer(uint32_t milliseconds);


uint32_t getElapsedTimer();

/**
 * Get the elapsed seconds based on the PIT tick count.
 *
 * @attention The returned value lacks precision and depends solely on the hardware speed.
 * It should not be used for exact measurements. For accurate timekeeping, consider using
 * the real-time clock (RTC).
 *
 * @return The elapsed PIT seconds.
 */
uint32_t getTimerSeconds();

/**
 * Get the elapsed minutes based on the PIT tick count.
 *
 * @attention The returned value lacks precision and depends solely on the hardware speed.
 * It should not be used for exact measurements. For accurate timekeeping, consider using
 * the real-time clock (RTC).
 *
 * @return The elapsed PIT minutes.
 */
uint32_t getTimerMinutes();

/**
 * Get the elapsed hours based on the PIT tick count.
 *
 * @attention The returned value lacks precision and depends solely on the hardware speed.
 * It should not be used for exact measurements. For accurate timekeeping, consider using
 * the real-time clock (RTC).
 *
 * @return The elapsed PIT hours.
 */
uint32_t getTimerHours();

#endif /* _CPU_TIMER_H */
