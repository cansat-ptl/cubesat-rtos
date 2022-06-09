/*
 * mutex.c
 * 
 * Created: 04.02.2021 07:50:40
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/ipc/mutex.h>
#include <kernel/arch/arch.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>
#include <kernel/panic.h>

void ipc_mutexInit(kMutex_t *mutex, kLockType_t type)
{
	if (mutex != NULL) {
		mutex->type = type;
		mutex->lockCount = 0;
		mutex->blockedTasks.head = NULL;
		mutex->blockedTasks.tail = NULL;
		mutex->lockOwner = NULL;
		mutex->spinlock = 0;
		
		#if CFG_ENABLE_MUTEX_PRIORITY_INHERITANCE == 1
			mutex->basePriority = 0;
		#endif
	}
}

void ipc_mutexLock(kMutex_t *mutex)
{
	kTask_t *currentTask = NULL;
	kBaseType_t priority = 0;

	if (mutex != NULL && (mutex->type == KLOCK_MUTEX || mutex->type == KLOCK_MUTEX_RECURSIVE)) {
		while (1) {
			arch_enterCriticalSection();

			if (mutex->lockCount == 0) {
				mutex->lockCount++;
				mutex->lockOwner = tasks_getCurrentTask();

				#if CFG_ENABLE_MUTEX_PRIORITY_INHERITANCE == 1
					priority = tasks_getTaskPriority(mutex->lockOwner);

					if (priority >= mutex->basePriority) {
						mutex->basePriority = priority;
					}
				#endif

				kBaseType_t mutexCount = tasks_getTaskHeldMutexCount(mutex->lockOwner);
				tasks_setTaskHeldMutexCount(mutex->lockOwner, mutexCount + 1);

				arch_exitCriticalSection();
				break;
			}
			else {
				currentTask = tasks_getCurrentTask();

				if (mutex->type == KLOCK_MUTEX_RECURSIVE) {
					if (currentTask == mutex->lockOwner) {
						mutex->lockCount++;

						arch_exitCriticalSection();
						break;
					}
				}
				
				if (mutex->lockOwner  == NULL || tasks_getTaskState(mutex->lockOwner) == KSTATE_UNINIT) {
					kernel_panic_p(ROMSTR("ipc_mutexLock: mutex locked with no owner"));
				}
				else {	
					#if CFG_ENABLE_MUTEX_PRIORITY_INHERITANCE == 1
						priority = tasks_getTaskPriority(mutex->lockOwner);	

						if (mutex->basePriority < priority) {
							mutex->basePriority = priority;
							tasks_raiseTaskPriority(mutex->lockOwner, mutex->basePriority);
						}
					#endif
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
	if (mutex != NULL && (mutex->type == KLOCK_MUTEX || mutex->type == KLOCK_MUTEX_RECURSIVE)) {
		arch_enterCriticalSection();

		kTask_t *currentTask = tasks_getCurrentTask();

		if (currentTask == mutex->lockOwner) {
			if (mutex->lockCount == 0) {
				kBaseType_t mutexCount = tasks_getTaskHeldMutexCount(mutex->lockOwner);

				if (mutexCount > 0) {
					tasks_setTaskHeldMutexCount(mutex->lockOwner, mutexCount - 1);
				}

				#if CFG_ENABLE_MUTEX_PRIORITY_INHERITANCE == 1
					tasks_restoreTaskPriority(mutex->lockOwner);
					mutex->basePriority = 0;
				#endif

				mutex->lockOwner = NULL;
			} else {
				if (mutex->lockCount > 0) {
					mutex->lockCount--;
				} else {
					kernel_panic_p(ROMSTR("ipc_mutexUnlock: mutex->lockCount < 0"));
				}
			}
		}

		if(mutex->blockedTasks.head != NULL) {
			tasks_unblockTask((kTask_t *)mutex->blockedTasks.head->data);
		}

		arch_exitCriticalSection();
	}

	return;
}