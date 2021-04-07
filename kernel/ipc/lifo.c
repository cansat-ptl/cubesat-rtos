/*
 * lifo.c
 * 
 * Created: 23.02.2021 04:36:09
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/ipc/ipc.h>
#include <kernel/ipc/lifo.h>
#include <kernel/ipc/fifo.h>
#include <kernel/ipc/mutex.h>
#include <kernel/tasks/sched.h>
#include <kernel/common/string.h>

void ipc_lifoInit(kLIFO_t *lifo, void *lifoBuffer, size_t bufferSize, size_t itemSize, kMutex_t *mutex)
{
	ipc_fifoInit((kLIFO_t *)lifo, lifoBuffer, bufferSize, itemSize, mutex);
}

size_t ipc_lifoWrite(kLIFO_t *lifo, void *input)
{
	size_t bytesWritten = 0;

	if (lifo != NULL) {
		ipc_mutexLock(lifo->mutex);

		if (ipc_lifoFreeSpace(lifo) != 0) {
			common_memcpy(lifo->pointer + lifo->currentPosition, input, lifo->itemSize);
			
			lifo->currentPosition += lifo->itemSize;
			bytesWritten += lifo->itemSize;
		}

		ipc_mutexUnlock(lifo->mutex);
	}

	return bytesWritten;
}

size_t ipc_lifoWriteBlocking(kLIFO_t *lifo, void *input)
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

size_t ipc_lifoRead(kLIFO_t *lifo, void *output)
{
	size_t bytesRead = 0;

	if (lifo != NULL) {
		ipc_mutexLock(lifo->mutex);

		if (ipc_lifoAvailable(lifo) != 0) {
			common_memcpy(output, lifo->pointer + lifo->currentPosition - lifo->itemSize, lifo->itemSize);
			
			lifo->currentPosition -= lifo->itemSize;
			bytesRead += lifo->itemSize;
		}

		ipc_mutexLock(lifo->mutex);
	}

	return bytesRead;
}

size_t ipc_lifoReadBlocking(kLIFO_t *lifo, void *output)
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

size_t ipc_lifoPeek(kLIFO_t *lifo, void *output)
{
	size_t bytesRead = 0;

	if (lifo != NULL) {
		if (ipc_fifoAvailable(lifo) != 0) {
			common_memcpy(output, lifo->pointer + lifo->currentPosition - lifo->itemSize, lifo->itemSize);
			bytesRead += lifo->itemSize;
		}
	}

	return bytesRead;
}

size_t ipc_lifoFreeSpace(kLIFO_t *lifo)
{
	if (lifo->bufferSize - lifo->currentPosition >= lifo->itemSize) {
		return lifo->bufferSize - lifo->currentPosition;
	}
	else {
		return 0;
	}
}

size_t ipc_lifoAvailable(kLIFO_t *lifo)
{
	return lifo->currentPosition;
}
