/*
 * heap.h
 *
 * Created: 18.12.2020 6:19:32
 *  Author: Admin
 */


#ifndef HEAP_H_
#define HEAP_H_

#include <types.h>
#include <config.h>
#include <common.h>

struct kMemoryBlockStruct_t
{
	struct kMemoryBlockStruct_t* next;

	size_t blockSize;
	kBaseType_t state;

	#if CFG_PROTECT_FROM_INVALID_HEAP_FREE == 1
		uint16_t magic1;
		uint16_t magic2;
		uint8_t checksum;
	#endif

	#if CFG_TRACK_MEMORY_BLOCK_OWNERS == 1
		kTaskHandle_t owner;
		volatile struct kListItemStruct_t ownListItem;
	#endif
};

void memory_heapInit();

void* memory_heapAlloc(size_t size);
void memory_heapFree(void* pointer);

size_t memory_getFreeHeap();
size_t memory_getFreeHeapMin();

#endif /* HEAP_H_ */