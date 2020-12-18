/*
 * heap.h
 *
 * Created: 18.12.2020 6:19:32
 *  Author: Admin
 */


#ifndef HEAP_H_
#define HEAP_H_

#include <stddef.h>

struct kMemoryBlockStruct_t
{
	struct kMemoryBlockStruct_t* next;
	size_t blockSize;
	uint8_t state;
};

void* memory_heapAlloc(size_t size);
void memory_heapFree(void* pointer);

size_t memory_getFreeHeap();
size_t memory_getFreeHeapMin();

#endif /* HEAP_H_ */