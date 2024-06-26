#ifndef _CPU_TIMER_H
#define _CPU_TIMER_H 1

#include "../../../common/sysutils.h"

uint32_t timerGetTicks(void);

/**
 * Get the elapsed seconds based on the PIT tick count.
 *
 * @attention The returned value lacks precision and depends solely on the hardware speed.
 * It should not be used for exact measurements. For accurate timekeeping, consider using
 * the real-time clock (RTC).
 *
 * @return The elapsed PIT seconds.
 */
uint32_t timerGetSeconds(void);

/**
 * Get the elapsed minutes based on the PIT tick count.
 *
 * @attention The returned value lacks precision and depends solely on the hardware speed.
 * It should not be used for exact measurements. For accurate timekeeping, consider using
 * the real-time clock (RTC).
 *
 * @return The elapsed PIT minutes.
 */
uint32_t timerGetMinutes(void);

/**
 * Get the elapsed hours based on the PIT tick count.
 *
 * @attention The returned value lacks precision and depends solely on the hardware speed.
 * It should not be used for exact measurements. For accurate timekeeping, consider using
 * the real-time clock (RTC).
 *
 * @return The elapsed PIT hours.
 */
uint32_t timerGetHours(void);


/**
 * Initializes the timer with the specified frequency.
 *
 * @param frequency  The frequency at which the timer should generate interrupts.
 */
void initializeTimer(uint32_t frequency);

/**
 * Terminate the PIT and unregister it handler from the IRQ
*/
void terminateTimer(void);

/**
 * Sleep an operation for shot time.
 *
 * @param time  The time to sleep in milliseconds.
 */
void timerSleep(uint32_t time);


#endif /* _CPU_TIMER_H */
