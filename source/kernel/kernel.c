#include "kernel.h"

#include "../common/sysutils.h"
#include "../common/sysutils.h"


#include "CPU/CMOS/CMOS.h"
#include "CPU//ISR/ISR.h"
#include "CPU//PIT/timer.h"
#include "CPU/RTC/clock.h"
#include "CPU/CPU.h"
#include "CPU/ports.h"

#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "drivers/sound.h"

#include "memory/memory.h"
#include "power/power.h"

#include "bugfault.h"
#include "version.h"

extern char butterfly[];

void kernel_main() {

    VGA_install();

    clearScreen(BG_BLACK | FG_YELLOW);
    operationSleep(256);
    setCursorShape(0x3F);

    /* DEBUG ONLY
    int value=2567;

    printFormat("Decimal value is: %d\n",value);
    printFormat("Octal value is: %o\n",value);
    printFormat("Hexadecimal value is (Alphabet in small letters): %x\n",value);
    printFormat("Hexadecimal value is (Alphabet in capital letters): %X\n",value);
    */

    putString("- THE POEM -", 38, 15, BG_BLACK | FG_LTGREEN);
    putString("A veces pienso en tu mirada,", 30, 18, BG_BLACK | FG_WHITE);
    putString("que me llena de paz y esperanza.", 28, 19, BG_BLACK | FG_WHITE);
    putString("Una vez te dije lo que sentia,", 29, 21, BG_BLACK | FG_WHITE);
    putString("y tu me respondiste con alegria.", 28, 22, BG_BLACK | FG_WHITE);
    putString("Entiendo que tenemos diferencias,", 28, 24, BG_BLACK | FG_WHITE);
    putString("pero eso no cambia mis preferencias.", 26, 25, BG_BLACK | FG_WHITE);
    putString("Te admiro por tu forma de ser,", 29, 27, BG_BLACK | FG_WHITE);
    putString("y por todo lo que puedes hacer.", 28, 28, BG_BLACK | FG_WHITE);
    putString("Te dedico estas palabras sinceras,", 28, 30, BG_BLACK | FG_WHITE);
    putString("y espero que sepas que son verdaderas.", 26, 31, BG_BLACK | FG_WHITE);
    putString("- with   Nahuel Senek", 33, 34, BG_BLACK | FG_LTGRAY);
    putCharacter((char)3, 40, 34, BG_BLACK | FG_LTRED);
    operationSleep(16000);

    clearScreen(BG_BLACK | FG_WHITE);

    putString(" -- INITIALIZING KERNEL --- \n\n\n\n", 31, 2, BG_BLACK | FG_LTGREEN);
    operationSleep(512);
    printColor("[i] ", BG_BLACK | FG_BROWN); print("Installing and initializing extended text mode (VGA) ...\n");       operationSleep(500);
    printColor("[i] ", BG_BLACK | FG_BROWN); print("Installing interrupt service routines (ISRs) ...\n");               ISR_install(); operationSleep(500);
    printColor("[i] ", BG_BLACK | FG_BROWN); print("Installing interrupt requests service routines (IRQs) ...\n\n");    IRQ_install(); operationSleep(500);
    print("\n...");
    operationSleep(1024);

    clearScreen(BG_BLACK | FG_WHITE);
    operationSleep(156);

    putString(butterfly, 0, 1, BG_BLACK | FG_DKGRAY);

    for (uint8_t i = 0; i < 4; i++) {
        putString("Checking memory pages [*]", 32, 35, BG_BLACK | FG_YELLOW); operationSleep(90);
        putString("Checking memory pages [ ]", 32, 35, BG_BLACK | FG_YELLOW); operationSleep(110);
        putString("Checking memory pages [&]", 32, 35, BG_BLACK | FG_YELLOW); operationSleep(90);
        putString("Checking memory pages [ ]", 32, 35, BG_BLACK | FG_YELLOW); operationSleep(110);
    }
    putString("\t\t\t\t\t\t ", 32, 35, BG_BLACK | FG_WHITE);
    putString("", 0, 35, BG_BLACK | FG_WHITE);
    startupSound();

    putColor('*', 20, 1, 68, 42, BG_BLACK | FG_BROWN);
    putColor('#', 20, 1, 68, 42, BG_BLACK | FG_BROWN);
    putColor('%', 20, 1, 68, 42, BG_BLACK | FG_BROWN);
    putColor('+', 20, 1, 68, 42, BG_BLACK | FG_DKGRAY);
    putColor('=', 20, 1, 68, 42, BG_BLACK | FG_YELLOW);
    putColor('-', 20, 1, 68, 42, BG_BLACK | FG_YELLOW);

    operationSleep(256);
    putString(" Monarch OS - Under development ", 29, 35, BG_BLACK | FG_DKGRAY); operationSleep(50);
    putString("   (C) 2022-2023 Nahuel Senek   ", 29, 36, BG_BLACK | FG_DKGRAY); operationSleep(50);

    operationSleep(256);
    print("\n\n\n");
    setCursorShape(0x00);

    printColor("/Monarch OS/[@] ", BG_BLACK | FG_GREEN);
}

// TODO: rewrite this as a Commands Handler
void user_input(char *input) {
    if (equalsWith(input, "EXIT") == 0) {
        printColor("[i] ", BG_BLACK | FG_BROWN); print("The CPU has HALTED NOW! ");
        shutdownSound();
        __asm__ __volatile__("hlt");

    } else if (equalsWith(input, "SHUTDOWN") == 0) {
        printColor("[i] ", BG_BLACK | FG_BROWN); print("Shutting down the system ...");
        shutdownSound();
        powerShutdown(1000);

    } else if (equalsWith(input, "REBOOT") == 0) {
        printColor("[i] ", BG_BLACK | FG_BROWN); print("Rebooting the system ...");
        powerReboot(2000);

    } else if (equalsWith(input, "CLS") == 0 || equalsWith(input, "CLEAR") == 0) {
        clearScreen(BG_BLACK | FG_WHITE);

    } else if (equalsWith(input, "PAGE") == 0) {
        for (uint8_t i = 0; i < 7; i++) {
            uint32_t physical_address;
            uint32_t memory_page = memoryAllocate(1000, 1, &physical_address);

            printColor("[*] ", BG_BLACK | FG_MAGENTA); printFormat("Memory page: %s\n", htoa(memory_page));
            printColor("[&] ", BG_BLACK | FG_LTMAGENTA); printFormat("Physical to: %s\n\n", htoa(physical_address));
            operationSleep(4);
        }

    } else if (equalsWith(input, "TIME") == 0) {
        const char* days_of_week[] = {
            "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
        };

        time_t current_time;
        getTime(&current_time);

        printColor("[o] ", BG_BLACK | FG_YELLOW);
        printFormat("Today is %s\n", days_of_week[current_time.day_of_week - 1]);
        printColor("[o] ", BG_BLACK | FG_YELLOW);
        printFormat("date is %d/%d/%d%d\n", current_time.day_of_month, current_time.month, current_time.century, current_time.year);
        printColor("[o] ", BG_BLACK | FG_YELLOW);
        printFormat("time is %d:%d:%d\n", current_time.hour, current_time.minute, current_time.second);

    } else if (equalsWith(input, "BUG") == 0) {
        printColor("[!] *** RAISING CONTROLLED EXCEPTION - 00 ***", BG_BLACK | FG_LTRED);
        operationSleep(1000);
        kernelException("Controlled Kernel Exception", 0, 0xDEADDEAD, NULL);

    } else if (startsWith("SUM", input) || startsWith("SUB", input) || startsWith("MUL", input)) {
        int result = 0;
        if (startsWith("SUM", input)) result = getInputSum(input, 4);
        if (startsWith("SUB", input)) result = getInputSub(input, 4);
        if (startsWith("MUL", input)) result = getInputMul(input, 4);

        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("-> = %d\n", result);

    } else if (startsWith("BEEP", input)) {
        playChord(input, 5);

    } else if (equalsWith(input, "TIMER") == 0) {
        printColor("[o] ", BG_BLACK | FG_YELLOW);
        printFormat("Internal: %d\n", getElapsedTimer());
        printColor("[o] ", BG_BLACK | FG_YELLOW);
        printFormat("Elapsed: %d:%d:%d\n", getTimerHours(), getTimerMinutes(), getTimerSeconds());

    } else if (equalsWith(input, "INFO") == 0) {
        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Build at %s\n\n", __TIMESTAMP__);
        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Created by %s\n\n", "Nahuel Senek (aka TheBigEye)");

        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Kernel version %s\n", __kernel_version);
        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Kernel project %s\n", __kernel_name);
        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Kernel codename %s\n", __kernel_version_codename);
        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Kernel CPU arch %s\n\n", __kernel_arch);

        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("GCC version %s\n\n", __VERSION__);

        printColor("[o] ", BG_BLACK | FG_YELLOW); print("Dedicated to:\n");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  My beloved,\n", BG_BLACK | FG_CYAN);
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  Hugo Luna,\n", BG_BLACK | FG_DKGRAY);
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  Pablo Zelaya,\n", BG_BLACK | FG_DKGRAY);
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  And my school friends :D\n", BG_BLACK | FG_BROWN);

    } else if (equalsWith(input, "CHARS") == 0) {
        printSupportedChars();

    } else if (equalsWith(input, "CMOSMEM") == 0) {
        printColor("[o] ", BG_BLACK | FG_YELLOW);
        printFormat("CMOS memory size: %s KB\n", itoa(getCmosTotalMemory()));

    } else if (equalsWith(input, "CPUID") == 0) {
        detectCPU();

    } else if (equalsWith(input, "BUGBUG") == 0) {
        printColor("[!] *** OH HOLY SHIT! ***", BG_BLACK | FG_LTRED);
        operationSleep(1000);
        for (uint8_t i = 0; i < 7; i++) {
            printColor(itoa(2 % i), BG_BLACK | FG_DKGRAY);
        }

    } else if (equalsWith(input, "RAND") == 0 || equalsWith(input, "URAND") == 0) {
        for (uint8_t i = 0; i < 7; i++) {
            printColor("[o] ", BG_BLACK | FG_YELLOW);
            printFormat("RNG: %s\n", equalsWith(input, "RAND") == 0 ? itoa(getSignedRandom()) : itoa(getUnsignedRandom()));
        }

    } else if (equalsWith(input, "HELP") == 0) {
        printColor("[o] ", BG_BLACK | FG_YELLOW); print("Kernel-mode commands:\n");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> halt the cpu and stop the system\n",            "EXIT\t\t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> perform a system shutdown process\n",           "SHUTDOWN\t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> perform a system reboot process\n",             "REBOOT  \t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> performs a memory paging process\n",            "PAGE\t\t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> clean up the current screen content\n",         "CLS or CLEAR\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> get and display the current time\n",            "TIME\t\t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> get and display the internal counter\n",        "TIMER   \t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> generate signed or unsigned random values\n",   "RAND or URAND   ");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> perform matematical operations\n",              "SUM SUB or MUL  ");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> reads and play a set of sound frequencies\n",   "BEEP\t\t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> get and display kernel version information\n",  "INFO\t\t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> get and print all the available characters\n",  "CHAR\t\t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> query and display the amount of RAM memory\n",  "CMOSMEM \t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> query and display the CPU information\n",       "CPUID   \t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> throw a handled kernel exception\n",            "BUG \t\t\t");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printFormat("%s -> throw a fatal handled kernel exception\n",      "BUGBUG  \t\t");

    } else {
        if (lengthString(input) != 0) { // We dont validate empty buffers
            playBeep(600, 100);
            PANIC("Invalid kernel-mode order or command!");
        }
    }

    printColor("\n/Monarch OS/[@] ", BG_BLACK | FG_GREEN);
}
