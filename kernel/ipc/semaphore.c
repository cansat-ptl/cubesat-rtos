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

void ipc_semaphoreInit(kSemaphoreHandle_t semaphore, kBaseType_t resourceAmount)
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

void ipc_semaphoreWait(kSemaphoreHandle_t semaphore)
{
	if (semaphore != NULL) {
		while (1) {
			arch_spinlockAcquire(&semaphore->spinlock);

			if (semaphore->lockCount != 0) {
				semaphore->lockCount--;

				if (semaphore->type == KLOCK_MUTEX) {
					semaphore->lockOwner = tasks_getCurrentTask();
					semaphore->basePriority = tasks_getTaskPriority(semaphore->lockOwner);
				}

				arch_spinlockRelease(&semaphore->spinlock);
				break;
			}
			else {
				kTaskHandle_t currentTask = tasks_getCurrentTask();

				if (semaphore->type == KLOCK_MUTEX) {
					if (tasks_getTaskPriority(semaphore->lockOwner) < tasks_getTaskPriority(currentTask)) {
						tasks_setTaskPriority(semaphore->lockOwner, tasks_getTaskPriority(currentTask));
					}
				}

				tasks_blockTask(currentTask, (kLinkedList_t*)&(semaphore->blockedTasks));
				arch_spinlockRelease(&semaphore->spinlock);
				tasks_sleep(0);
			}
		}
	}

	return;
}

/* TODO: lock tracking */
void ipc_semaphoreSignal(kSemaphoreHandle_t semaphore)
{
	if (semaphore != NULL) {
		arch_spinlockAcquire(&semaphore->spinlock);

		kLinkedListItem_t* temp = semaphore->blockedTasks.head;

		if (semaphore->type == KLOCK_MUTEX) {
			if (tasks_getTaskPriority(semaphore->lockOwner) != semaphore->basePriority) {
				tasks_setTaskPriority(semaphore->lockOwner, semaphore->basePriority);
				semaphore->lockOwner = NULL;
				semaphore->basePriority = 0;
			}
		}

		while(temp != NULL) {
			tasks_unblockTask((kTaskHandle_t)temp->data);
			temp = temp->next;
		}

		semaphore->lockCount++;

		arch_spinlockRelease(&semaphore->spinlock);
	}

	return;
}