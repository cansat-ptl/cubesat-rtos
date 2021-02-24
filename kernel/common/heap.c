/*
 * heap.c
 *
 * Created: 18.12.2020 6:21:51
 *  Author: Admin
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/arch/arch.h>
#include <kernel/common/heap.h>
#include <kernel/common/lists.h>

#define HEAP_SIGNATURE_FIRST16 0x4845
#define HEAP_SIGNATURE_LAST16 0x4150

static byte kHeapRegion[CFG_HEAP_SIZE];

static const size_t kHeapStructSize = (sizeof(struct kMemoryBlockStruct_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK); /* makes sense lol */

static struct kMemoryBlockStruct_t kHeapStart;
static struct kMemoryBlockStruct_t *kHeapEnd;

static size_t kFreeMemory = 0;
static size_t kMinimumFreeMemory = 0;

kReturnValue_t common_heapPointerSanityCheck(void *pointer);

size_t common_getFreeHeap()
{
	return kFreeMemory;
}

size_t common_getFreeHeapMin()
{
	return kMinimumFreeMemory;
}

static inline void common_prepareBlockMagic(struct kMemoryBlockStruct_t *block) 
{
	block->magic1 = HEAP_SIGNATURE_FIRST16;
	block->magic2 = HEAP_SIGNATURE_LAST16;
}

static inline uint8_t common_checkBlockValid(struct kMemoryBlockStruct_t *block) 
{
	uint8_t result = 0;

	if (block->magic1 == HEAP_SIGNATURE_FIRST16 && block->magic2 == HEAP_SIGNATURE_LAST16) {
		result = 1;
	}

	return result;
}

void common_heapInit()
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
	kHeapStart.state = 0;

	heapAddress = ((size_t)heapAligned) + heapSize;
	heapAddress -= kHeapStructSize;
	heapAddress &= ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK);

	kHeapEnd = (void *)heapAddress;
	kHeapEnd->blockSize = 0;
	kHeapEnd->next = NULL;
	kHeapEnd->state = 0;

	firstFreeBlock = (void *)heapAligned;
	firstFreeBlock->blockSize = heapAddress - (size_t)firstFreeBlock;
	firstFreeBlock->next = kHeapEnd;
	firstFreeBlock->state = 0;

	#if CFG_PROTECT_FROM_INVALID_HEAP_FREE == 1
		common_prepareBlockMagic(firstFreeBlock);
	#endif

	kMinimumFreeMemory = firstFreeBlock->blockSize;
	kFreeMemory = firstFreeBlock->blockSize;
}

static void common_insertFreeBlock(struct kMemoryBlockStruct_t *blockToInsert)
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

void* common_heapAlloc(size_t size, kLinkedList_t *allocList)
{
	void *returnAddress = NULL;
	struct kMemoryBlockStruct_t *block;
	struct kMemoryBlockStruct_t *newBlock;
	struct kMemoryBlockStruct_t *previousBlock;

	arch_enterCriticalSection();

	if (size > 0 && (size & CFG_PLATFORM_BYTE_ALIGNMENT_MASK) != 0x00) { //-V560
		size += (CFG_PLATFORM_BYTE_ALIGNMENT - (size & CFG_PLATFORM_BYTE_ALIGNMENT_MASK));
	}

	if (size > 0) size += kHeapStructSize;

	if (size > 0 && size <= kFreeMemory) {
		previousBlock = &kHeapStart;
		block = kHeapStart.next;

		while ((block->blockSize < size) && (block->next != NULL)) {
			previousBlock = block;
			block = block->next;
		}

		if (block != kHeapEnd) {
			returnAddress = (void *)(((byte *)previousBlock->next) + kHeapStructSize);

			previousBlock->next = block->next;

			if ((block->blockSize - size) > CFG_MIN_BLOCK_SIZE) {
				newBlock = (void *)(((byte *)block) + size);

				newBlock->blockSize = block->blockSize - size;
				block->blockSize = size;

				common_insertFreeBlock(newBlock);
			}

			kFreeMemory -= block->blockSize;

			if(kFreeMemory < kMinimumFreeMemory) {
				kMinimumFreeMemory = kFreeMemory;
			}

			block->state = 1;
			block->next = NULL;

			if (allocList != NULL) {
				common_listAddBack(allocList, &(block->allocListItem));
				block->allocListItem.data = returnAddress;
			}
			else {
				block->allocListItem.data = NULL;
			}

			#if CFG_PROTECT_FROM_INVALID_HEAP_FREE == 1
				common_prepareBlockMagic(block);
			#endif
		}
	}

	arch_exitCriticalSection();
	return returnAddress;
}

void common_heapFree(void *pointer)
{
	byte *pointer_casted = (byte *)pointer;
	struct kMemoryBlockStruct_t *block;

	arch_enterCriticalSection();

	if (common_heapPointerSanityCheck(pointer) == KRESULT_SUCCESS) {
		pointer_casted -= kHeapStructSize;

		block = (void *)pointer_casted;

		#if CFG_PROTECT_FROM_INVALID_HEAP_FREE == 1 /* TODO: fix weird #if */
		if (common_checkBlockValid(block)) {
			block->magic1 = 0;
			block->magic2 = 0;
			block->checksum = 0;
		#endif

			if (block->state != 0) {
				if (block->next == NULL) {
					if (block->allocListItem.data != NULL) {
						common_listDeleteAny(block->allocListItem.list, &(block->allocListItem));
						block->allocListItem.data = NULL;
					}
					block->state = 0;
					kFreeMemory += block->blockSize;
					common_insertFreeBlock((struct kMemoryBlockStruct_t *)block);
				}
			}

		#if CFG_PROTECT_FROM_INVALID_HEAP_FREE == 1 /* TODO: fix weird #if */
		}
		#endif
	}

	arch_exitCriticalSection();
	return;
}

void common_heapFreeSafe(void **pointer)
{
	arch_enterCriticalSection();

	common_heapFree(*pointer);
	*pointer = NULL;

	arch_exitCriticalSection();
	return;
}

kReturnValue_t common_heapPointerSanityCheck(void *pointer)
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