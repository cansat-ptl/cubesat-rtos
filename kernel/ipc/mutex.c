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
#include <tasks/tasks.h>
#include <tasks/dispatcher.h>
#include <arch/arch.h>

void ipc_mutexInit(struct kLockStruct_t *mutex)
{
	ipc_semaphoreInit(mutex, 1);
	mutex->type = KLOCK_MUTEX;
}

void ipc_mutexLock(volatile struct kLockStruct_t* mutex)
{
	ipc_semaphoreWait(mutex);
}

void ipc_mutexUnlock(volatile struct kLockStruct_t* mutex)
{
	ipc_semaphoreSignal(mutex);
}