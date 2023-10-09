#include "poem.h"

#include "CPU/ports.h"
#include "drivers/screen.h"

void initializePoem() {
    clearScreen(BG_BLACK | FG_YELLOW);
    setCursorShape(0x3F);
    operationSleep(256);

    putString("- THE BOOT POEM -", 36, 16, BG_BLACK | FG_LTGREEN);
    putString("En el sector de arranque, mi codigo se despierta,", 20, 19, BG_BLACK | FG_WHITE);
    putString("ceros y unos bailan, en una danza eterna.", 25, 20, BG_BLACK | FG_WHITE);
    putString("En el BIOS te encuentro, en la memoria te cargo,", 21, 22, BG_BLACK | FG_WHITE);
    putString("eres mi primer paso, en este viaje largo.", 25, 23, BG_BLACK | FG_WHITE);
    putString("Con tus bytes y bits, este mundo cobra vida,", 24, 25, BG_BLACK | FG_WHITE);
    putString("en este rincon binario, mi alma esta contenida", 22, 26, BG_BLACK | FG_WHITE);
    putString("- with & Nahuel Senek", 33, 29, BG_BLACK | FG_LTGRAY);
    putCharacter((char) 3, 40, 29, BG_BLACK | FG_LTRED);

    operationSleep(12000);
    clearScreen(BG_BLACK | FG_WHITE);
    setCursorShape(0x00);
}
