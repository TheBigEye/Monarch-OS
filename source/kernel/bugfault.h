#ifndef _KERNEL_BUGFAULT_H
#define _KERNEL_BUGFAULT_H 1

#include <stdint.h>
#include "CPU/ISR/ISR.h"

/**
 * Trigger a kernel low priority panic.
 *
 * @param msg The panic message.
 */
#define PANIC(msg) kernelPanic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : kernelAssert(__FILE__, __LINE__, #b))


/**
 * Handle a kernel driver exception.
 *
 * @param reason The exception reason message or ISR error code.
 * @param interrupt The IRQ value that triggered the exception.
 * @param regpointer The memory pointer to the ISR register.
 */
void kernelException(const char *reason, uint32_t interrupt, uint32_t segment, registers_t *registers);

/**
 * Trigger a kernel panic.
 *
 * @param message The panic message.
 * @param file The file where the panic occurred.
 * @param line The line number where the panic occurred.
 */
void kernelPanic(const char *message, const char *file, uint32_t line);

/**
 * Handle a kernel assertion failure.
 *
 * @param file The file where the assertion failed.
 * @param line The line number where the assertion failed.
 * @param desc Description of the assertion failure.
 */
void kernelAssert(const char *file, uint32_t line, const char *desc);

#endif /* _KERNEL_BUGFAULT_H */
