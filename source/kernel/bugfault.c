#include "bugfault.h"

#include "../common/sysutils.h"

#include "CPU/ISR/ISR.h"
#include "CPU/PIT/timer.h"
#include "CPU/HAL.h"
#include "drivers/console.h"
#include "drivers/sound.h"
#include "power/power.h"

#include "binaries.h"

void printStackTrace(unsigned int ebp, int column, int row) {
    uint32_t *stackPosition = (uint32_t *) ebp;
    uint32_t index = row;

    while (stackPosition != 0) {
        // methodLocation is the dereference of EIP
        // (which is itself just above EBP on the stack)
        uint32_t methodLocation = *(stackPosition + 1);

        //  You can look methodLocation up to get a method name
        putString(htoa(methodLocation), column, index, BG_BLACK | FG_LTRED);
        if (*stackPosition != 0) {
            index++;
        }
        // Keep derefencing EBP until we reach 0. If you infinite
        // loop here, make certain you set EBP to zero in the assembly stub
        stackPosition = (uint32_t *)(*stackPosition);
    }
}

/**
 * Handle a kernel driver exception.
 *
 * @param reason The exception reason message or ISR error code.
 * @param interrupt The IRQ value that triggered the exception.
 * @param regpointer The memory pointer to the ISR register.
 */
void kernelException(const char *reason, uint32_t interrupt, uint32_t segment, registers_t *registers) {

    __asm__ __volatile__ ("cli"); // DISABLE INTERRUPTS

    clearScreen();
    operationSleep(50);
    setCursor(0x3F);

    putString(butterfly, 0, 1, BG_BLACK | FG_LTRED);

    putString(" Uh Oh ... This isn't good ... ", 21, 20, BG_BLACK | FG_LTRED);
    operationSleep(1000);
    putString("                               ", 21, 20, BG_BLACK | FG_LTRED);

    if (registers) {
        putString(" [registers at interrupt] ", 25, 20, BG_BKRED | FG_BLACK);
        putString("eax = ", 10, 21, BG_BLACK | FG_RED); putString(htoa(registers->eax), 16, 21, BG_BLACK | FG_LTRED);
        putString("ebx = ", 30, 21, BG_BLACK | FG_RED); putString(htoa(registers->ebx), 36, 21, BG_BLACK | FG_LTRED);
        putString("ecx = ", 50, 21, BG_BLACK | FG_RED); putString(htoa(registers->ecx), 56, 21, BG_BLACK | FG_LTRED);

        putString("edx = ", 10, 22, BG_BLACK | FG_RED); putString(htoa(registers->edx), 16, 22, BG_BLACK | FG_LTRED);
        putString("esp = ", 30, 22, BG_BLACK | FG_RED); putString(htoa(registers->esp), 36, 22, BG_BLACK | FG_LTRED);
        putString("ebp = ", 50, 22, BG_BLACK | FG_RED); putString(htoa(registers->ebp), 56, 22, BG_BLACK | FG_LTRED);

        putString("error code = ", 26, 44, BG_BLACK | FG_RED); putString(htoa(registers->err_code), 39, 44, BG_BLACK | FG_LTRED);
        putString("eflags = ", 26, 45, BG_BLACK | FG_RED); putString(htoa(registers->eflags), 35, 45, BG_BLACK | FG_LTRED);
        putString("eip = ", 26, 46, BG_BLACK | FG_RED); putString(htoa(registers->eip), 32, 46, BG_BLACK | FG_LTRED);

        putString("[stacktrace]", 38, 48, BG_BKRED | FG_BLACK);
        printStackTrace(registers->ebp, 39, 50);
        operationSleep(8000);
    } else {
        operationSleep(1000);
    }

    char title[] = " Monarch OS ";
    char msg_line_1[] = "A 32-bit device driver has corrupted critical system memory, resulting";
    char msg_line_2[] = "in an exception 00 at seg:0x00000000 (reg). The system has stopped now.";
    char msg_line_3[] = "* The current device will automatically restart to";
    char msg_line_4[] = "  try to fix this error.";
    char msg_line_5[] = "* If a solution to the problem is not found, contact";
    char msg_line_6[] = "  your system administrator or technical.";
    char msg_line_7[] = "Rebooting...";

    timerSleep(500);
    setScreen(BG_RED | FG_WHITE);
    timerSleep(50);
    clearScreen();
    setCursor(0x3F);

    putString(title, 38, 20, BG_LTGRAY | FG_BLACK);
    putString(msg_line_1, 10, 23, BG_BLACK | FG_WHITE);
    putString(msg_line_2, 10, 24, BG_BLACK | FG_WHITE);
    putString(itoa(interrupt), 26, 24, BG_BLACK | FG_WHITE);
    putString(htoa((uint32_t)segment), 36, 24, BG_BLACK | FG_WHITE);
    putString(reason, 11, 26, BG_BLACK | FG_WHITE);
    putString(msg_line_3, 10, 28, BG_BLACK | FG_WHITE);
    putString(msg_line_4, 10, 29, BG_BLACK | FG_WHITE);
    putString(msg_line_5, 10, 30, BG_BLACK | FG_WHITE);
    putString(msg_line_6, 10, 31, BG_BLACK | FG_WHITE);

    playBeep(100, 3000);
    timerSleep(1500);
    putString(msg_line_7, 38, 34, BG_BLACK | FG_DKGRAY);
    timerSleep(500);
    clearScreen();
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
    __asm__ __volatile__ ("cli");
    printColor("[ASSERTION FAILURE] ", BG_BLACK | FG_BROWN);
    printColor(desc, BG_BLACK | FG_LTGRAY);
    printColor(" [@", BG_BLACK | FG_BROWN);
    printColor(file, BG_BLACK | FG_BROWN);
    printColor(":", BG_BLACK | FG_BROWN);
    printColor(itoa(line), BG_BLACK | FG_BROWN);
    printColor("]\n", BG_BLACK | FG_BROWN);
    return;
}
