#ifndef _KERNEL_POWER_H
#define _KERNEL_POWER_H 1

#include "../../common/sysutils.h"

/**
 * Halt the CPU with security and stops all.
 *
 * @param time The time to wait in milliseconds before halt.
 */
void powerHalt(uint32_t time);

/**
 * Reboots the computer after the specified time.
 *
 * @param time The time to wait in milliseconds before rebooting.
 */
void powerReboot(uint32_t time);

/**
 * Shuts down the computer after the specified time.
 *
 * @param time The time to wait in milliseconds before shutting down.
 */
void powerShutdown(uint32_t time);

#endif /* KERNEL_POWER_H */
