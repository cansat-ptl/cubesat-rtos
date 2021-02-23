/*
 * fifo.h
 * 
 * Created: 23.02.2021 04:04:44
 * Author: ThePetrovich
 */


#ifndef KERNEL_FIFO_H_
#define KERNEL_FIFO_H_

#include <rtos/types.h>
#include <rtos/ipc/ipc.h>

typedef volatile struct kIPCStruct_t kFIFO_t;
typedef volatile struct kIPCStruct_t* kFIFOHandle_t;

void ipc_fifoInit(kFIFOHandle_t fifo, void* fifoBuffer, size_t bufferSize, size_t itemSize);

size_t ipc_fifoWrite(kFIFOHandle_t fifo, void* input);
size_t ipc_fifoRead(kFIFOHandle_t fifo, void* output);
size_t ipc_fifoPeek(kFIFOHandle_t fifo, void* output);
size_t ipc_fifoFreeSpace(kFIFOHandle_t fifo);
size_t ipc_fifoAvailable(kFIFOHandle_t fifo);

#endif