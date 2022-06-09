/*
 * lifo.c
 * 
 * Created: 23.02.2021 04:36:09
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/ipc/lifo.h>
#include <kernel/ipc/mutex.h>
#include <kernel/tasks/sched.h>
#include <kernel/common/string.h>
#include <kernel/arch/arch.h>

void ipc_lifoInit(kLIFO_t *lifo, void *lifoBuffer, size_t bufferSize, size_t itemSize)
{
	if (lifoBuffer != NULL && bufferSize >= itemSize) {
		lifo->itemSize = itemSize;
		lifo->bufferSize = bufferSize;
		lifo->pointer = lifoBuffer;
		lifo->inputPosition = 0;
		lifo->outputPosition = 0;
		lifo->currentPosition = 0;
	}
}

size_t ipc_lifoWrite(kLIFO_t *lifo, void *input)
{
	size_t bytesWritten = 0;
	size_t currentPositionPrev = 0;

	if (lifo != NULL) {
		if (ipc_lifoFreeSpace(lifo) != 0) {
			arch_enterAtomicSection();

			currentPositionPrev = lifo->currentPosition;

			lifo->currentPosition += lifo->itemSize;
			bytesWritten += lifo->itemSize;

			arch_exitAtomicSection();

			common_memcpy(lifo->pointer + currentPositionPrev, input, lifo->itemSize);
		}
	}

	return bytesWritten;
}

size_t ipc_lifoRead(kLIFO_t *lifo, void *output)
{
	size_t bytesRead = 0;
	size_t currentPositionPrev = 0;

	if (lifo != NULL) {
		if (ipc_lifoAvailable(lifo) != 0) {
			arch_enterAtomicSection();

			currentPositionPrev = lifo->currentPosition;

			lifo->currentPosition -= lifo->itemSize;
			bytesRead += lifo->itemSize;

			arch_exitAtomicSection();
			
			common_memcpy(output, lifo->pointer + currentPositionPrev - lifo->itemSize, lifo->itemSize);
		}
	}

	return bytesRead;
}

size_t ipc_lifoPeek(kLIFO_t *lifo, void *output)
{
	size_t bytesRead = 0;
	size_t currentPositionPrev = 0;

	if (lifo != NULL) {
		if (ipc_lifoAvailable(lifo) != 0) {
			arch_enterAtomicSection();

			currentPositionPrev = lifo->currentPosition;
			bytesRead = lifo->itemSize;

			arch_exitAtomicSection();

			common_memcpy(output, lifo->pointer + currentPositionPrev - lifo->itemSize, lifo->itemSize);
		}
	}

	return bytesRead;
}

size_t ipc_lifoFreeSpace(kLIFO_t *lifo)
{
	size_t freeSpace = 0;

	if (lifo != NULL) {
		arch_enterAtomicSection();

		if (lifo->bufferSize - lifo->currentPosition >= lifo->itemSize) {
			freeSpace = lifo->bufferSize - lifo->currentPosition;
		}

		arch_exitAtomicSection();
	}

	return freeSpace;
}

size_t ipc_lifoAvailable(kLIFO_t *lifo)
{
	size_t currentPosition = 0;

	if (lifo != NULL) {
		arch_enterAtomicSection();

		currentPosition = lifo->currentPosition;

		arch_exitAtomicSection();
	}

	return currentPosition;
}
