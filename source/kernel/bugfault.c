#include "bugfault.h"

#include "../common/cstring.h"

#include "CPU/timer.h"
#include "drivers/screen.h"
#include "drivers/sound.h"
#include "power/power.h"

extern char butterfly[];

/**
 * Handle a kernel driver exception.
 *
 * @param reason The exception reason message or ISR error code.
 * @param interrupt The IRQ value that triggered the exception.
 * @param regpointer The memory pointer to the ISR register.
 */
void kernelException(const char *reason, uint32_t interrupt, uint32_t regpointer) {

    clearScreen(BG_BLACK | FG_WHITE);
    sleepTimer(50);

    putString(butterfly, 0, 1, BG_BLACK | FG_DKGRAY);

    putColor('*', 20, 1, 68, 42, BG_BLACK | FG_LTRED);
    putColor('#', 20, 1, 68, 42, BG_BLACK | FG_LTRED);
    putColor('%', 20, 1, 68, 42, BG_BLACK | FG_LTRED);
    putColor('+', 20, 1, 68, 42, BG_BLACK | FG_RED);
    putColor('=', 20, 1, 68, 42, BG_BLACK | FG_RED);
    putColor('-', 20, 1, 68, 42, BG_BLACK | FG_RED);

    char title[] = " Monarch OS ";
    char msg_line_1[] = "A 32-bit device driver has corrupted critical system memory, resulting";
    char msg_line_2[] = "in an exception 00 at &:           (*reg). The system has stopped now.";
    char msg_line_3[] = "* The current device will automatically restart to";
    char msg_line_4[] = "  try to fix this error.";
    char msg_line_5[] = "* If a solution to the problem is not found, contact";
    char msg_line_6[] = "  your system administrator or technical.";
    char msg_line_7[] = "Rebooting...";

    sleepTimer(666);
    clearScreen(BG_RED | FG_WHITE);
    sleepTimer(50);
    clearScreen(BG_BLACK | FG_WHITE);
    setCursorShape(0x3F);

    putString(title, 38, 20, BG_LTGRAY | FG_BLACK);
    putString(msg_line_1, 10, 23, BG_BLACK | FG_WHITE);
    putString(msg_line_2, 10, 24, BG_BLACK | FG_WHITE);
    putString(itoa(interrupt), 26, 24, BG_BLACK | FG_WHITE);
    putString(htoa(regpointer), 33, 24, BG_BLACK | FG_WHITE);
    putString(reason, 11, 26, BG_BLACK | FG_WHITE);
    putString(msg_line_3, 10, 28, BG_BLACK | FG_WHITE);
    putString(msg_line_4, 10, 29, BG_BLACK | FG_WHITE);
    putString(msg_line_5, 10, 30, BG_BLACK | FG_WHITE);
    putString(msg_line_6, 10, 31, BG_BLACK | FG_WHITE);

    playBeep(100, 3000);
    sleepTimer(9000);
    putString(msg_line_7, 38, 34, BG_BLACK | FG_DKGRAY);
    sleepTimer(1900);
    clearScreen(BG_BLACK | FG_WHITE);
    powerReboot(100);
}

/**
 * Trigger a kernel panic.
 *
 * @param message The panic message.
 * @param file The file where the panic occurred.
 * @param line The line number where the panic occurred.
 */
void kernelPanic(const char *message, const char *file, uint32_t line) {
    // :(
    printColor("[!] ", BG_BLACK | FG_BROWN);
    printColor(message, BG_BLACK | FG_LTGRAY);
    printColor(" [@", BG_BLACK | FG_BROWN);
    printColor(file, BG_BLACK | FG_BROWN);
    printColor(":", BG_BLACK | FG_BROWN);
    printColor(itoa(line), BG_BLACK | FG_BROWN);
    printColor("]\n", BG_BLACK | FG_BROWN);
}

/**
 * Handle a kernel assertion failure.
 *
 * @param file The file where the assertion failed.
 * @param line The line number where the assertion failed.
 * @param desc Description of the assertion failure.
 */
void kernelAssert(const char *file, uint32_t line, const char *desc) {
    // Assertion failure
    __asm__ __volatile__("cli");

    print("ASS. FAILURE.. ;): ");
    print(desc);
    print(" @");
    print(file);
    print(":");
    print(itoa(line));
    print("\n");
    for(;;);
}
