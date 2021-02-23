/*
 * lifo.c
 * 
 * Created: 23.02.2021 04:36:09
 * Author: ThePetrovich
 */


#include <rtos/types.h>
#include <rtos/config.h>
#include <rtos/ipc/ipc.h>
#include <rtos/ipc/lifo.h>
#include <rtos/ipc/fifo.h>
#include <rtos/tasks/tasks.h>
#include <rtos/tasks/sched.h>
#include <string.h> /* TODO: memcpy */

void ipc_lifoInit(kLIFOHandle_t lifo, void* lifoBuffer, size_t bufferSize, size_t itemSize)
{
	ipc_fifoInit((kFIFOHandle_t)lifo, lifoBuffer, bufferSize, itemSize);
}

size_t ipc_lifoWrite(kLIFOHandle_t lifo, void* input)
{
    size_t bytesWritten = 0;

	if (lifo != NULL) {
        ipc_mutexLock(&(lifo->mutex));

        if (ipc_lifoFreeSpace(lifo) != 0) {
            memcpy(lifo->pointer + lifo->currentPosition, input, lifo->itemSize);
            
            lifo->currentPosition += lifo->itemSize;
            bytesWritten += lifo->itemSize;
        }

        ipc_mutexUnlock(&(lifo->mutex));
	}

	return bytesWritten;
}

size_t ipc_lifoWriteBlocking(kLIFOHandle_t lifo, void* input)
{
    size_t bytesWritten = 0;

	if (lifo != NULL) {
        while (1) {
			bytesWritten = ipc_fifoRead(lifo, input);
			if (bytesWritten != 0) {
				break;
			}
        }
	}

	return bytesWritten;
}

size_t ipc_lifoRead(kLIFOHandle_t lifo, void* output)
{
	size_t bytesRead = 0;

	if (lifo != NULL) {
        ipc_mutexLock(&(lifo->mutex));

        if (ipc_lifoAvailable(lifo) != 0) {
            memcpy(output, lifo->pointer + lifo->currentPosition - lifo->itemSize, lifo->itemSize);
            
            lifo->currentPosition -= lifo->itemSize;
            bytesRead += lifo->itemSize;
        }

        ipc_mutexLock(&(lifo->mutex));
	}

	return bytesRead;
}

size_t ipc_lifoReadBlocking(kLIFOHandle_t lifo, void* output)
{
	size_t bytesRead = 0;

	if (lifo != NULL) {
        while (1) {
			bytesRead = ipc_fifoRead(lifo, output);
			if (bytesRead != 0) {
				break;
			}
        }
	}

	return bytesRead;
}

size_t ipc_lifoPeek(kLIFOHandle_t lifo, void* output)
{
	size_t bytesRead = 0;

	if (lifo != NULL) {
		if (ipc_fifoAvailable(lifo) != 0) {
			memcpy(output, lifo->pointer + lifo->currentPosition - lifo->itemSize, lifo->itemSize);
            bytesRead += lifo->itemSize;
		}
	}

	return bytesRead;
}

size_t ipc_lifoFreeSpace(kLIFOHandle_t lifo)
{
	if (lifo->bufferSize - lifo->currentPosition >= lifo->itemSize)
		return lifo->bufferSize - lifo->currentPosition;
	else
		return 0;
}

size_t ipc_lifoAvailable(kLIFOHandle_t lifo)
{
	return lifo->currentPosition;
}
