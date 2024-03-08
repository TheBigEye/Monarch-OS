#include "poem.h"

#include "CPU/PIT/timer.h"
#include "drivers/screen.h"

void initializePoem(void) {
    clearScreen(BG_BLACK | FG_YELLOW);
    setCursor(0x3F);
    timerSleep(10);

    putString("- THE BOOT POEM -",                                  30, 6, BG_BLACK | FG_LTGREEN);

    putString("En el sector de arranque, mi codigo se despierta,",  15, 8, BG_BLACK | FG_WHITE);
    putString("ceros y unos bailan, en una danza eterna.",          18, 9, BG_BLACK | FG_WHITE);

    putString("En el BIOS te encuentro, en la memoria te cargo,",   15, 11, BG_BLACK | FG_WHITE);
    putString("eres mi primer paso, en este viaje largo.",          18, 12, BG_BLACK | FG_WHITE);

    putString("Con tus bytes y bits, este mundo cobra vida,",       17, 14, BG_BLACK | FG_WHITE);
    putString("en este rincon binario, mi alma esta contenida",     16, 15, BG_BLACK | FG_WHITE);

    putString("- with # Nahuel Senek",                              27, 17, BG_BLACK | FG_LTGRAY);
    putCharacter((char) 3, 34, 17, BG_BLACK | FG_LTRED);

    timerSleep(1500);
    clearScreen(BG_BLACK | FG_WHITE);
    setCursor(0x3F);
}
