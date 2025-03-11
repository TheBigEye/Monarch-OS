#ifndef _KERNEL_BUTTERFLY_H
#define _KERNEL_BUTTERFLY_H 1

#include "../common/multiboot.h"
#include "../common/common.h"

void initializeKernel(multiboot_info_t* multiboot_header);
void dumpMultiboot(void);
void ramdiskGetContent(void);

#endif /* _KERNEL_BUTTERFLY_H */
