#ifndef MEMORY_H_
#define MEMORY_H_

#include "../../common/monarch.h"

void memset(void* start, uint_64 value, uint_64 num);
void memcpy(void* destination, void* source, uint_64 num);

#endif /* MEMORY_H_ */
