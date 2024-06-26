#ifndef _SYS_RANDOM_H
#define _SYS_RANDOM_H 1

#include "sysutils.h"

#define RANDOM_MAX 32767U

int randomGet(void);
void randomSet(int seed);

#endif /* ! _SYS_RANDOM_H */
