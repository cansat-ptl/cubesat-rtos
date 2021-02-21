/*
 * mutex.c
 * 
 * Created: 04.02.2021 07:50:40
 * Author: ThePetrovich
 */


#include <rtos/types.h>
#include <rtos/config.h>
#include <rtos/ipc/ipc.h>
#include <rtos/ipc/mutex.h>
#include <rtos/ipc/semaphore.h>

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