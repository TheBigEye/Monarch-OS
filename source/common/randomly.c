#include "randomly.h"
#include "sysutils.h"

static unsigned long int next = 0;

int randomGet() {
    next = (next * 1103515245U + 12345U);
    return (unsigned int)(next / 65536) % (RANDOM_MAX + 1);
}

void randomSet(int seed) {
    next = seed;
}
