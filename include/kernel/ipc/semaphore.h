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

#ifdef __cplusplus
extern "C" {
#endif

void ipc_semaphoreInit(kSemaphore_t *semaphore, kBaseType_t resourceAmount);
void ipc_semaphoreWait(kSemaphore_t *semaphore);
void ipc_semaphoreSignal(kSemaphore_t *semaphore);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_SEMAPHORE_H_ */