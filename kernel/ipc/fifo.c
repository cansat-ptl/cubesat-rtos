/*
 * fifo.c
 * 
 * Created: 23.02.2021 03:43:09
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/ipc/fifo.h>
#include <kernel/ipc/mutex.h>
#include <kernel/tasks/sched.h>
#include <kernel/common/string.h>
#include <kernel/arch/arch.h>

void ipc_fifoInit(kFIFO_t *fifo, void *fifoBuffer, size_t bufferSize, size_t itemSize)
{
	if (fifoBuffer != NULL && bufferSize >= itemSize) {
		fifo->itemSize = itemSize;
		fifo->bufferSize = bufferSize;
		fifo->pointer = fifoBuffer;
		fifo->inputPosition = 0;
		fifo->outputPosition = 0;
		fifo->currentPosition = 0;
	}
}

size_t ipc_fifoWrite(kFIFO_t *fifo, void *input)
{
	size_t bytesWritten = 0;
	size_t inputPositionPrev = 0;

	if (fifo != NULL) {
		if (ipc_fifoFreeSpace(fifo)) {
			arch_enterAtomicSection();
			
			inputPositionPrev = fifo->inputPosition;

			fifo->inputPosition += fifo->itemSize;

			if (fifo->inputPosition >= fifo->bufferSize) {
				fifo->inputPosition = 0;
			}

			fifo->currentPosition += fifo->itemSize;
			bytesWritten += fifo->itemSize;

			arch_exitAtomicSection();

			common_memcpy(fifo->pointer + inputPositionPrev, input, fifo->itemSize);
		}
	}

	return bytesWritten;
}

size_t ipc_fifoRead(kFIFO_t *fifo, void *output)
{
	size_t bytesRead = 0;
	size_t outputPositionPrev = 0;

	if (fifo != NULL) {
		if (ipc_fifoAvailable(fifo) != 0) {
			arch_enterAtomicSection();
			
			outputPositionPrev = fifo->outputPosition;

			fifo->outputPosition += fifo->itemSize;

			if (fifo->outputPosition >= fifo->bufferSize) {
				fifo->outputPosition = 0;
			}

			fifo->currentPosition -= fifo->itemSize;
			bytesRead += fifo->itemSize;

			arch_exitAtomicSection();

			common_memcpy(output, fifo->pointer + outputPositionPrev, fifo->itemSize);
		}
	}
	
	return bytesRead;
}

size_t ipc_fifoPeek(kFIFO_t *fifo, void *output)
{
	size_t bytesRead = 0;
	size_t outputPositionPrev = 0;

	if (fifo != NULL) {
		if (ipc_fifoAvailable(fifo) != 0) {
			arch_enterAtomicSection();

			outputPositionPrev = fifo->outputPosition;
			bytesRead = fifo->itemSize;

			arch_exitAtomicSection();

			common_memcpy(output, fifo->pointer + outputPositionPrev, fifo->itemSize);
		}
	}

	return bytesRead;
}

size_t ipc_fifoFreeSpace(kFIFO_t *fifo)
{
	size_t freeSpace = 0;

	if (fifo != NULL) {
		arch_enterAtomicSection();

		if (fifo->bufferSize - fifo->currentPosition >= fifo->itemSize) {
			freeSpace = (fifo->bufferSize - fifo->currentPosition);
		}

		arch_exitAtomicSection();
	}

	return freeSpace;
}


size_t ipc_fifoAvailable(kFIFO_t *fifo)
{
	size_t currentPosition = 0;

	if (fifo != NULL) {
		arch_enterAtomicSection();

		currentPosition = fifo->currentPosition;

		arch_exitAtomicSection();
	}

	return currentPosition;
}