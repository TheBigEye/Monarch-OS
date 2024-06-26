#ifndef _KERNEL_BUTTERFLY_H
#define _KERNEL_BUTTERFLY_H 1

#include "../common/multiboot.h"
#include "../common/sysutils.h"

extern multiboot_info_t butterfly_info;

void configureKernel(multiboot_info_t* multiboot_header);
void butterflyGetStatus(void);

#endif /* _KERNEL_BUTTERFLY_H */
