#ifndef HEAP_H_
#define HEAP_H_

#include "memory.h"
#include "../../common/monarch.h"

struct MemorySegmentHeader {
    uint_64 MemoryLength;
    MemorySegmentHeader* NextSegment;
    MemorySegmentHeader* PreviousSegment;
    MemorySegmentHeader* NextFreeSegment;
    MemorySegmentHeader* PreviousFreeSegment;
    bool Free;
};

struct AlignedMemorySegmentHeader {
	uint_64 MemorySegmentHeaderAddress : 63;
	bool IsAligned : 1;
};

void InitializeHeap(uint_64 heapAddress, uint_64 heapLength);

void* calloc(uint_64 size);
void* calloc(uint_64 num, uint_64 size);
void* CleanAllocate(uint_64 size);
void* CleanAllocate(uint_64 num, uint_64 size);

void* realloc(void* address, uint_64 newSize);

void* malloc(uint_64 size);
void* MemoryAllocate(uint_64 size);

void* aligned_alloc(uint_64 alignment, uint_64 size);

void free(void* address);

#endif /* HEAP_H_ */
