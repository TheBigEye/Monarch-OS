#ifndef _KERNEL_BUTTERFLY_H
#define _KERNEL_BUTTERFLY_H 1

#include "../common/multiboot.h"
#include "../common/sysutils.h"

void initializeKernel(multiboot_info_t* multiboot_header);
void multibootGetInfo(void);
void ramdiskGetContent(void);

#endif /* _KERNEL_BUTTERFLY_H */
