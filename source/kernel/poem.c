#include "poem.h"

#include "CPU/PIT/timer.h"
#include "drivers/console.h"

void initializeMyPoem() {
    clearScreen();
    setCursor(0x3F);
    timerSleep(2);

    putString("- THE BOOT POEM -",                                  36, 16, BG_BLACK | FG_LTGREEN);

    putString("En el sector de arranque, mi codigo se despierta,",  20, 18, BG_BLACK | FG_WHITE);
    putString("ceros y unos bailan, en una danza eterna.",          25, 19, BG_BLACK | FG_WHITE);

    putString("En el BIOS te encuentro, en la memoria te cargo,",   21, 21, BG_BLACK | FG_WHITE);
    putString("eres mi primer paso, en este viaje largo.",          25, 22, BG_BLACK | FG_WHITE);

    putString("Con tus bytes y bits, este mundo cobra vida,",       24, 24, BG_BLACK | FG_WHITE);
    putString("en este rincon binario, mi alma esta contenida",     22, 25, BG_BLACK | FG_WHITE);

    putString("- with # Nahuel Senek",                              33, 27, BG_BLACK | FG_LTGRAY);
    putCharacter((char) 3, 40, 27, BG_BLACK | FG_LTRED);

    timerSleep(1500);
    clearScreen();
    setCursor(0x3F);
}
