/*
 * semaphore.c
 * 
 * Created: 04.02.2021 07:50:35
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/ipc/semaphore.h>
#include <kernel/arch/arch.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>

void ipc_semaphoreInit(kSemaphore_t *semaphore, kLockType_t type, kBaseType_t resourceAmount)
{
	if (semaphore != NULL) {
		semaphore->type = type;
		semaphore->lockCount = resourceAmount;
		semaphore->blockedTasks.head = NULL;
		semaphore->blockedTasks.tail = NULL;
		semaphore->spinlock = 0;
	}
}

void ipc_semaphoreWait(kSemaphore_t *semaphore)
{	
	kTask_t *currentTask = NULL;

	if (semaphore != NULL && semaphore->type == KLOCK_SEMAPHORE) {
		while (1) {
			arch_enterCriticalSection();

			if (semaphore->lockCount > 0) {
				semaphore->lockCount--;

				arch_exitCriticalSection();
				break;
			}
			else {
				currentTask = tasks_getCurrentTask();

				tasks_blockTask(currentTask, (kLinkedList_t *)&(semaphore->blockedTasks));
				arch_exitCriticalSection();
				tasks_sleep(0);
			}
		}
	}

	return;
}

/* TODO: lock tracking */
void ipc_semaphoreSignal(kSemaphore_t *semaphore)
{		
	if (semaphore != NULL && semaphore->type == KLOCK_SEMAPHORE) {
		arch_enterCriticalSection();

		semaphore->lockCount++;

		if(semaphore->blockedTasks.head != NULL) {
			tasks_unblockTask((kTask_t *)semaphore->blockedTasks.head->data);
		}

		arch_exitCriticalSection();
	}

	return;
}