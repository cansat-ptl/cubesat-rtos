/*
 * This is a modified version of heap4.c from the FreeRTOS project.
 * 
 * The FreeRTOS kernel is released under the MIT open source license, the text of
 * which is provided below.
 * 
 * This license covers the FreeRTOS kernel source files, which are located in the
 * /FreeRTOS/Source directory of the official FreeRTOS kernel download.  It also
 * covers most of the source files in the demo application projects, which are
 * located in the /FreeRTOS/Demo directory of the official FreeRTOS download.  The
 * demo projects may also include third party software that is not part of FreeRTOS
 * and is licensed separately to FreeRTOS.  Examples of third party software
 * includes header files provided by chip or tools vendors, linker scripts,
 * eripheral drivers, etc.  All the software in subdirectories of the /FreeRTOS
 * directory is either open source or distributed with permission, and is free for
 * use.  For the avoidance of doubt, refer to the comments at the top of each
 * source file.
 * 
 * 
 * License text:
 * -------------
 * 
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/arch/arch.h>
#include <kernel/mem/heap.h>
#include <kernel/common/lists.h>

#define HEAP_SIGNATURE_FIRST 0xBA
#define HEAP_SIGNATURE_LAST 0xDD

static byte kHeapRegion[CFG_HEAP_SIZE];

static struct kMemoryBlockStruct_t kHeapStart;
static struct kMemoryBlockStruct_t *kHeapEnd;

static size_t kFreeMemory = 0;
static size_t kMinimumFreeMemory = 0;

kReturnValue_t mem_heapPointerSanityCheck(void *pointer);

size_t mem_getFreeHeap()
{
	return kFreeMemory;
}

size_t mem_getFreeHeapMin()
{
	return kMinimumFreeMemory;
}

static inline uint8_t mem_checkBlockValid(struct kMemoryBlockStruct_t *block) 
{
	return (uint8_t)(block->magic1 == HEAP_SIGNATURE_FIRST && block->magic2 == HEAP_SIGNATURE_LAST);
}

void mem_heapInit()
{
	struct kMemoryBlockStruct_t *firstFreeBlock;
	byte *heapAligned;
	size_t heapAddress;
	size_t heapSize = CFG_HEAP_SIZE;

	heapAddress = (size_t)kHeapRegion;

	if ((heapAddress & CFG_PLATFORM_BYTE_ALIGNMENT_MASK) != 0) { //-V547
		heapAddress += (CFG_PLATFORM_BYTE_ALIGNMENT - 1);
		heapAddress &= ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK);
		heapSize -= heapAddress - (size_t)kHeapRegion;
	}

	heapAligned = (byte *)heapAddress;

	kHeapStart.next = (void *)heapAligned;
	kHeapStart.blockSize = (size_t)0;
	kHeapStart.magic1 = 0;
	kHeapStart.magic2 = 0;

	heapAddress = ((size_t)heapAligned) + heapSize;
	heapAddress -= mem_HEAP_STRUCT_SIZE;
	heapAddress &= ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK);

	kHeapEnd = (void *)heapAddress;
	kHeapEnd->blockSize = 0;
	kHeapEnd->next = NULL;
	kHeapEnd->magic1 = 0;
	kHeapEnd->magic2 = 0;

	firstFreeBlock = (void *)heapAligned;
	firstFreeBlock->blockSize = heapAddress - (size_t)firstFreeBlock;
	firstFreeBlock->next = kHeapEnd;
	firstFreeBlock->magic1 = HEAP_SIGNATURE_FIRST;
	firstFreeBlock->magic2 = HEAP_SIGNATURE_LAST;

	kMinimumFreeMemory = firstFreeBlock->blockSize;
	kFreeMemory = firstFreeBlock->blockSize;
}

static void mem_insertFreeBlock(struct kMemoryBlockStruct_t *blockToInsert)
{
	struct kMemoryBlockStruct_t *blockIterator;
	byte *pointer_casted;

	for (blockIterator = &kHeapStart; blockIterator->next < blockToInsert; blockIterator = blockIterator->next) {;}

	pointer_casted = (byte *)blockIterator;

	if ((pointer_casted + blockIterator->blockSize) == (byte *)blockToInsert) {
		blockIterator->blockSize += blockToInsert->blockSize;
		blockToInsert = blockIterator;
	}

	pointer_casted = (byte *)blockToInsert;

	if ((pointer_casted + blockToInsert->blockSize) == (byte *)blockIterator->next) {
		if (blockIterator->next != kHeapEnd) {
			blockToInsert->blockSize += blockIterator->next->blockSize;
			blockToInsert->next = blockIterator->next->next;
		}
		else {
			blockToInsert->next = kHeapEnd;
		}
	}
	else {
		blockToInsert->next = blockIterator->next;
	}

	if(blockIterator != blockToInsert) {
		blockIterator->next = blockToInsert;
	}

	return;
}

void* mem_heapAlloc(size_t size, kLinkedList_t *allocList)
{
	void *returnAddress = NULL;
	struct kMemoryBlockStruct_t *block;
	struct kMemoryBlockStruct_t *newBlock;
	struct kMemoryBlockStruct_t *previousBlock;

	arch_enterCriticalSection();

	if (size > 0 && (size & CFG_PLATFORM_BYTE_ALIGNMENT_MASK) != 0x00) { //-V560
		size += (CFG_PLATFORM_BYTE_ALIGNMENT - (size & CFG_PLATFORM_BYTE_ALIGNMENT_MASK));
	}

	if (size > 0) size += mem_HEAP_STRUCT_SIZE;

	if (size > 0 && size <= kFreeMemory) {
		previousBlock = &kHeapStart;
		block = kHeapStart.next;

		while ((block->blockSize < size) && (block->next != NULL)) {
			previousBlock = block;
			block = block->next;
		}

		if (block != kHeapEnd) {
			returnAddress = (void *)(((byte *)previousBlock->next) + mem_HEAP_STRUCT_SIZE);

			block->magic1 = HEAP_SIGNATURE_FIRST;

			previousBlock->next = block->next;

			if ((block->blockSize - size) > CFG_MIN_BLOCK_SIZE) {
				newBlock = (void *)(((byte *)block) + size);

				newBlock->blockSize = block->blockSize - size;
				block->blockSize = size;

				mem_insertFreeBlock(newBlock);
			}

			kFreeMemory -= block->blockSize;

			if(kFreeMemory < kMinimumFreeMemory) {
				kMinimumFreeMemory = kFreeMemory;
			}

			block->next = NULL;
			
			#if CFG_HEAP_ALLOCATION_TRACKING == 1
				if (allocList != NULL) {
					common_listAddBack(allocList, &(block->allocListItem));
					block->allocListItem.data = returnAddress;
				}
				else {
					block->allocListItem.data = NULL;
				}
			#endif

			block->magic2 = HEAP_SIGNATURE_LAST;
		}
	}

	arch_exitCriticalSection();
	return returnAddress;
}

void mem_heapFree(void *pointer)
{
	byte *pointer_casted = (byte *)pointer;
	struct kMemoryBlockStruct_t *block;

	arch_enterCriticalSection();

	if (mem_heapPointerSanityCheck(pointer) == KRESULT_SUCCESS) {
		pointer_casted -= mem_HEAP_STRUCT_SIZE;

		block = (void *)pointer_casted;

		if (mem_checkBlockValid(block)) {
			block->magic1 = 0;
			
			if (block->next == NULL) {
				#if CFG_HEAP_ALLOCATION_TRACKING == 1
					if (block->allocListItem.data != NULL) {
						common_listDeleteAny(block->allocListItem.list, &(block->allocListItem));
						block->allocListItem.data = NULL;
					}
				#endif

				kFreeMemory += block->blockSize;
				mem_insertFreeBlock((struct kMemoryBlockStruct_t *)block);
			}

			block->magic2 = 0;
		}
	}

	arch_exitCriticalSection();
	return;
}

void mem_heapFreeSafe(void **pointer)
{
	arch_enterCriticalSection();

	mem_heapFree(*pointer);
	*pointer = NULL;

	arch_exitCriticalSection();
	return;
}

kReturnValue_t mem_heapPointerSanityCheck(void *pointer)
{
	kReturnValue_t exitcode = KRESULT_ERR_MEMORY_VIOLATION;

	if (pointer != NULL) {
		if (pointer >= (void *)kHeapRegion) {
			if (pointer <= (void *)(kHeapRegion + CFG_HEAP_SIZE-1)) {
				exitcode = KRESULT_SUCCESS;
			}
		}
	}
	return exitcode;
}