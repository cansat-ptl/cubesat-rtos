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

	if (semaphore != NULL) {
		while (1) {
			arch_enterCriticalSection();

			if (semaphore->lockCount > 0) {
				semaphore->lockCount--;

				if (semaphore->type == KLOCK_MUTEX) {
					semaphore->lockOwner = tasks_getCurrentTask();
					semaphore->basePriority = tasks_getTaskPriority(semaphore->lockOwner);

					kBaseType_t mutexCount = tasks_getHeldMutexCount(semaphore->lockOwner);
					tasks_setHeldMutexCount(semaphore->lockOwner, mutexCount + 1);
				}

				arch_exitCriticalSection();
				break;
			}
			else {
				currentTask = tasks_getCurrentTask();

				if (semaphore->type == KLOCK_MUTEX) {
					if (semaphore->lockOwner  == NULL || tasks_getTaskState(semaphore->lockOwner) == KSTATE_UNINIT) {
						if(semaphore->blockedTasks.head != NULL) {
							tasks_unblockTask((kTask_t *)semaphore->blockedTasks.head->data);
						}
						semaphore->lockCount = 1;
					}
					else {						
						if (tasks_getTaskPriority(semaphore->lockOwner) < tasks_getTaskPriority(currentTask)) {
							tasks_setTaskPriority(semaphore->lockOwner, tasks_getTaskPriority(currentTask));
						}
					}
				}

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
	
	if (semaphore != NULL) {
		arch_enterCriticalSection();

		kTask_t *currentTask = tasks_getCurrentTask();
		head = semaphore->blockedTasks.head;

		if (semaphore->type == KLOCK_MUTEX) {
			if (currentTask == semaphore->lockOwner) {
				kBaseType_t mutexCount = tasks_getHeldMutexCount(currentTask);
				if (mutexCount > 0) {
					tasks_setHeldMutexCount(currentTask, mutexCount - 1);
				}

				if (tasks_getTaskPriority(semaphore->lockOwner) != semaphore->basePriority) {
					tasks_setTaskPriority(semaphore->lockOwner, semaphore->basePriority);
					semaphore->lockOwner = NULL;
					semaphore->basePriority = 0;
				}

				semaphore->lockCount = 1;
			}
		}
		else {
			semaphore->lockCount++;
		}

		if(head != NULL) {
			tasks_unblockTask((kTask_t *)head->data);
		}

		arch_exitCriticalSection();
	}

	return;
}