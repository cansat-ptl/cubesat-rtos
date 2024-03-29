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
	struct kMemoryBlockStruct_t *next;

	uint8_t magic1;

	size_t blockSize;

	uint8_t magic2;

	#if CFG_HEAP_ALLOCATION_TRACKING == 1
		kLinkedListItem_t allocListItem;
	#endif
};

#define mem_HEAP_STRUCT_SIZE ((sizeof(struct kMemoryBlockStruct_t) \
				+ ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) \
				& ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK))

#ifdef __cplusplus
extern "C" {
#endif

void mem_heapInit();

void* mem_heapAlloc(size_t size, kLinkedList_t *allocList);
void mem_heapFree(void *pointer);

size_t mem_getFreeHeap();
size_t mem_getFreeHeapMin();

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_HEAP_H_ */