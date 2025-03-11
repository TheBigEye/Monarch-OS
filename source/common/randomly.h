#ifndef _LIB_RANDOM_H
#define _LIB_RANDOM_H 1

#include <stdint.h>

#define RANDOM_MAX 0xFFFFFFFFU

uint32_t randomGet(void);
void randomSet(int seed);

#endif /* ! _SYS_RANDOM_H */
