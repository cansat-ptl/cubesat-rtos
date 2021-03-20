/*
 * mutex.h
 * 
 * Created: 04.02.2021 08:07:14
 * Author: ThePetrovich
 */


#ifndef KERNEL_MUTEX_H_
#define KERNEL_MUTEX_H_

#include <kernel/types.h>
#include <kernel/ipc/ipc.h>

typedef volatile struct kLockStruct_t kMutex_t;

void ipc_mutexInit(kMutex_t *mutex);
void ipc_mutexLock(kMutex_t *mutex);
void ipc_mutexUnlock(kMutex_t *mutex);
#endif