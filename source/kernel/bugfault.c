#include "bugfault.h"

#include "../common/common.h"

#include "CPU/ISR/ISR.h"
#include "CPU/PIT/timer.h"
#include "CPU/HAL.h"
#include "drivers/console.h"
#include "drivers/speaker.h"
#include "power/power.h"

#include "binaries.h"

/**
 * If you're reading this, it's because you've probably broken something and want to cheat
 * a little :), or maybe you want to see what this is or how it actually works ._.
 */

static void printStackTrace(unsigned int ebp, int col, int row) {
    uint32_t *stack = (uint32_t *) ebp;
    uint32_t index = row;

    while (stack != 0) {
        // methodLocation is the dereference of EIP
        // (which is itself just above EBP on the stack)
        uint32_t method = *(stack + 1); // CWE-125 - Out Of Bounds Read

        ttyPutText(htoa(method), col, index, (BG_BLACK | FG_LTRED));
        if (*stack != 0) {
            index++;
        }
        // Keep derefencing EBP until we reach 0. If you infinite
        // loop here, make certain you set EBP to zero in the assembly stub
        stack = (uint32_t *)(*stack);
    }
}


/**
 * @brief Triggers a KERNEL PANIC!
 *
 * This function is called when a critical system error occurs, such as a device driver,
 * an bad operation, or an buggy kernel section. It halts the system to prevent further
 * damage and provides an stacktrace about the error.
 *
 * @param reason A description of the error or reason for the panic.
 * @param interrupt The interrupt number where the error occurred.
 * @param segment The memory segment associated with the error.
 * @param registers Pointer to the CPU registers at the time of the error (optional).
 */
void triggerPanic(const char *reason, uint32_t interrupt, uint32_t segment, registers_t *registers) {

    /* WARNING: never, NEVER, put IRQ based componentes here, no PIT, not RTC, etc. Console functions are safe :) */

    ASM VOLATILE ("cli"); // DISABLE INTERRUPTS

    setScreen(NULL);
    setCursor(0x3F);

    ttyPutText(butterfly, 0, 1, (BG_BLACK | FG_LTRED));

    ttyPutText(                            " Monarch OS "                                 ,  38, 20, (BG_LTGRAY | FG_BLACK));
    ttyPutText("A 32-bit device driver has corrupted critical system memory, resulting in",   8, 23, (BG_BLACK | FG_WHITE));
    ttyPutText("an exception at IRQ 00 and segment 0x00000000. The system has stopped now.",  8, 24, (BG_BLACK | FG_WHITE));

    ttyPutText( reason,                                                                       9, 26, (BG_BLACK | FG_WHITE));

    ttyPutText("* The current device has been halted to prevent any damage.",                 8, 28, (BG_BLACK | FG_WHITE));
    ttyPutText("* If a solution to the problem is not found, contact your",                   8, 29, (BG_BLACK | FG_WHITE));
    ttyPutText("  system administrator or technical.",                                        8, 30, (BG_BLACK | FG_WHITE));

    ttyPutText(itoa(interrupt),                                                              28, 24, (BG_BLACK | FG_WHITE));
    ttyPutText(htoa(segment),                                                                43, 24, (BG_BLACK | FG_WHITE));

    if (registers) {
        ttyPutText(" \"Uh Oh ... This isn't good ...\" ", 28, 34, (BG_BLACK | FG_LTRED));

        ttyPutText(" [registers at interrupt] ", 31, 37, (BG_BKRED | FG_BLACK));
        ttyPutText("eax = ", 16, 39, (BG_BLACK | FG_RED)); ttyPutText(htoa(registers->eax), 22, 39, (BG_BLACK | FG_LTRED));
        ttyPutText("ebx = ", 36, 39, (BG_BLACK | FG_RED)); ttyPutText(htoa(registers->ebx), 42, 39, (BG_BLACK | FG_LTRED));
        ttyPutText("ecx = ", 56, 39, (BG_BLACK | FG_RED)); ttyPutText(htoa(registers->ecx), 62, 39, (BG_BLACK | FG_LTRED));

        ttyPutText("edx = ", 16, 40, (BG_BLACK | FG_RED)); ttyPutText(htoa(registers->edx), 22, 40, (BG_BLACK | FG_LTRED));
        ttyPutText("esp = ", 36, 40, (BG_BLACK | FG_RED)); ttyPutText(htoa(registers->esp), 42, 40, (BG_BLACK | FG_LTRED));
        ttyPutText("ebp = ", 56, 40, (BG_BLACK | FG_RED)); ttyPutText(htoa(registers->ebp), 62, 40, (BG_BLACK | FG_LTRED));

        ttyPutText("error code = ", 32, 43, (BG_BLACK | FG_RED)); ttyPutText(htoa(registers->err_code), 45, 43, (BG_BLACK | FG_LTRED));
        ttyPutText("eflags = "    , 32, 44, (BG_BLACK | FG_RED)); ttyPutText(htoa(registers->eflags), 41, 44, (BG_BLACK | FG_LTRED));
        ttyPutText("eip = "       , 32, 45, (BG_BLACK | FG_RED)); ttyPutText(htoa(registers->eip), 38, 45, (BG_BLACK | FG_LTRED));

        ttyPutText("[stacktrace]", 38, 48, (BG_BKRED | FG_BLACK));
        printStackTrace(registers->ebp, 39, 50);
    }

    /* Explanation:
     * - EAX, EBX, ECX, EDX: General-purpose registers used in arithmetic operations and data storage.
     * - ESP (Stack Pointer): Points to the top of the current stack. Useful for stack tracing.
     * - EBP (Base Pointer): Points to the base of the stack frame. Used for stack tracing and local variables.
     * - EIP (Instruction Pointer): Points to the next instruction to be executed.
     *   By examining the value of EIP, we can determine the exact location in the code where the error occurred.
     * - EFLAGS: Flags register, which contains the current state of the CPU, such as the carry flag, zero flag, etc.
     * - Error Code: Provides additional information about the error. For example, it can indicate a specific type of exception.
     *
     * To find the function that caused the error, we can use the value of EIP.
     * 1. Use the EIP value to look up the corresponding function the symbol table or kernel map file.
     * 2. The symbol table will provide the function name and offset within the function where the error occurred.
     * 3. The stack trace, obtained using the EBP and ESP registers, will provide the call stack leading up to the error.
     *    This allows to trace back through the function calls to understand the sequence of events that led to the panic.
    */

    /* Dead end, doh! */
       FOREVER NOTHING
}


/**
 * @brief Trigger a short kernel exception.
 *
 * @param message The exception description message.
 * @param file The file where the exception occurred.
 * @param line The line number where the exception occurred.
 */
void triggerError(const char *message, const char *file, uint32_t line) {
    ttyPrintOut("\033[33;40m[EXCEPTION RAISED]", "[%s] -> [@%s:%d]\n\r", message, file, line);
}


/**
 * @brief Trigger a kernel assertion failure.
 *
 * @param file The name of the source file where the assertion occurred.
 * @param func The name of the function where the assertion occurred.
 * @param line The line number where the assertion occurred.
 * @param condition The failed condition.
 * @param message Description of what happened.
 */
void triggerAssert(const char *file, const char *func, uint32_t line, const char *condition, const char *message) {
    ttyPrintOut("\033[91;40m[ASSERTION FAILED]", "in section {%s : %s} at line {%d}\n\r", file, func, line);
    ttyPrintLog("\033[91;40m | \n\r");
    ttyPrintOut("\033[91;40m +--[AT CONDITION]", "(%s) -> [%s]\n\r", condition, message);
}
