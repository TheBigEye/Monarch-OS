#include "main.h"

#include "BFS/filesystem.h"
#include "BGL/demo.h"

#include "CPU//PIT/timer.h"
#include "CPU/RTC/clock.h"
#include "CPU/BIOS.h"
#include "CPU/CPU.h"
#include "CPU/HAL.h"

#include "drivers/VGA/video.h"
#include "drivers/console.h"
#include "drivers/graphics.h"
#include "drivers/keyboard.h"
#include "drivers/speaker.h"

#include "memory/heap.h"
#include "memory/memory.h"
#include "memory/paging.h"

#include "power/power.h"

#include "utils/arithmetic.h"
#include "utils/calendar.h"

#include "binaries.h"
#include "bugfault.h"
#include "butterfly.h"
#include "version.h"

/**
 * TODO: Most of the time, we use the console directly for the kernel logs
 * which right now I see a little... out of place... although it is fine,
 * I would prefer to use the serial port for more important things, This
 * way we leave the code as clean and less dependent as possible
*/

/* Kernel entry point */
void kernelMain(multiboot_info_t* multiboot_header, uint32_t boot_magic) {

    // Check if the bootloader is multiboot compliant
    if (boot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        THROW("Bootloader is not multiboot compliant!");

        /**
         * For now, no driver or kernel section actually uses
         * the mutiboot specification, so we're not going to
         * put a DEAD-END here
        */

        FOREVER NOTHING // <- dead end
    }

    // Initialize the kernel modules
    initializeKernel(multiboot_header);
    setScreen(NULL);

    /** NOTE: We need initialize the Heap before the File System */
    mountFileSystem();

    // We show a nice welcome screen fisrt ...
    initializeVGA(video_mode);
    fillScreen(PX_BLACK);

    bglPlayWork();

    timerSleep(1500);
    fillScreen(PX_BLACK);

    initializeVGA(text_mode);

    setScreen(NULL);
    setCursor(0x3F);

    ttyPutText(butterfly, 0, 1, (BG_BLACK | FG_CYAN));
    startupSound();

    ttyPutText(" Monarch OS - Under development ", 28, 20, (BG_BLACK | FG_DKGRAY));
    ttyPutText(" (C) 2022-2025 ", 36, 21, (BG_BLACK | FG_DKGRAY));

    ttyPrintStr("\n\n");

    memoryGetStatus();
    dumpMultiboot();

    // We use the PIT and CPU ticks like the RNG seed
    randomSet(timerGetTicks() + processorGetTicks());

    // We init the keyboard calls
    configureKeyboard();
}


/**
 * Okay okay okay... there shouldn't be a shell inside a kernel...
 * this isn't userspace yet... but it's very useful, right? :)
 */
void consoleMain(char *input) {

    /* I have a natural revulsion to any operating system that shows so little
     * planning as to have to named all of its commands after digestive noises
     * (awk, grep, fsck, nroff)
    */

    if (strlen(input) > 0) {
        ttyPrintStr("\n");
    }

    if (strcmp(input, "HALT") == 0) {
        ttyPrintLog(INFO "The CPU has HALTED NOW!");
        powerControl(POWER_HALT);

    } else if (strcmp(input, "SHUTDOWN") == 0) {
        ttyPrintLog(INFO "Shutting down the system ...");
        shutdownSound();

        setScreen(NULL);
        setCursor(0x3F);

        initializeVGA(video_mode);
        fillScreen(PX_BLACK);
        drawBitmapFast(candle_640, 0, 0, 640, 480);

        powerControl(POWER_SHUTDOWN);

        /* Just in case */
        FOREVER NOTHING

    } else if (strcmp(input, "REBOOT") == 0) {
        ttyPrintLog(INFO "Rebooting the system ...");
        shutdownSound();
        powerControl(POWER_REBOOT);

    } else if (strcmp(input, "CLS") == 0 || strcmp(input, "CLEAR") == 0) {
        setScreen(NULL);

    } else if (strcmp(input, "CLOCK") == 0) {
        getCalendar();

        const char* days_of_week[7] = {
            "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
        };

        time_t current_time;
        clockGetTime(&current_time);

        ttyPrintOut(LINE, "Today is %s\n", days_of_week[current_time.week_day - 1]);
        ttyPrintOut(LINE, "date is %d/%d/%d%d\n", current_time.month_day, current_time.month, current_time.century, current_time.year);
        ttyPrintOut(LINE, "time is %d:%d:%d\n", current_time.hour, current_time.minute, current_time.second);
        ttyPrintOut(LINE, "leap year?: %s\n", clockIsLeap(current_time.year) ? "true" : "false");

    } else if (strcmp(input, "BUG") == 0) {
        triggerPanic("Controlled Kernel Panic", 0, 0xDEADDEAD, NULL);

    } else if (substrcmp("CALC", input) == 0) {
        ttyPrintOut(LINE, "The result is %f\n", getMath(input, 5));

    } else if (substrcmp("BEEP", input) == 0) {
        playChord(input, 5);

    } else if (strcmp("TIMER", input) == 0) {
        ttyPrintOut(LINE, "Counter: %d\n", timerGetTicks());
        ttyPrintOut(LINE, "Elapsed: %d:%d:%d\n", timerGetHours(), timerGetMinutes(), timerGetSeconds());

    } else if (strcmp("INFO", input) == 0) {
        ttyPrintLog(INFO "Project development:\n");
        ttyPrintFmt(" * Developed by %s\n", "Nahuel Senek (TheBigEye)");
        ttyPrintFmt(" * Tested by %s\n", "Sergio Vargas");
        ttyPrintFmt(" * Inspired from Temple OS, by %s\n\n", "Terry Davis");

        ttyPrintLog(INFO "Kernel build info:\n");
        ttyPrintFmt(" * Kernel version %s\n", __kernel_version);
        ttyPrintFmt(" * Kernel project %s\n", __kernel_name);
        ttyPrintFmt(" * Kernel codename %s\n", __kernel_version_codename);
        ttyPrintFmt(" * Kernel CPU arch %s\n\n", __kernel_arch);

        ttyPrintOut(INFO, "GCC version %s\n", __VERSION__);
        ttyPrintFmt(" * Compiled at %s\n\n", __TIMESTAMP__);

        ttyPrintLog(INFO "This project is dedicated to:\n");
        ttyPrintFmt(
            " * My teachers [\n\r\t%s,\n\r\t%s,\n\r\t%s,\n\r\t%s\n\r   ]\n",
            "Hugo Luna", "Pablo Zelaya", "Sergio Scoleri", "Daniel Palacios"
        );
        ttyPrintLog("\033[96;40m * And my school friends :D \033[0m \n\r");

    } else if (strcmp("CHARS", input) == 0) {
        ttyCharset();

    } else if (strcmp("HEAP", input) == 0) {
        memoryGetStatus();

    } else if (strcmp("BOOT", input) == 0) {
        dumpMultiboot();

    } else if (strcmp("VIDEO", input) == 0) {
        ttyPrintLog(INFO "Switching to video mode...");
        timerSleep(150);
        initializeVGA(video_mode);

        fillScreen(PX_BLACK); timerSleep(20);
        fillScreen(PX_GREEN); timerSleep(20);
        fillScreen(PX_CYAN); timerSleep(20);
        fillScreen(PX_RED); timerSleep(20);
        fillScreen(PX_MAGENTA); timerSleep(20);
        fillScreen(PX_BROWN); timerSleep(20);
        fillScreen(PX_BLUE); timerSleep(20);

        drawCharset();
        drawString("- Two of the most famous products of Berkeley are LSD and Unix.\n\rI don't think that this is a coincidence ...", 8, 72, 0x10 | 0x0F);
        drawString("- Installing software on Windows is like opening a surprise box:\n\rYou never know what else comes bundled with the program you're installing", 8, 96, 0x10 | 0x0F);
        drawString("- MacOS users: convinced that paying a premium price for a computer\n\rmeans never having to admit that right-clicking might actually be useful", 8, 120, 0x10 | 0x0F);

        drawSolidRect(PX_LTGREEN, 16, 152, 128, 32); // rectangle
        drawLine(PX_LTGREEN, 16, 216, 144, 216); // horizontal line
        drawLine(PX_LTGREEN, 16, 248, 144, 280); // diagonal line

        drawEmptyCircle(PX_LTRED, 80, 328, 32);
        drawSolidCircle(PX_LTRED, 80, 408, 32);

        // vertical line
        drawLine(PX_LTMAGENTA, 176, 152, 176, 440);

    } else if (strcmp(input, "GRAPHICS") == 0) {
        ttyPrintLog(INFO "Starting graphics demo...");
        timerSleep(200);
        initializeVGA(video_mode);
        fillScreen(PX_BLACK);

        bglPlayDemo();

        timerSleep(100);
        fillScreen(PX_BLACK);

        bglPlayDemoEx();


    } else if (strcmp(input, "WALLPAPER") == 0) {
        ttyPrintLog(INFO "Switching to video mode...");
        timerSleep(200);
        initializeVGA(video_mode);
        fillScreen(PX_BLACK);
        timerSleep(200);

        fillScreen(PX_BLACK);
        drawBitmapFast(bigeye_480, 80, 0, 480, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(myfall_480, 80, 0, 480, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(myfood_480, 80, 0, 480, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(mylamb_480, 80, 0, 480, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(mylife_480, 80, 0, 480, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(mymind_480, 80, 0, 480, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(theman_480, 80, 0, 480, 480);
        timerSleep(1024);


        fillScreen(PX_BLACK);
        drawBitmapFast(candle_640, 0, 0, 640, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(choice_640, 0, 0, 640, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(clouds_640, 0, 0, 640, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(myhill_640, 0, 0, 640, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(mypain_640, 0, 0, 640, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(mypath_640, 0, 0, 640, 480);
        timerSleep(1024);

        fillScreen(PX_BLACK);
        drawBitmapFast(myroad_640, 0, 0, 640, 480);
        timerSleep(1024);


    } else if (strcmp(input, "CPUID") == 0) {
        processorGetStatus();


    } else if (strcmp(input, "MATH") == 0) {
        ttyPrintOut(LINE, "%-11s -> (-0.114784) = %f\n",  "[sin(69) rad ]", SIN(69));
        ttyPrintOut(LINE, "%-11s -> ( 0.993390) = %f\n",  "[cos(69) rad ]", COS(69));
        ttyPrintOut(LINE, "%-11s -> (-0.115548) = %f\n\n", "[tan(69) rad ]", TAN(69));

        ttyPrintOut(LINE, "%-11s -> ( 0.933580) = %f\n",   "[sin(69) deg ]", SIN(69 * (PI / 180.0)));
        ttyPrintOut(LINE, "%-11s -> ( 0.358367) = %f\n",   "[cos(69) deg ]", COS(69 * (PI / 180.0)));
        ttyPrintOut(LINE, "%-11s -> ( 2.605089) = %f\n\n", "[tan(69) deg ]", TAN(69 * (PI / 180.0)));

        ttyPrintOut(LINE, "%-11s -> ( 484416  ) = %d\n",   "[sqr(696)    ]", SQR(696));
        ttyPrintOut(LINE, "%-11s -> ( 264.001 ) = %f\n\n", "[sqrt(69697) ]", sqrt(69697));

        ttyPrintOut(LINE, "%-11s -> ( 2.00000 ) = %f\n",   "[ceil(1.3)   ]", CEIL(1.3));
        ttyPrintOut(LINE, "%-11s -> (-4.00000 ) = %f\n\n", "[floor(-3.14)]", FLOOR(-3.14));

        ttyPrintOut(LINE, "%-11s -> ( 150.792 ) = %f\n\n", "[3.1415*2*4*6]", getMath("3.1415*2*4*6", 0));

        ttyPrintLog(INFO "Press RETURN to continue ...\n\n");
        waitPressKeyboard(KEY_RETURN);

        timerSleep(2);

        ttyPrintLog(INFO "Press any key to continue ...\n\n");
        waitPressKeyboard(NULL);


    } else if (strcmp(input, "BUGBUG") == 0) {
        ttyPrintLog(ERROR "*** OH HOLY SHIT! ***");
        timerSleep(100);
        for (uint8_t i = 0; i < 7; i++) {
            ttyPrintLog(itoa(2 % i));
        }


    } else if (strcmp(input, "RAND") == 0 || strcmp(input, "URAND") == 0) {
        for (uint8_t i = 0; i < 7; i++) {
            ttyPrintFmt(" %s\n",
            strcmp(input, "RAND") == 0 ?
                itoa(randomGet()):              // For RAND: use full unsigned range
                itoa(randomGet() & 0x7FFFFFFF)  // For URAND: mask to positive range
            );
        }


    } else if (strcmp(input, "HELP") == 0) {
        ttyPrintLog(INFO "Availible commands:\n");
        ttyPrintFmt(" * %-15s -> %s\n", "HALT", "Halt the CPU and stop the system");
        ttyPrintFmt(" * %-15s -> %s\n", "SHUTDOWN", "Perform a system shutdown process");
        ttyPrintFmt(" * %-15s -> %s\n", "REBOOT", "Perform a system reboot process");
        ttyPrintFmt(" * %-15s -> %s\n", "CLS or CLEAR", "Clean up the current screen content");
        ttyPrintFmt(" * %-15s -> %s\n", "CLOCK", "Get and display the current time");
        ttyPrintFmt(" * %-15s -> %s\n", "TIMER", "Get and display the internal counter");
        ttyPrintFmt(" * %-15s -> %s\n", "RAND or URAND", "Generate signed or unsigned random values");
        ttyPrintFmt(" * %-15s -> %s\n", "CALC", "Perform mathematical operations (+ - * /)");
        ttyPrintFmt(" * %-15s -> %s\n", "BEEP", "Reads and play a set of sound frequencies");
        ttyPrintFmt(" * %-15s -> %s\n", "INFO", "Get and display kernel version information");
        ttyPrintFmt(" * %-15s -> %s\n", "CHARS", "Get and print all the available characters");
        ttyPrintFmt(" * %-15s -> %s\n", "HEAP", "Query and display the heap information");
        ttyPrintFmt(" * %-15s -> %s\n", "CPUID", "Query and display the CPU information");
        ttyPrintFmt(" * %-15s -> %s\n", "BUG", "Throw a handled kernel exception");
        ttyPrintFmt(" * %-15s -> %s\n", "BUGBUG", "Throw a fatal handled kernel exception");


    } else if (strncmp(input, "MKDIR ", 6) == 0) {
        const char *path = input + 6;
        char *slash = strrchr(path, '/');

        if (slash) {
            char parentPath[MAX_NAME_LEN];
            strncpy(parentPath, path, slash - path + 1);
            parentPath[slash - path] = '\0';
            Directory *parent = bfsFindDirectory(parentPath);
            if (parent) {
                if (bfsCheckName(slash + 1)) {
                    bfsCreateDirectory(parent, slash + 1);
                } else {
                    ttyPrintLog(ERROR "Invalid directory name\n\r");
                }
            } else {
                ttyPrintLog(ERROR "Cannot find the parent directory\n\r");
            }
        } else {
            if (bfsCheckName(path)) {
                bfsCreateDirectory(BFS_CURRENT_DIR, path);
            } else {
                ttyPrintLog(ERROR "Invalid directory name\n\r");
            }
        }


    } else if (strncmp(input, "TOUCH ", 6) == 0) {
        const char *path = input + 6;
        char *slash = strrchr(path, '/');

        if (slash) {
            char parentPath[MAX_NAME_LEN];
            strncpy(parentPath, path, slash - path + 1);
            parentPath[slash - path] = '\0';
            Directory *parent = bfsFindDirectory(parentPath);
            if (parent) {
                if (bfsCheckName(slash + 1)) {
                    bfsCreateFile(parent, slash + 1);
                } else {
                    ttyPrintLog(ERROR "Invalid file name\n\r");
                }
            } else {
                ttyPrintLog(ERROR "Cannot find the parent directory\n\r");
            }

        } else {
            if (bfsCheckName(path)) {
                bfsCreateFile(BFS_CURRENT_DIR, path);
            } else {
                ttyPrintLog(ERROR "Invalid file name\n\r");
            }
        }


    } else if (strncmp(input, "RM ", 3) == 0) {
        const char *path = input + 3;
        char *slash = strrchr(path, '/');
        Directory *directory;

        if (slash) {
            char dirPath[MAX_NAME_LEN];
            strncpy(dirPath, path, (slash - path + 1));
            dirPath[slash - path] = '\0';
            directory = bfsFindDirectory(dirPath);
            path = slash + 1;
        } else {
            directory = BFS_CURRENT_DIR;
        }

        if (directory) {
            File *file = directory->files;
            while (file && strcmp(file->name, path) != 0) {
                file = file->next;
            }

            if (file) {
                bfsRemoveFile(directory, file->name);
                ttyPrintLog(INFO "File removed successfully\n\r");
            } else {
                ttyPrintLog(ERROR "Cannot find the file specified\n\r");
            }
        } else {
            ttyPrintLog(ERROR "Cannot find the directory specified\n\r");
        }


    } else if (strncmp(input, "RMDIR ", 6) == 0) {
        Directory *directory = bfsFindDirectory(input + 6);

        if (directory) {

            /* Just in case ...*/
            if (directory == BFS_PRIMARY_DIR) {
                ttyPrintLog(ERROR "Cannot remove root directory\n\r");
                return;
            }

            if (directory->parent) {
                Directory *parent = directory->parent;
                Directory *prev = NULL;
                Directory *subdir = parent->subdirs;

                // Search in the parent subdirectory
                while (subdir && subdir != directory) {
                    prev = subdir;
                    subdir = subdir->next;
                }

                // If found ...
                if (subdir) {
                    if (prev) {
                        prev->next = subdir->next;
                    } else {
                        parent->subdirs = subdir->next;
                    }

                    // we remove it
                    bfsRemoveDirectory(subdir);
                    ttyPrintLog(INFO "Directory removed successfully\n\r");

                } else {
                    ttyPrintLog(ERROR "Cannot find the directory in parent\n\r");
                }
            } else {
                ttyPrintLog(ERROR "The directory has no parent\n\r");
            }
        } else {
            ttyPrintLog(ERROR "Cannot find the directory specified\n\r");
        }


    } else if (strncmp(input, "WRITE ", 6) == 0) {
        const char *filename = input + 6;
        char *content = strchr(filename, ' ');

        if (content) {
            *content = '\0';
            content++;

            File *file = bfsFindFile(filename);
            if (file) {
                bfsWriteFile(file, content);
                ttyPrintLog(INFO "File written successfully\n\r");
            } else {
                ttyPrintLog(ERROR "Cannot find the file specified\n\r");
            }
        } else {
            ttyPrintLog(ERROR "Invalid command format\n\r");
        }


    } else if (strncmp(input, "CAT ", 4) == 0) {
        File *file = bfsFindFile(input + 4);
        if (file) {
            bfsReadFile(file);
        } else {
            ttyPrintLog(ERROR "Cannot find the file specified\n\r");
        }


    } else if (strncmp(input, "LS", 2) == 0) {
        const char *path = input[2] == '\0' ? NULL : input + 4;
        Directory *directory = path ? bfsFindDirectory(path) : BFS_CURRENT_DIR;

        if (!directory) {
            ttyPrintLog(ERROR "Cannot find the directory specified\n\r");
            return;
        }

        ttyPrintOut(INFO, "Contents of directory <%s>\r\n", directory->name);

        Directory *subdir = directory->subdirs;
        while (subdir) {
            ttyPrintOut("\033[93;40m\t[/] \033[0m", "/%s\r\n", subdir->name);
            subdir = subdir->next;
        }

        File *file = directory->files;
        while (file) {
            ttyPrintOut("\033[33;40m\t *  \033[0m", "%s {%d bytes}\r\n", file->name, file->size);
            file = file->next;
        }


    } else if (strncmp(input, "CD ", 3) == 0) {
        const char *targetPath = input + 3;

        if (strcmp(targetPath, "..") == 0) {
            if (BFS_CURRENT_DIR->parent) {
                BFS_CURRENT_DIR = BFS_CURRENT_DIR->parent;
            }

        } else {
            Directory *directory = bfsFindDirectory(targetPath);
            if (directory) {
                BFS_CURRENT_DIR = directory;
            } else {
                ttyPrintLog(ERROR "Cannot find the directory specified\n\r");
            }
        }


    } else if (strcmp(input, "TREE") == 0) {
        bfsPrintTree(BFS_PRIMARY_DIR, 0);


    } else if (strcmp(input, "PWD") == 0) {
        ttyPrintOut(INFO, "Current working directory is <%s>\r\n", BFS_CURRENT_DIR->name);


    } else if (strncmp(input, "CP ", 3) == 0) {
        const char *sourcePath = input + 3;
        const char *targetPath = strchr(sourcePath, ' ');

        if (targetPath) {
            char source[MAX_NAME_LEN];
            char target[MAX_NAME_LEN];

            strncpy(source, sourcePath, (targetPath - sourcePath + 1));
            source[targetPath - sourcePath] = '\0';
            targetPath++;
            strcpy(target, targetPath);

            File *file = bfsFindFile(source);
            Directory *directory = bfsFindDirectory(target);

            if (file && directory) {
                bfsCopyFile(file, directory, NULL);
                ttyPrintLog(INFO "File copied successfully\n\r");
            } else {
                ttyPrintLog(ERROR "Cannot find the source file or destination directory\n\r");
            }
        } else {
            ttyPrintLog(ERROR "Invalid command format\n\r");
        }

    } else {
        // We dont validate empty buffers :)
        if (strlen(input) != 0) {
            playBeep(600, 32);
            THROW("Command not found!");
        }
    }
}
