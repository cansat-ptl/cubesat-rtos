/*
 * semaphore.h
 * 
 * Created: 04.02.2021 08:07:08
 * Author: ThePetrovich
 */


#ifndef KERNEL_SEMAPHORE_H_
#define KERNEL_SEMAPHORE_H_

#include <rtos/types.h>
#include <rtos/ipc/ipc.h>

typedef volatile struct kLockStruct_t kSemaphore_t;
typedef volatile struct kLockStruct_t* kSemaphoreHandle_t;

void ipc_semaphoreInit(kSemaphoreHandle_t semaphore, uint8_t resourceAmount);
void ipc_semaphoreWait(kSemaphoreHandle_t semaphore);
void ipc_semaphoreSignal(kSemaphoreHandle_t semaphore);

#endif