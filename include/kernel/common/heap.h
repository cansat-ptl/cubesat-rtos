/*
 * heap.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_HEAP_H_
#define KERNEL_HEAP_H_

#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/common/lists.h>

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

	kLinkedListItem_t allocListItem;
};

void common_heapInit();

void* common_heapAlloc(size_t size, kLinkedList_t* allocList);
void common_heapFree(void* pointer);

size_t common_getFreeHeap();
size_t common_getFreeHeapMin();

#endif