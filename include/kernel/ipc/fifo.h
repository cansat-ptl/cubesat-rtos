/*
 * fifo.h
 * 
 * Created: 23.02.2021 04:04:44
 * Author: ThePetrovich
 */


#ifndef KERNEL_FIFO_H_
#define KERNEL_FIFO_H_

#include <kernel/types.h>
#include <kernel/ipc/ipc.h>
#include <kernel/ipc/mutex.h>

typedef volatile struct kIPCStruct_t kFIFO_t;

#ifdef __cplusplus
extern "C" {
#endif

void ipc_fifoInit(kFIFO_t *fifo, void *fifoBuffer, size_t bufferSize, size_t itemSize);

size_t ipc_fifoWrite(kFIFO_t *fifo, void *input);
size_t ipc_fifoRead(kFIFO_t *fifo, void *output);
size_t ipc_fifoPeek(kFIFO_t *fifo, void *output);
size_t ipc_fifoFreeSpace(kFIFO_t *fifo);
size_t ipc_fifoAvailable(kFIFO_t *fifo);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_FIFO_H_ */