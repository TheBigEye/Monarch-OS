#include "kernel.h"

#include "../common/cstring.h"

#include "CPU/clock.h"
#include "CPU/ISR.h"
#include "CPU/timer.h"
#include "CPU/ports.h"

#include "drivers/screen.h"
#include "drivers/sound.h"

#include "memory/memory.h"
#include "power/power.h"

#include "bugfault.h"

extern char butterfly[];

void kernel_main() {

    VGA_install();

    clearScreen(BG_BLACK | FG_WHITE);
    setCursorShape(0x3F);
    print("[i] Installing and initializing extended text mode (VGA) ...\n");
    print("[i] Installing interrupt service routines (ISRs) ...\n"); ISR_install();
    print("[i] Installing interrupt requests service routines (IRQs) ...\n"); IRQ_install();

    clearScreen(BG_BLACK | FG_YELLOW);
    sleepTimer(156);


    clearScreen(BG_BLACK | FG_WHITE);
    sleepTimer(156);

    putString(butterfly, 0, 1, BG_BLACK | FG_DKGRAY);

    for (uint8_t i = 0; i < 10; i++) {
        putString("Checking memory pages [*]", 32, 35, BG_BLACK | FG_YELLOW); sleepTimer(90);
        putString("Checking memory pages [ ]", 32, 35, BG_BLACK | FG_YELLOW); sleepTimer(100);
        putString("Checking memory pages [&]", 32, 35, BG_BLACK | FG_YELLOW); sleepTimer(90);
        putString("Checking memory pages [ ]", 32, 35, BG_BLACK | FG_YELLOW); sleepTimer(100);
    }
    putString("                         ", 32, 35, BG_BLACK | FG_WHITE);
    putString("", 0, 35, BG_BLACK | FG_WHITE);
    startupSound();

    putColor('*', 20, 1, 68, 42, BG_BLACK | FG_BROWN);
    putColor('#', 20, 1, 68, 42, BG_BLACK | FG_BROWN);
    putColor('%', 20, 1, 68, 42, BG_BLACK | FG_BROWN);
    putColor('+', 20, 1, 68, 42, BG_BLACK | FG_DKGRAY);
    putColor('=', 20, 1, 68, 42, BG_BLACK | FG_YELLOW);
    putColor('-', 20, 1, 68, 42, BG_BLACK | FG_YELLOW);

    sleepTimer(512);
    putString(" Monarch OS - Under development ", 29, 35, BG_BLACK | FG_DKGRAY); sleepTimer(50);
    putString("   (C) 2022-2023 TheBigEye :)   ", 29, 36, BG_BLACK | FG_DKGRAY); sleepTimer(50);

    sleepTimer(512);
    print("\n\n\n");
    setCursorShape(0x00);

    printColor("/Monarch OS/[@] ", BG_BLACK | FG_GREEN);
}

void user_input(char *input) {
    if (strcmp(input, "EXIT") == 0) {
        printColor("[i] ", BG_BLACK | FG_BROWN); print("The CPU has HALTED NOW! ");
        shutdownSound();
        __asm__ __volatile__("hlt");

    } else if (strcmp(input, "SHUTDOWN") == 0) {
        printColor("[i] ", BG_BLACK | FG_BROWN); print("Shutting down the system ...");
        shutdownSound();
        powerShutdown(1000);

    } else if (strcmp(input, "REBOOT") == 0) {
        printColor("[i] ", BG_BLACK | FG_BROWN); print("Rebooting the system ...");
        powerReboot(2000);

    } else if (strcmp(input, "CLS") == 0 || strcmp(input, "CLEAR") == 0) {
        clearScreen(BG_BLACK | FG_WHITE);

    } else if (strcmp(input, "PAGE") == 0) {
        /* Code to test memoryAllocate(), the rest is unchanged */
        for (uint8_t i = 0; i < 7; i++) {
            uint32_t physical_address;
            uint32_t memory_page = memoryAllocate(1000, 1, &physical_address);

            printColor("[*] ", BG_BLACK | FG_MAGENTA); print("Memory page: ");
            printColor(htoa(memory_page), BG_BLACK | FG_DKGRAY);
            printColor("\n[&] ", BG_BLACK | FG_LTMAGENTA); print("Physical address: ");
            printColor(htoa(physical_address), BG_BLACK | FG_DKGRAY);
            sleepTimer(512);
            print("\n\n");
        }

    } else if (strcmp(input, "TIME") == 0) {
        time_t current_time;
        getTime(&current_time);

        printColor("[o] ", BG_BLACK | FG_YELLOW); print("Today is ");
        const char* days_of_week[] = {
            "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
        };

        printColor(days_of_week[current_time.day_of_week - 1], BG_BLACK | FG_DKGRAY);
        print("\n");
        printColor("[o] ", BG_BLACK | FG_YELLOW); print("date is ");

        printColor(itoa(current_time.day_of_month), BG_BLACK | FG_DKGRAY);

        printColor("/", BG_BLACK | FG_LTGRAY);
        printColor(itoa(current_time.month), BG_BLACK | FG_DKGRAY);

        printColor("/", BG_BLACK | FG_LTGRAY);
        printColor(itoa(current_time.year), BG_BLACK | FG_DKGRAY);
        print("\n");

        printColor("[o] ", BG_BLACK | FG_YELLOW); print("time is ");
        printColor(itoa(current_time.hour), BG_BLACK | FG_DKGRAY);

        printColor(":", BG_BLACK | FG_LTGRAY);
        printColor(itoa(current_time.minute), BG_BLACK | FG_DKGRAY);

        printColor(":", BG_BLACK | FG_LTGRAY);
        printColor(itoa(current_time.second), BG_BLACK | FG_DKGRAY);
        print("\n");

    } else if (strcmp(input, "BUG") == 0) {
        printColor("[!] *** RAISING CONTROLLED EXCEPTION - 00 ***", BG_BLACK | FG_LTRED);
        sleepTimer(1000);
        print("\n\n");
        printColor("/Monarch OS/[@] ", BG_BLACK | FG_RED);
        sleepTimer(500);
        kernelException("Controlled Kernel Exception", 0, 0xDEADDEAD);

    } else if (stringStartsWith("SUM", input) || stringStartsWith("MUL", input)) {
        int result;
        if (stringStartsWith("SUM", input)) {
            result = sum_decode(input, 4);
            printColor("[o]", BG_BLACK | FG_YELLOW); print(" SUM result = ");
        } else {
            result = mul_decode(input, 4);
            printColor("[o]", BG_BLACK | FG_YELLOW); print(" MUL result = ");
        }
        printColor(itoa(result), BG_BLACK | FG_DKGRAY);
        print("\n");

    } else if (stringStartsWith("BEEP", input)) {
        playChord(input, 5);

    } else if (strcmp(input, "TIMER") == 0) {
        printColor("[o]", BG_BLACK | FG_YELLOW); print(" Internal: ");
        printColor(itoa(getElapsedTimer()), BG_BLACK | FG_DKGRAY);
        print("\n");

        printColor("[o]", BG_BLACK | FG_YELLOW); print(" Elapsed: ");
        printColor(itoa(getTimerHours()), BG_BLACK | FG_DKGRAY);
        printColor(":", BG_BLACK | FG_LTGRAY);
        printColor(itoa(getTimerMinutes()), BG_BLACK | FG_DKGRAY);
        printColor(":", BG_BLACK | FG_LTGRAY);
        printColor(itoa(getTimerSeconds()), BG_BLACK | FG_DKGRAY);
        print("\n");

    } else if (strcmp(input, "INFO") == 0) {
        char *build_date = __TIMESTAMP__;
        char *gcc_version = __VERSION__;
        printColor("[o] ", BG_BLACK | FG_YELLOW); print("Build at "); printColor(build_date, BG_BLACK | FG_DKGRAY); print("\n");
        printColor("[o] ", BG_BLACK | FG_YELLOW); print("GCC version "); printColor(gcc_version, BG_BLACK | FG_DKGRAY); print("\n\n");
        printColor("[o] ", BG_BLACK | FG_YELLOW); print("Created by "); printColor("TheBigEye\n", BG_BLACK | FG_DKGRAY);
        printColor("[o] ", BG_BLACK | FG_YELLOW); print("Dedicated to:\n");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  My brother,\n", BG_BLACK | FG_CYAN);
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  And my friends :D\n", BG_BLACK | FG_BROWN);
    } else if (strcmp(input, "CHARS") == 0) {
        printSupportedChars();
        print("\n");

    } else {
        if (strlen(input) != 0) { // We dont validate empty buffers
            playBeep(600, 100);
            PANIC("Invalid kernel-mode order or command!");
        }
    }

    print("\n");
    printColor("/Monarch OS/[@] ", BG_BLACK | FG_GREEN);
}
