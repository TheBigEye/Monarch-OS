#ifndef _DRIVER_SOUND_H
#define _DRIVER_SOUND_H 1

#include <stdint.h>

/**
 * Make a beep sound with the specified frequency and duration.
 *
 * @param frequency The frequency of the beep sound.
 * @param duration The duration of the beep sound in milliseconds.
 */
void playBeep(uint32_t frequency, uint32_t duration);

/**
 * Read a music chord and play their notes.
 *
 * @param chord The chord notes.
 * @param position The starting position of the chord.
 */
void playChord(char chord[], int position);

/**
 * Play the startup sound.
 */
void startupSound();

/**
 * Play the shutdown sound.
 */
void shutdownSound();

#endif /* _DRIVER_SOUND_H */
