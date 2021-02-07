/*
 * tasks.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_TASKS_H_
#define KERNEL_TASKS_H_

#include <types.h>
#include <common.h>

struct kLockStruct_t;

struct kEventStruct_t
{
	kEventState_t state;
	kBaseType_t eventFlags;
};

struct kTaskStruct_t
{
	kStackPtr_t stackPtr;
	kStackPtr_t stackBegin;
	kStackSize_t stackSize;

	kTask_t entry;
	void* args;

	kTaskState_t state;
	kTaskTicks_t sleepTime;
	kTaskType_t type;
	kBaseType_t priority;
	byte flags;
	kPid_t pid;
	char* name;

	volatile struct kEventStruct_t notification;

	volatile struct kListItemStruct_t activeTaskListItem;
	volatile struct kListItemStruct_t globalTaskListItem;

	//#if CFG_TRACK_MEMORY_BLOCK_OWNERS == 1
		volatile struct kLinkedListStruct_t ownedHeapBlocks;
	//#endif

	#if CFG_TRACK_TASK_HELD_LOCKS == 1
		volatile struct kLinkedListStruct_t heldLocks;
	#endif
};

kReturnValue_t tasks_init();

#endif /* TASKS_H_ */