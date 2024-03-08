#include "kernel.h"

#include "CPU/BIOS/BIOS.h"
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

#include "utils/arithmetic.h"

#include "bugfault.h"
#include "poem.h"
#include "version.h"

/* The butterfly ascii logo */
extern char butterfly[];


void kernel_main(void) {

    // We load the screen config
    configureScreen();

    // We load the kernel drivers and modules
    initializeKernel();

    // We show a nice poem for you! :)
    initializePoem();

    // We use the PIT and CPU ticks like seed
    setRandom(getElapsedTimer() + getCpuTicks());

    clearScreen(BG_BLACK | FG_WHITE);

    timerSleep(50);
    putString(butterfly, 0, 1, BG_BLACK | FG_DKGRAY);
    timerSleep(50);
    putString(butterfly, 0, 1, BG_BLACK | FG_CYAN);

    startupSound();

    timerSleep(10);
    putString(" Monarch OS - Under development ", 22, 20, BG_BLACK | FG_DKGRAY); timerSleep(10);
    putString("   (C) 2022-2023 Nahuel Senek   ", 22, 21, BG_BLACK | FG_DKGRAY); timerSleep(10);
    timerSleep(10);

    printString("\n");

    // We init the keyboard calls
    configureKeyboard();
}

void cosoleHandler(char *input) {
    if (lengthString(input) > 0) {
        printString("\n");
    }

    if (equalsWith(input, "HALT") == 0) {
        printOutput("[i] ", BG_BLACK | FG_BROWN, "The CPU has HALTED NOW!");
        powerHalt(100);

    } else if (equalsWith(input, "SHUTDOWN") == 0) {
        printOutput("[i] ", BG_BLACK | FG_BROWN, "Shutting down the system ...");
        shutdownSound();
        powerShutdown(100);

    } else if (equalsWith(input, "REBOOT") == 0) {
        printOutput("[i] ", BG_BLACK | FG_BROWN, "Rebooting the system ...");
        shutdownSound();
        powerReboot(100);

    } else if (equalsWith(input, "CLS") == 0 || equalsWith(input, "CLEAR") == 0) {
        clearScreen(BG_BLACK | FG_WHITE);

    } else if (equalsWith(input, "PAGE") == 0) {
        for (uint8_t i = 0; i < 7; i++) {
            uint32_t physical_address;
            uint32_t memory_page = memoryAllocate(1000, 1, &physical_address);

            printOutput("[*] ", BG_BLACK | FG_MAGENTA, "{%d} Memory page: %s\n", i, htoa(memory_page));
            printOutput("[&] ", BG_BLACK | FG_LTMAGENTA, "{%d} Physical to: %s\n", i, htoa(physical_address));
        }

    } else if (equalsWith(input, "TIME") == 0) {
        const char* days_of_week[7] = {
            "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
        };

        time_t current_time;
        getTime(&current_time);

        printOutput("[o] ", BG_BLACK | FG_YELLOW, "Today is %s\n", days_of_week[current_time.day_of_week - 1]);
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "date is %d/%d/%d%d\n", current_time.day_of_month, current_time.month, current_time.century, current_time.year);
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "time is %d:%d:%d\n", current_time.hour, current_time.minute, current_time.second);

    } else if (equalsWith(input, "BUG") == 0) {
        kernelException("Controlled Kernel Exception", 0, 0xDEADDEAD, NULL);

    } else if (startsWith("CALC", input)) {
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "The result is %f\n", getArithmeticInput(input, 5));

    } else if (startsWith("BEEP", input)) {
        playChord(input, 5);

    } else if (equalsWith(input, "TIMER") == 0) {
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "Internal: %d\n", getElapsedTimer());
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "Elapsed: %d:%d:%d\n", getTimerHours(), getTimerMinutes(), getTimerSeconds());

    } else if (equalsWith(input, "INFO") == 0) {
        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Build at %s\n\n", __TIMESTAMP__);
        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Created by %s\n", "Nahuel Senek (aka TheBigEye)");
        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Testing by %s\n\n", "Sergio Vargas");

        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Kernel version %s\n", __kernel_version);
        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Kernel project %s\n", __kernel_name);
        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Kernel codename %s\n", __kernel_version_codename);
        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("Kernel CPU arch %s\n\n", __kernel_arch);

        printColor("[o] ", BG_BLACK | FG_YELLOW); printFormat("GCC version %s\n\n", __VERSION__);

        printColor("[o] ", BG_BLACK | FG_YELLOW); printString("Dedicated to:\n");
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  My teachers,\n", BG_BLACK | FG_CYAN);
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  Hugo Luna,\n", BG_BLACK | FG_DKGRAY);
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  Pablo Zelaya,\n", BG_BLACK | FG_DKGRAY);
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  And my school friends :D\n", BG_BLACK | FG_BROWN);

    } else if (equalsWith(input, "CHARS") == 0) {
        printCharset();

    } else if (equalsWith(input, "BIOS") == 0) {
        getBIOS();

    } else if (equalsWith(input, "CPUID") == 0) {
        detectCPU();

    } else if (equalsWith(input, "BUGBUG") == 0) {
        printColor("[!] *** OH HOLY SHIT! ***", BG_BLACK | FG_LTRED);
        timerSleep(100);
        for (uint8_t i = 0; i < 7; i++) {
            printColor(itoa(2 % i), BG_BLACK | FG_DKGRAY);
        }

    } else if (equalsWith(input, "RAND") == 0 || equalsWith(input, "URAND") == 0) {
        for (uint8_t i = 0; i < 7; i++) {
            printOutput("[o] ", BG_BLACK | FG_YELLOW, "RNG: %s\n", equalsWith(input, "RAND") == 0 ? itoa(getSignedRandom()) : itoa(getUnsignedRandom()));
        }

    } else if (equalsWith(input, "HELP") == 0) {
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "Kernel-mode commands:\n");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Halt the CPU and stop the system\n",            "HALT\t\t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Perform a system shutdown process\n",           "SHUTDOWN\t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Perform a system reboot process\n",             "REBOOT  \t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Perform a memory paging process\n",             "PAGE\t\t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Clean up the current screen content\n",         "CLS or CLEAR\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Get and display the current time\n",            "TIME\t\t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Get and display the internal counter\n",        "TIMER   \t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Generate signed or unsigned random values\n",   "RAND or URAND   ");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Perform matematical operations (+ - * /)\n",    "CALC\t\t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Reads and play a set of sound frequencies\n",   "BEEP\t\t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Get and display kernel version information\n",  "INFO\t\t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Get and print all the available characters\n",  "CHARS   \t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Query and display the BIOS information\n",      "BIOS\t\t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Query and display the CPU information\n",       "CPUID   \t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Throw a handled kernel exception\n",            "BUG \t\t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Throw a fatal handled kernel exception\n",      "BUGBUG  \t\t");
    } else {
        if (lengthString(input) != 0) { // We dont validate empty buffers :)
            playBeep(600, 50);
            PANIC("Invalid kernel-mode order or command!");
        }
    }
}


void initializeKernel(void) {
    clearScreen(BG_BLACK | FG_WHITE);
    ISR_install();
    operationSleep(1);
    IRQ_install();
    operationSleep(1);
    clearScreen(BG_BLACK | FG_WHITE);
    setCursor(0x3F);
}

void terminateKernel(void) {
    clearScreen(BG_BLACK | FG_WHITE);
    setCursor(0x00);
    IRQ_uninstall();
    operationSleep(1);
    clearScreen(BG_BLACK | FG_WHITE);
    setCursor(0x3F);
}
