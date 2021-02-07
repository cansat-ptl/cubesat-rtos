/*
 * mutex.c
 * 
 * Created: 04.02.2021 07:50:40
 * Author: ThePetrovich
 */


#include <types.h>
#include <config.h>
#include <common.h>
#include <ipc/ipc.h>
#include <ipc/mutex.h>
#include <ipc/semaphore.h>

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