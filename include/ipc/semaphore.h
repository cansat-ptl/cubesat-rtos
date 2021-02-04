/*
 * semaphore.h
 * 
 * Created: 04.02.2021 08:07:08
 * Author: ThePetrovich
 */


#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <types.h>
#include <common.h>
#include <ipc/ipc.h>

typedef volatile struct kLockStruct_t kSemaphore_t;
typedef volatile struct kLockStruct_t* kSemaphoreHandle_t;

void ipc_semaphoreInit(struct kLockStruct_t *semaphore, uint8_t resourceAmount);
void ipc_semaphoreWait(volatile struct kLockStruct_t* semaphore);
void ipc_semaphoreSignal(volatile struct kLockStruct_t* semaphore);

#endif