/*
 * mutex.h
 * 
 * Created: 04.02.2021 08:07:14
 * Author: ThePetrovich
 */


#ifndef KERNEL_MUTEX_H_
#define KERNEL_MUTEX_H_

#include <kernel/types.h>
#include <kernel/common/lists.h>

typedef volatile struct kMutexStruct_t
{	
	kLockType_t type;

	kBaseType_t lockCount;

        #if CFG_ENABLE_MUTEX_PRIORITY_INHERITANCE == 1
	        kBaseType_t basePriority;
        #endif

	kTask_t *lockOwner;

	kLinkedList_t blockedTasks;

	kSpinlock_t spinlock;
} kMutex_t;

#ifdef __cplusplus
extern "C" {
#endif

void ipc_mutexInit(kMutex_t *mutex, kLockType_t type);
void ipc_mutexLock(kMutex_t *mutex);
void ipc_mutexUnlock(kMutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_MUTEX_H_ */