/*
 * fifo.c
 * 
 * Created: 23.02.2021 03:43:09
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/ipc/ipc.h>
#include <kernel/ipc/fifo.h>
#include <kernel/ipc/mutex.h>
#include <kernel/tasks/sched.h>
#include <kernel/common/string.h>

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

	if (fifo != NULL) {
		if (ipc_fifoFreeSpace(fifo)) {
			common_memcpy(fifo->pointer + fifo->inputPosition, input, fifo->itemSize);

			fifo->inputPosition += fifo->itemSize;

			if (fifo->inputPosition >= fifo->bufferSize) {
				fifo->inputPosition = 0;
			}

			fifo->currentPosition += fifo->itemSize;
			bytesWritten += fifo->itemSize;
		}
	}

	return bytesWritten;
}

size_t ipc_fifoRead(kFIFO_t *fifo, void *output)
{
	size_t bytesRead = 0;

	if (fifo != NULL) {
		if (ipc_fifoAvailable(fifo) != 0) {
			common_memcpy(output, fifo->pointer + fifo->outputPosition, fifo->itemSize);

			fifo->outputPosition += fifo->itemSize;

			if (fifo->outputPosition >= fifo->bufferSize) {
				fifo->outputPosition = 0;
			}

			fifo->currentPosition -= fifo->itemSize;
			bytesRead += fifo->itemSize;
		}
	}
	
	return bytesRead;
}

size_t ipc_fifoPeek(kFIFO_t *fifo, void *output)
{
	size_t bytesRead = 0;

	if (fifo != NULL) {
		if (ipc_fifoAvailable(fifo) != 0) {
			common_memcpy(output, fifo->pointer + fifo->outputPosition, fifo->itemSize);
		}
	}

	return bytesRead;
}

size_t ipc_fifoFreeSpace(kFIFO_t *fifo)
{
	if (fifo->bufferSize - fifo->currentPosition < fifo->itemSize) {
		return 0;
	}		
	else {
		return 1;
	}
}


size_t ipc_fifoAvailable(kFIFO_t *fifo)
{
	return fifo->currentPosition;
}