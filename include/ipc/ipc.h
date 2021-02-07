/*
 * ipc.h
 * 
 * Created: 04.02.2021 07:53:54
 * Author: ThePetrovich
 */


#ifndef IPC_H_
#define IPC_H_

#include <types.h>
#include <common.h>

struct kLockStruct_t
{
	uint8_t lockCount;
	uint8_t basePriority;
	kLockType_t type;
	kTaskHandle_t lockOwner;
	struct kLinkedListStruct_t blockedTasks;
};

#endif