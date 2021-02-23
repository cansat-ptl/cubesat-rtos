/*
 * semaphore.h
 * 
 * Created: 04.02.2021 08:07:08
 * Author: ThePetrovich
 */


#ifndef KERNEL_SEMAPHORE_H_
#define KERNEL_SEMAPHORE_H_

#include <kernel/types.h>
#include <kernel/ipc/ipc.h>

typedef volatile struct kLockStruct_t kSemaphore_t;
typedef volatile struct kLockStruct_t* kSemaphoreHandle_t;

void ipc_semaphoreInit(kSemaphoreHandle_t semaphore, kBaseType_t resourceAmount);
void ipc_semaphoreWait(kSemaphoreHandle_t semaphore);
void ipc_semaphoreSignal(kSemaphoreHandle_t semaphore);

#endif