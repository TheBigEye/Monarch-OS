#ifndef _CPU_CLOCK_H
#define _CPU_CLOCK_H 1

#include <stdint.h>

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day_of_week;
    uint8_t day_of_month;
    uint8_t month;
    uint8_t year;
    uint8_t century;
} time_t;

/**
 * Read the current time from the Real Time Clock into the specified time object.
 *
 * @param time  Pointer to the time object to store the current time
 */
void getTime(time_t* time);

/**
 * Initialize the Real Time Clock chip and add its handler to the IDT.
 */
void initClock();

#endif /* _CPU_CLOCK_H */
