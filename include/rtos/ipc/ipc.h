/*
 * ipc.h
 * 
 * Created: 04.02.2021 07:53:54
 * Author: ThePetrovich
 */


#ifndef KERNEL_IPC_H_
#define KERNEL_IPC_H_

#include <rtos/types.h>
#include <rtos/common/lists.h>

struct kLockStruct_t
{	
	kLockType_t type;

	kBaseType_t lockCount;
	kBaseType_t basePriority;

	kTaskHandle_t lockOwner;

	kLinkedList_t blockedTasks;

	kSpinlock_t spinlock;
};

struct kIPCStruct_t
{
	void* pointer;

	size_t itemSize;
	size_t bufferSize;
	size_t inputPosition;
	size_t outputPosition;
	size_t currentPosition;

	struct kLockStruct_t mutex;
};

#endif