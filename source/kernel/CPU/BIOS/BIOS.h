#ifndef _BIOS_H
#define _BIOS_H 1

#include <stdint.h>
#include "../../../common/sysutils.h"

#define BIOS_ADDRESS    0x70
#define BIOS_DATA       0x71

void getBIOS();

#endif /* _BIOS_H */
