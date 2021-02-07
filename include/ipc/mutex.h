/*
 * mutex.h
 * 
 * Created: 04.02.2021 08:07:14
 * Author: ThePetrovich
 */


#ifndef MUTEX_H_
#define MUTEX_H_

#include <types.h>
#include <common.h>
#include <ipc/ipc.h>

typedef volatile struct kLockStruct_t kMutex_t;
typedef volatile struct kLockStruct_t* kMutexHandle_t;

void ipc_mutexInit(kMutexHandle_t mutex);
void ipc_mutexLock(kMutexHandle_t mutex);
void ipc_mutexUnlock(kMutexHandle_t mutex);
#endif