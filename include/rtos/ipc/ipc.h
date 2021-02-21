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

	uint8_t lockCount;
	uint8_t basePriority;
	
	kTaskHandle_t lockOwner;

	kLinkedList_t blockedTasks;
};

#endif