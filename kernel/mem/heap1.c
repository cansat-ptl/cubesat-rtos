/*
 * heap1.c
 *
 * Created: 18.12.2020 6:21:51
 *  Author: Admin
 */


#include <types.h>
#include <config.h>
#include <common.h>
#include <arch/arch.h>
#include <mem/heap.h>

static byte kHeapRegion[CFG_HEAP_SIZE];

static const size_t kHeapStructSize	= (sizeof(struct kMemoryBlockStruct_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK); //What the hell is this I shouldn't have copied FreeRTOS code

static struct kMemoryBlockStruct_t kHeapStart;
static struct kMemoryBlockStruct_t* kHeapEnd;

static size_t kFreeMemory = 0;
static size_t kMinimumFreeMemory = 0;

size_t memory_getFreeHeap()
{
	return kFreeMemory;
}

size_t memory_getFreeHeapMin()
{
	return kMinimumFreeMemory;
}

void memory_heapInit()
{
	struct kMemoryBlockStruct_t* firstFreeBlock;
	byte* heapAligned;
	size_t heapAddress;
	size_t heapSize = CFG_HEAP_SIZE;

	heapAddress = (size_t)kHeapRegion;

	if ((heapAddress & CFG_PLATFORM_BYTE_ALIGNMENT_MASK) != 0) { //-V547
		heapAddress += (CFG_PLATFORM_BYTE_ALIGNMENT - 1);
		heapAddress &= ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK);
		heapSize -= heapAddress - (size_t)kHeapRegion;
	}

	heapAligned = (byte*)heapAddress;

	kHeapStart.next = (void*)heapAligned;
	kHeapStart.blockSize = (size_t)0;
	kHeapStart.state = 0;

	heapAddress = ((size_t)heapAligned) + heapSize;
	heapAddress -= kHeapStructSize;
	heapAddress &= ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK);

	kHeapEnd = (void*)heapAddress;
	kHeapEnd -> blockSize = 0;
	kHeapEnd -> next = NULL;
	kHeapEnd -> state = 0;

	firstFreeBlock = (void*)heapAligned;
	firstFreeBlock -> blockSize = heapAddress - (size_t)firstFreeBlock;
	firstFreeBlock -> next = kHeapEnd;
	firstFreeBlock -> state = 0;

	kMinimumFreeMemory = firstFreeBlock -> blockSize;
	kFreeMemory = firstFreeBlock -> blockSize;
}

static void memory_insertFreeBlock(struct kMemoryBlockStruct_t* blockToInsert)
{
	struct kMemoryBlockStruct_t* blockIterator;
	byte* pointer_casted;

	for (blockIterator = &kHeapStart; blockIterator -> next < blockToInsert; blockIterator = blockIterator -> next) {;} //What

	pointer_casted = (byte*)blockIterator;

	if ((pointer_casted + blockIterator -> blockSize) == (byte*)blockToInsert) {
		blockIterator -> blockSize += blockToInsert -> blockSize;
		blockToInsert = blockIterator;
	}

	pointer_casted = (byte*)blockToInsert;

	if ((pointer_casted + blockToInsert -> blockSize) == (byte*)blockIterator -> next) {
		if (blockIterator -> next != kHeapEnd) {
			blockToInsert -> blockSize += blockIterator -> next -> blockSize;
			blockToInsert -> next = blockIterator -> next -> next;
		}
		else {
			blockToInsert -> next = kHeapEnd;
		}
	}
	else {
		blockToInsert -> next = blockIterator -> next;
	}

	if(blockIterator != blockToInsert) {
		blockIterator -> next = blockToInsert;
	}

	return;
}

void* memory_heapAlloc(size_t size)
{
	void* returnAddress = NULL;
	struct kMemoryBlockStruct_t *block, *newBlock, *previousBlock;

	kStatusRegister_t sreg = arch_startAtomicOperation();

	if (size > 0 && (size & CFG_PLATFORM_BYTE_ALIGNMENT_MASK) != 0x00) { //-V560
		size += (CFG_PLATFORM_BYTE_ALIGNMENT - (size & CFG_PLATFORM_BYTE_ALIGNMENT_MASK));
	}

	if (size > 0) size += kHeapStructSize;

	if (size > 0 && size <= kFreeMemory) {
		previousBlock = &kHeapStart;
		block = kHeapStart.next;

		while ((block -> blockSize < size) && (block -> next != NULL)) {
			previousBlock = block;
			block = block -> next;
		}

		if (block != kHeapEnd) {
			returnAddress = (void*)(((byte*)previousBlock -> next) + kHeapStructSize); //Parenthesis hell

			previousBlock -> next = block -> next;

			if ((block -> blockSize - size) > CFG_MIN_BLOCK_SIZE) {
				newBlock = (void*)(((byte*)block) + size);

				newBlock -> blockSize = block -> blockSize - size;
				block -> blockSize = size;

				memory_insertFreeBlock(newBlock);
			}

			kFreeMemory -= block -> blockSize;

			if(kFreeMemory < kMinimumFreeMemory) {
				kMinimumFreeMemory = kFreeMemory;
			}

			block -> state = 1;
			block -> next = NULL;
		}
	}

	arch_endAtomicOperation(sreg);
	return returnAddress;
}

void memory_heapFree(void* pointer)
{
	byte* pointer_casted = (byte*)pointer;
	struct kMemoryBlockStruct_t* block;

	kStatusRegister_t sreg = arch_startAtomicOperation();

	if (memory_heapPointerSanityCheck(pointer) == KRESULT_SUCCESS) {
		pointer_casted -= kHeapStructSize;

		block = (void*)pointer_casted;
		if (block -> state != 0) {
			if (block -> next == NULL) {
				block -> state = 0;
				kFreeMemory += block -> blockSize;
				memory_insertFreeBlock((struct kMemoryBlockStruct_t*)block);
			}
		}
	}
	arch_endAtomicOperation(sreg);
	return;
}

kReturnValue_t memory_heapPointerSanityCheck(void* pointer)
{
	kReturnValue_t exitcode = KRESULT_ERR_MEMORY_VIOLATION;
	if (pointer != NULL) {
		if (pointer >= (void*)kHeapRegion) {
			if (pointer <= (void*)(kHeapRegion + CFG_HEAP_SIZE-1)) {
				exitcode = KRESULT_SUCCESS;
			}
		}
	}
	return exitcode;
}