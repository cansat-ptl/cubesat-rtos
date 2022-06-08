/*
 * lifo.h
 * 
 * Created: 23.02.2021 04:36:31
 * Author: ThePetrovich
 */


#ifndef KERNEL_LIFO_H_
#define KERNEL_LIFO_H_

#include <kernel/types.h>

typedef volatile struct kLIFOStruct_t
{
	void *pointer;

	size_t itemSize;
	size_t bufferSize;

	size_t inputPosition;
	size_t outputPosition;
	size_t currentPosition;
} kLIFO_t;

#ifdef __cplusplus
extern "C" {
#endif

void ipc_lifoInit(kLIFO_t *lifo, void *lifoBuffer, size_t bufferSize, size_t itemSize);

size_t ipc_lifoWrite(kLIFO_t *lifo, void *input);
size_t ipc_lifoRead(kLIFO_t *lifo, void *output);
size_t ipc_lifoPeek(kLIFO_t *lifo, void *output);
size_t ipc_lifoFreeSpace(kLIFO_t *lifo);
size_t ipc_lifoAvailable(kLIFO_t *lifo);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_LIFO_H_ */