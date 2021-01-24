/*
 * heap.h
 *
 * Created: 18.12.2020 6:19:32
 *  Author: Admin
 */


#ifndef HEAP_H_
#define HEAP_H_

#include <types.h>

struct kMemoryBlockStruct_t
{
	struct kMemoryBlockStruct_t* next;
	size_t blockSize;
	kBaseType_t state;
};

void memory_heapInit();

void* memory_heapAlloc(size_t size);
void memory_heapFree(void* pointer);

size_t memory_getFreeHeap();
size_t memory_getFreeHeapMin();

#endif /* HEAP_H_ */