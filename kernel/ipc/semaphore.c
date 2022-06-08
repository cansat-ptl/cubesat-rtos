/*
 * semaphore.c
 * 
 * Created: 04.02.2021 07:50:35
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/ipc/ipc.h>
#include <kernel/ipc/semaphore.h>
#include <kernel/arch/arch.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>

void ipc_semaphoreInit(kSemaphore_t *semaphore, kBaseType_t resourceAmount)
{
	if (semaphore != NULL) {
		semaphore->type = KLOCK_SEMAPHORE;
		semaphore->lockCount = resourceAmount;
		semaphore->basePriority = 0;
		semaphore->blockedTasks.head = NULL;
		semaphore->blockedTasks.tail = NULL;
		semaphore->lockOwner = NULL;
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
	kLinkedListItem_t *head = NULL;
	
	if (semaphore != NULL && semaphore->type == KLOCK_SEMAPHORE) {
		arch_enterCriticalSection();

		kTask_t *currentTask = tasks_getCurrentTask();
		head = semaphore->blockedTasks.head;

		semaphore->lockCount++;

		if(head != NULL) {
			tasks_unblockTask((kTask_t *)head->data);
		}

		arch_exitCriticalSection();
	}

	return;
}