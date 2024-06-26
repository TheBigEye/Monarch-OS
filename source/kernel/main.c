#include "main.h"

#include "CPU/CMOS/CMOS.h"
#include "CPU//GDT/GDT.h"
#include "CPU//ISR/ISR.h"
#include "CPU//PIT/timer.h"
#include "CPU/RTC/clock.h"
#include "CPU/BIOS.h"
#include "CPU/CPU.h"
#include "CPU/HAL.h"

#include "drivers/VGA/VGA.h"
#include "drivers/console.h"
#include "drivers/graphics.h"
#include "drivers/keyboard.h"
#include "drivers/sound.h"

#include "memory/heap.h"
#include "memory/memory.h"
#include "memory/paging.h"

#include "power/power.h"

#include "utils/arithmetic.h"

#include "binaries.h"
#include "bugfault.h"
#include "butterfly.h"
#include "poem.h"
#include "version.h"

/* The kernel 'end' section */
extern uint32_t end;

void monarchKernelMain(multiboot_info_t* multiboot_header, uint32_t boot_magic) {

    // Check if the bootloader is multiboot compliant
    if (boot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        PANIC("Bootloader is not multiboot compliant!");
    }

    // We pass the multiboot info to the kernel
    configureKernel(multiboot_header);

    // We configure the screen in text mode
    configureScreen(text_mode, false);

    // Clear the screen's trash
    clearScreen();

    gdtDoInstall(); // Install the General Descriptor Table
    isrDoInstall(); // Intall exception handlers
    irqDoInstall(); // Install drivers
    timerSleep(2);
    clearScreen();

    // Initialize the memory heap
    initializeMemory(&end);
    initializePaging();
    timerSleep(2);
    clearScreen();

    // We show a nice poem for you! :)
    initializeMyPoem();
    clearScreen();

    timerSleep(20);
    putString(butterfly, 0, 1, BG_BLACK | FG_DKGRAY);
    timerSleep(20);
    putString(butterfly, 0, 1, BG_BLACK | FG_CYAN);

    startupSound();

    timerSleep(10);
    putString(" Monarch OS - Under development ", 28, 20, BG_BLACK | FG_DKGRAY);
    putString("   (C) 2022-2024 Nahuel Senek   ", 28, 21, BG_BLACK | FG_DKGRAY);
    timerSleep(10);

    printString("\n\n");
    memoryGetStatus();
    printString("\n\n");
    butterflyGetStatus();
    printString("\n\n");

    // We use the PIT and CPU ticks like the RNG seed
    randomSet(timerGetTicks() + processorGetTicks());

    // We init the keyboard calls
    configureKeyboard();
}

void monarchConsoleMain(char *input) {

    /*
     * I have a natural revulsion to any operating system that shows so little
     *  planning as to have to named all of its commands after digestive noises
     *  (awk, grep, fsck, nroff)
    */

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
        clearScreen();

    } else if (equalsWith(input, "TIME") == 0) {
        const char* days_of_week[7] = {
            "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
        };

        time_t current_time;
        clockGetTime(&current_time);

        printOutput("[o] ", BG_BLACK | FG_YELLOW, "Today is %s\n", days_of_week[current_time.day_of_week - 1]);
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "date is %d/%d/%d%d\n", current_time.day_of_month, current_time.month, current_time.century, current_time.year);
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "time is %d:%d:%d\n", current_time.hour, current_time.minute, current_time.second);

    } else if (equalsWith(input, "BUG") == 0) {
        kernelException("Controlled Kernel Exception", 0, 0xDEADDEAD, NULL);

    } else if (startsWith("CALC", input)) {
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "The result is %f\n", getMath(input, 5));

    } else if (startsWith("BEEP", input)) {
        playChord(input, 5);

    } else if (equalsWith(input, "TIMER") == 0) {
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "Internal: %d\n", timerGetTicks());
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "Elapsed: %d:%d:%d\n", timerGetHours(), timerGetMinutes(), timerGetSeconds());

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
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  Sergio Scoleri,\n", BG_BLACK | FG_DKGRAY);
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  Daniel Palacios,\n", BG_BLACK | FG_DKGRAY);
        printColor("[-] ", BG_BLACK | FG_YELLOW); printColor("  And my school friends :D\n", BG_BLACK | FG_BROWN);

    } else if (equalsWith(input, "CHARS") == 0) {
        printCharset();

    } else if (equalsWith(input, "BIOS") == 0) {
        firmwareGetStatus();
        printString("\n");
        firmwareGetFloppy();

    } else if (equalsWith(input, "MEM") == 0) {
        memoryGetStatus();

    } else if (equalsWith(input, "BTFLY") == 0) {
        butterflyGetStatus();

    } else if (equalsWith(input, "VIDEO") == 0) {
        printOutput("[i] ", BG_BLACK | FG_BROWN, "Switching to video mode...");
        timerSleep(200);
        configureScreen(video_mode, true);
        fillScreen(PX_BLACK); timerSleep(20);
        drawCharacter('A', 8, 8, 0x00 | 0x0F); timerSleep(20);
        fillScreen(PX_BLUE); timerSleep(20);
        drawCharacter('B', 16, 8, 0x00 | 0x0F); timerSleep(20);
        fillScreen(PX_GREEN); timerSleep(20);
        drawCharacter('C', 24, 8, 0x00 | 0x0F); timerSleep(20);
        fillScreen(PX_CYAN); timerSleep(20);
        drawCharacter('D', 32, 8, 0x00 | 0x0F); timerSleep(20);
        fillScreen(PX_RED); timerSleep(20);
        drawCharacter('E', 40, 8, 0x00 | 0x0F); timerSleep(20);
        fillScreen(PX_MAGENTA); timerSleep(20);
        drawCharacter('F', 48, 8, 0x00 | 0x0F); timerSleep(20);
        fillScreen(PX_BROWN); timerSleep(20);
        drawCharacter('G', 56, 8, 0x00 | 0x0F); timerSleep(20);
        fillScreen(PX_LTGRAY); timerSleep(20);
        drawString("Hello, World\n\rThis is a \t test, graphics text! ...", 64, 8, 0x00 | 0x0F); timerSleep(40);
        timerSleep(100);
        fillScreen(PX_LTGRAY); timerSleep(20);

        drawRect(PX_GREEN, 48, 16, 48, 24); timerSleep(40);

        // diagonal line
        drawLine(PX_CYAN, 16, 64, 128, 128); timerSleep(40);

        // horizontal line
        drawLine(PX_GREEN, 16, 192, 128, 192); timerSleep(40);

        drawEmptyCircle(PX_LTRED, 255, 192, 32); timerSleep(40);
        drawSolidCircle(PX_LTRED, 351, 192, 32); timerSleep(40);

        // vertical line
        drawLine(PX_BLUE, 64, 256, 64, 380); timerSleep(40);

        timerSleep(512);
        /*fillScreen(0x80 | 0x08); timerSleep(40);*/
        /*fillScreen(0x90 | 0x09);*/ drawBitmap(mouse_bitmap, 8, 8, 9, 18); timerSleep(80);
        /*fillScreen(0xA0 | 0x0A);*/ drawBitmap(mouse_bitmap, 16, 8, 9, 18); timerSleep(80);
        /*fillScreen(0xB0 | 0x0B);*/ drawBitmap(mouse_bitmap, 24, 8, 9, 18); timerSleep(80);

        /*fillScreen(0xD0 | 0x0D);*/ drawBitmapFast(mouse_bitmap, 40, 8, 9, 18); timerSleep(80);
        /*fillScreen(0xE0 | 0x0E);*/ drawBitmapFast(mouse_bitmap, 48, 8, 9, 18); timerSleep(80);
        /*fillScreen(0xF0 | 0x0F);*/ drawBitmapFast(mouse_bitmap, 56, 8, 9, 18); timerSleep(80);
        timerSleep(100);

    } else if (equalsWith(input, "WALLPAPER") == 0) {
        printOutput("[i] ", BG_BLACK | FG_BROWN, "Switching to video mode...");
        timerSleep(200);
        configureScreen(video_mode, true);

        fillScreen(PX_BLACK); drawBitmap(wallpaper, 0, 0, 640, 480); timerSleep(512);
        fillScreen(PX_BLUE); drawBitmap(leafpaper, 0, 0, 640, 480); timerSleep(512);
        fillScreen(PX_GREEN); drawBitmap(workpaper, 0, 0, 640, 480); timerSleep(512);

        fillScreen(PX_CYAN); drawBitmapFast(wallpaper, 0, 0, 640, 480); timerSleep(512);
        fillScreen(PX_RED); drawBitmapFast(leafpaper, 0, 0, 640, 480); timerSleep(512);
        fillScreen(PX_MAGENTA); drawBitmapFast(workpaper, 0, 0, 640, 480); timerSleep(512);

    } else if (equalsWith(input, "CPUID") == 0) {
        processorGetStatus();

    } else if (equalsWith(input, "MATH") == 0) {
        printOutput("[TEST -> sin(69) rad ] = ", BG_BLACK | FG_LTGREEN, "%f (should be (double) -0.1147 rad)\n", SIN(69));
        printOutput("[TEST -> cos(69) rad ] = ", BG_BLACK | FG_LTGREEN, "%f (should be (double)  0.3583 rad)\n", COS(69));
        printOutput("[TEST -> tan(69) rad ] = ", BG_BLACK | FG_LTGREEN, "%f (should be (double) -0.1155 rad)\n\n", TAN(69));

        printOutput("[TEST -> sin(69) deg ] = ", BG_BLACK | FG_LTGREEN, "%f (should be (double) 0,9335 deg)\n", SIN(69 * (PI / 180.0)));
        printOutput("[TEST -> cos(69) deg ] = ", BG_BLACK | FG_LTGREEN, "%f (should be (double) 0,3583 deg)\n", COS(69 * (PI / 180.0)));
        printOutput("[TEST -> tan(69) deg ] = ", BG_BLACK | FG_LTGREEN, "%f (should be (double) 2,605 deg)\n\n", TAN(69 * (PI / 180.0)));

        printOutput("[TEST -> sqr(69)     ] = ", BG_BLACK | FG_LTGREEN, "%d (should be (int) 4761)\n", SQR(69));
        printOutput("[TEST -> sqrt(69)    ] = ", BG_BLACK | FG_LTGREEN, "%f (should be (double) 8,306)\n\n", sqrt(69));

        printOutput("[TEST -> ceil(1.3)   ] = ", BG_BLACK | FG_LTGREEN, "%f (should be (double) 2,000)\n", CEIL(1.3));
        printOutput("[TEST -> floor(-3.14)] = ", BG_BLACK | FG_LTGREEN, "%f (should be (double) -4,000)\n", FLOOR(-3.14));

    } else if (equalsWith(input, "BUGBUG") == 0) {
        printColor("[!] *** OH HOLY SHIT! ***", BG_BLACK | FG_LTRED);
        timerSleep(100);
        for (uint8_t i = 0; i < 7; i++) {
            printColor(itoa(2 % i), BG_BLACK | FG_DKGRAY);
        }

    } else if (equalsWith(input, "RAND") == 0 || equalsWith(input, "URAND") == 0) {
        for (uint8_t i = 0; i < 7; i++) {
            printFormat("%s\n",
            equalsWith(input, "RAND") == 0 ?
                itoa(randomGet()) :
                itoa((-1 * (randomGet() + 1)))
            );
        }

    } else if (equalsWith(input, "HELP") == 0) {
        printOutput("[o] ", BG_BLACK | FG_YELLOW, "Kernel-mode commands:\n");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Halt the CPU and stop the system\n",            "HALT\t\t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Perform a system shutdown process\n",           "SHUTDOWN\t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Perform a system reboot process\n",             "REBOOT  \t\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Clean up the current screen content\n",         "CLS or CLEAR\t");
        printOutput("[-] ", BG_BLACK | FG_YELLOW, "%s -> Query and display the heap information\n",      "MEM \t\t\t");
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
