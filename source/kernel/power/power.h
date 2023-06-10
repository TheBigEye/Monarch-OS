#ifndef _KERNEL_POWER_H
#define _KERNEL_POWER_H 1

#include <stdint.h>
#include <stdbool.h>

void powerReboot(uint32_t time);
void powerShutdown(uint32_t time);

#endif /* KERNEL_POWER_H */
