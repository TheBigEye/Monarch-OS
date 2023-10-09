#include "sound.h"

#include "../../common/sysutils.h"
#include "../CPU/ports.h"


/** @see https://wiki.osdev.org/PC_Speaker **/

//
// PRIVATE API ROUTINES
//

/**
 * Play sound with the specified frequency.
 *
 * @param frequency The frequency of the sound.
 */
void playSound(uint32_t frequency) {
    int scale;
    if (frequency == 0) {
        writeByteToPort(0x61, readByteFromPort(0x61) & ~3);
        return;
    }
    scale = 1193182 / frequency;
    writeByteToPort(0x43, 0xb6);
    writeByteToPort(0x42, scale & 0xff);
    writeByteToPort(0x42, scale >> 8);
    writeByteToPort(0x61, readByteFromPort(0x61) | 3);
}

/**
 * Stop the currently playing sound.
 */
void stopSound() {
    uint8_t tmp = readByteFromPort(0x61) & 0xFC;
    writeByteToPort(0x61, tmp);
}


//
// PUBLIC API ROUTINES
//

/**
 * Make a beep sound with the specified frequency and duration.
 *
 * @param frequency The frequency of the beep sound.
 * @param duration The duration of the beep sound in milliseconds.
 */
void playBeep(uint32_t frequency, uint32_t duration) {
    if (!frequency) frequency = 950;
    if (!duration) duration = 255;
    playSound(frequency);
    operationSleep(duration);
    stopSound();
}

/**
 * Read a music chord and play their notes.
 *
 * @param chord The chord notes.
 * @param position The starting position of the chord.
 */
void playChord(char chord[], int position) {
    int len = lengthString(chord);  // Length of the chord array
    char notes[256] = "";  // Array to store extracted notes
    int index = 0;  // Index of the notes array

    // Extract notes from the chord starting at the given position
    for (int i = position; i < len; i++) {
        notes[index] = chord[i];  // Copy the note to the notes array
        index++;
    }
    notes[index] = '\0';  // Null-terminate the notes array

    int notesLen = lengthString(notes);  // Length of the extracted notes
    int frequency = 1;  // Default frequency value

    // Iterate over the extracted notes
    for (int i = 0; i < notesLen - 1; i++) {
        if (notes[i] != ',') {  // Check if the character is not a comma
            if ((notes[i + 1] != ',') && (i + 1 < notesLen - 1)) {
                // Check if the next character is also not a comma and not the last note
                frequency = notes[i] - '0';  // Convert the note character to integer frequency
                playBeep(frequency, (200 - i));  // Play the beep with adjusted duration
                i++;  // Skip the next character
            } else {
                frequency = notes[i] - '0';  // Convert the note character to integer frequency
                playBeep(frequency, 200);  // Play the beep with default duration
            }
        }
    }
}

/**
 * Play the startup sound.
 */
void startupSound() {
    // Chord -> C D G+C D G+F+E+
    // Windows XP startup
    /*operationSleep(50);
    playBeep(523, 200); // C5
    playBeep(587, 190); // D5
    playBeep(392, 180); // G4
    playBeep(1047, 180); // C6
    playBeep(1047, 190); // C6
    playBeep(1047, 200); // C6
    playBeep(1047, 210); // C6
    playBeep(1175, 220); // D6
    playBeep(1397, 230); // F6
    playBeep(1319, 240); // E6
    playBeep(1319, 250); // E6
    playBeep(988, 400); // B5
    operationSleep(50);*/

    // Windows NT 4.0 Startup Sound
    /*operationSleep(50);
    playBeep(523, 200);  // C5
    playBeep(587, 210);  // D5
    playBeep(659, 200);  // E5
    playBeep(783, 210);  // G5
    operationSleep(10);
    playBeep(659, 200);  // E5
    playBeep(587, 210);  // D5
    playBeep(523, 200);  // C5
    playBeep(587, 210);  // D5
    operationSleep(10);
    playBeep(659, 200);  // E5
    playBeep(783, 210);  // G5
    playBeep(659, 200);  // E5
    playBeep(587, 210);  // D5
    operationSleep(10);
    playBeep(523, 200);  // C5
    playBeep(440, 400);  // A4
    operationSleep(400);*/

    operationSleep(300);
    playBeep(262, 300);  // C4
    playBeep(294, 150);  // D4
    playBeep(330, 150);  // E4
    operationSleep(10);
    playBeep(440, 150);  // A4
    playBeep(494, 150);  // B4
    playBeep(523, 300);  // C5
    operationSleep(50);
}

/**
 * Play the shutdown sound.
 */
void shutdownSound() {
    // Windows XP Shutdown Sound
    operationSleep(50);
    playBeep(523, 300);  // C5
    playBeep(494, 150);  // B4
    playBeep(440, 150);  // A4
    operationSleep(10);
    playBeep(330, 150);  // E4
    playBeep(294, 150);  // D4
    playBeep(262, 300);  // C4
    operationSleep(300);
}
