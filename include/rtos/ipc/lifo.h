/*
 * fifo.h
 * 
 * Created: 23.02.2021 04:04:44
 * Author: ThePetrovich
 */


#ifndef KERNEL_LIFO_H_
#define KERNEL_LIFO_H_

#include <rtos/types.h>
#include <rtos/ipc/ipc.h>
#include <rtos/ipc/mutex.h>

typedef volatile struct kIPCStruct_t kLIFO_t;
typedef volatile struct kIPCStruct_t* kLIFOHandle_t;

void ipc_lifoInit(kLIFOHandle_t lifo, void* lifoBuffer, size_t bufferSize, size_t itemSize, kMutexHandle_t mutex);

size_t ipc_lifoWrite(kLIFOHandle_t lifo, void* input);
size_t ipc_lifoWriteBlocking(kLIFOHandle_t lifo, void* input);
size_t ipc_lifoRead(kLIFOHandle_t lifo, void* output);
size_t ipc_lifoReadBlocking(kLIFOHandle_t lifo, void* output);
size_t ipc_lifoPeek(kLIFOHandle_t lifo, void* output);
size_t ipc_lifoFreeSpace(kLIFOHandle_t lifo);
size_t ipc_lifoAvailable(kLIFOHandle_t lifo);

#endif