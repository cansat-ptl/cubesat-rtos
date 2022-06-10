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
	size_t currentPositionPrev = 0;

	/* I will try my best to explain what is going on here */
	if (fifo != NULL) {
		do {
			/* Here we atomically check for free space in FIFO */
 			arch_enterAtomicSection();

			/* We need this do {...} while(0) weirdness because interrupts or 
			 * context switches can occur between exiting ipc_fifoFreeSpace
			 * and entering the next atomic section */
			if (!ipc_fifoFreeSpace(fifo)) {
				arch_exitAtomicSection();
				break;
			}
			
			/* While in atomic section, we save the current inputPosition for
			 * the following non-atomic memcpy */
			inputPositionPrev = fifo->inputPosition;

			/* We also have to update FIFO write position early, so that two
			 * threads will not attempt to write at the same memory location */
			fifo->inputPosition += fifo->itemSize;

			if (fifo->inputPosition >= fifo->bufferSize) {
				fifo->inputPosition = 0;
			}
			
			/* Now the stupid part */
			/* We save the current position and then set it to zero, to mark this
			 * FIFO as write-in-progress */
			/* This makes sure ipc_fifoRead and ipc_fifoPeek will fail and not
			 * perform an incomplete read */
			currentPositionPrev = fifo->currentPosition;

			/* This will certainly cause issuses later and is not a good solution,
			 * but ig not reading /sometimes/ is better than reading garbage */
			fifo->currentPosition = 0;

			/* We can now safely(?) exit the atomic section and do memcpy */
			arch_exitAtomicSection();

			common_memcpy(fifo->pointer + inputPositionPrev, input, fifo->itemSize);
			bytesWritten += fifo->itemSize;

			arch_enterAtomicSection();

			/* Now we restore and update the previously stored position to mark 
			 * this FIFO as available for reading */
			fifo->currentPosition = currentPositionPrev + fifo->itemSize;

			arch_exitAtomicSection();
		} while (0);
	}

	return bytesWritten;
}

size_t ipc_fifoRead(kFIFO_t *fifo, void *output)
{
	size_t bytesRead = 0;
	size_t outputPositionPrev = 0;

	if (fifo != NULL) {
		do {
			/* Perform the same do {...} while(0) trick */
			arch_enterAtomicSection();
			
			/* Incomplete write makes ipc_fifoAvailable to FAIL! */
			if (!ipc_fifoAvailable(fifo)) {
				arch_exitAtomicSection();
				break;
			}
			
			/* Save the current outputPosition for non-atomic memcpy */
			outputPositionPrev = fifo->outputPosition;
			
			/* Update outputPosition early so two concurrent reads will not
			 * read the same location */
			fifo->outputPosition += fifo->itemSize;

			if (fifo->outputPosition >= fifo->bufferSize) {
				fifo->outputPosition = 0;
			}

			arch_exitAtomicSection();

			/* Do the non-atomic memcpy */
			common_memcpy(output, fifo->pointer + outputPositionPrev, fifo->itemSize);
			bytesRead += fifo->itemSize;

			arch_enterAtomicSection();

			/* Update current position after the read to ensure ipc_fifoFreeSpace
			 * does not change state before memcpy completes */
			fifo->currentPosition -= fifo->itemSize;

			arch_exitAtomicSection();
		} while (0);
	}
	
	return bytesRead;
}

size_t ipc_fifoPeek(kFIFO_t *fifo, void *output)
{
	size_t bytesRead = 0;
	size_t outputPositionPrev = 0;

	/* Atomic peek is much easier to implement as it does not
	 * modify FIFO positions */
	if (fifo != NULL) {
		do {
			/* Perform the same do {...} while(0) trick */
			arch_enterAtomicSection();

			/* Incomplete write makes ipc_fifoAvailable to FAIL! */
			if (!ipc_fifoAvailable(fifo)) {
				arch_exitAtomicSection();
				break;
			}
			
			/* Save the current outputPosition for non-atomic memcpy */
			outputPositionPrev = fifo->outputPosition;

			arch_exitAtomicSection();

			common_memcpy(output, fifo->pointer + outputPositionPrev, fifo->itemSize);
			bytesRead = fifo->itemSize;
		} while (0);
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