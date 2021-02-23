/*
 * mutex.c
 * 
 * Created: 04.02.2021 07:50:40
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/ipc/ipc.h>
#include <kernel/ipc/mutex.h>
#include <kernel/ipc/semaphore.h>

void ipc_mutexInit(kMutexHandle_t mutex)
{
	ipc_semaphoreInit(mutex, 1);
	mutex->type = KLOCK_MUTEX;
}

void ipc_mutexLock(kMutexHandle_t mutex)
{
	ipc_semaphoreWait(mutex);
}

void ipc_mutexUnlock(kMutexHandle_t mutex)
{
	ipc_semaphoreSignal(mutex);
}