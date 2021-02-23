/*
 * fifo.c
 * 
 * Created: 23.02.2021 03:43:09
 * Author: ThePetrovich
 */


#include <rtos/types.h>
#include <rtos/config.h>
#include <rtos/ipc/ipc.h>
#include <rtos/ipc/fifo.h>
#include <string.h> /* TODO: memcpy */

/* TODO: proper architecture-independent structure size calculation */
void ipc_fifoInit(kFIFOHandle_t fifo, void* fifoBuffer, size_t bufferSize, size_t itemSize)
{
	if (fifoBuffer != NULL && bufferSize >= itemSize) {
        fifo = (kFIFOHandle_t)fifoBuffer;
        fifo->itemSize = itemSize;
        fifo->bufferSize = bufferSize;
        fifo->pointer = fifoBuffer;
        fifo->inputPosition = 0;
        fifo->outputPosition = 0;
        fifo->currentPosition = 0;
	}
}

/* TODO: blocking FIFO writes & reads */
size_t ipc_fifoWrite(kFIFOHandle_t fifo, void* input)
{
	size_t bytesWritten = 0;

	if (fifo != NULL) {
		if (ipc_fifoFreeSpace(fifo)) {
			memcpy(fifo->pointer + fifo->inputPosition, input, fifo->itemSize);

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

size_t ipc_fifoRead(kFIFOHandle_t fifo, void* output)
{
	size_t bytesRead = 0;

	if (fifo != NULL) {
		if (ipc_fifoAvailable(fifo) != 0) {
			memcpy(output, fifo->pointer + fifo->outputPosition, fifo->itemSize);

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

size_t ipc_fifoPeek(kFIFOHandle_t fifo, void* output)
{
    size_t bytesRead = 0;

	if (fifo != NULL) {
		if (ipc_fifoAvailable(fifo) != 0) {
			memcpy(output, fifo->pointer + fifo->outputPosition, fifo->itemSize);
		}
	}

	return bytesRead;
}

size_t ipc_fifoFreeSpace(kFIFOHandle_t fifo)
{
	if (fifo->currentPosition >= fifo->bufferSize) /* what */
		return 0;
	else
		return 1;
}


size_t ipc_fifoAvailable(kFIFOHandle_t fifo)
{
	return fifo->currentPosition;
}