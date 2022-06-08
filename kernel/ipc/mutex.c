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

void ipc_mutexInit(kMutex_t *mutex)
{
	if (mutex != NULL) {
		mutex->type = KLOCK_MUTEX;
		mutex->lockCount = 1;
		mutex->basePriority = 0;
		mutex->blockedTasks.head = NULL;
		mutex->blockedTasks.tail = NULL;
		mutex->lockOwner = NULL;
		mutex->spinlock = 0;
	}
}

void ipc_mutexLock(kMutex_t *mutex)
{
	kTask_t *currentTask = NULL;

	if (mutex != NULL && mutex->type == KLOCK_MUTEX) {
		while (1) {
			arch_enterCriticalSection();

			if (mutex->lockCount > 0) {
				mutex->lockCount--;

				mutex->lockOwner = tasks_getCurrentTask();
				mutex->basePriority = tasks_getTaskPriority(mutex->lockOwner);

				kBaseType_t mutexCount = tasks_getHeldMutexCount(mutex->lockOwner);
				tasks_setHeldMutexCount(mutex->lockOwner, mutexCount + 1);


				arch_exitCriticalSection();
				break;
			}
			else {
				currentTask = tasks_getCurrentTask();

				if (mutex->lockOwner  == NULL || tasks_getTaskState(mutex->lockOwner) == KSTATE_UNINIT) {
					if(mutex->blockedTasks.head != NULL) {
						tasks_unblockTask((kTask_t *)mutex->blockedTasks.head->data);
					}
					mutex->lockCount = 1;
				}
				else {						
					if (tasks_getTaskPriority(mutex->lockOwner) < tasks_getTaskPriority(currentTask)) {
						tasks_setTaskPriority(mutex->lockOwner, tasks_getTaskPriority(currentTask));
					}
				}

				tasks_blockTask(currentTask, (kLinkedList_t *)&(mutex->blockedTasks));
				arch_exitCriticalSection();
				tasks_sleep(0);
			}
		}
	}

	return;
}

void ipc_mutexUnlock(kMutex_t *mutex)
{
		kLinkedListItem_t *head = NULL;
	
	if (mutex != NULL && mutex->type == KLOCK_MUTEX) {
		arch_enterCriticalSection();

		kTask_t *currentTask = tasks_getCurrentTask();
		head = mutex->blockedTasks.head;

		if (currentTask == mutex->lockOwner) {
			kBaseType_t mutexCount = tasks_getHeldMutexCount(currentTask);
			if (mutexCount > 0) {
				tasks_setHeldMutexCount(currentTask, mutexCount - 1);
			}

			if (tasks_getTaskPriority(mutex->lockOwner) != mutex->basePriority) {
				tasks_setTaskPriority(mutex->lockOwner, mutex->basePriority);
				mutex->lockOwner = NULL;
				mutex->basePriority = 0;
			}

			mutex->lockCount = 1;
		}

		if(head != NULL) {
			tasks_unblockTask((kTask_t *)head->data);
		}

		arch_exitCriticalSection();
	}

	return;
}