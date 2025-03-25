#include "randomly.h"

static uint32_t state[4];

static uint32_t rotl(const uint32_t x, int k) {
    return (x << k) | (x >> (32 - k));
}


uint32_t randomGet() {
    const uint32_t result = rotl(state[1] * 5, 7) * 9;
    const uint32_t t = state[1] << 9;

    state[2] ^= state[0];
    state[3] ^= state[1];
    state[1] ^= state[2];
    state[0] ^= state[3];

    state[2] ^= t;
    state[3] = rotl(state[3], 11);

    return result;
}


void randomSet(int seed) {
    state[0] = seed;
    state[1] = 0x97816A91U;
    state[2] = 0x78BC1ABC;
    state[3] = 0x9A3E2BB7;
}
