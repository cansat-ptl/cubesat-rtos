/*
 * tasks.h
 *
 * Created: 18.12.2020 6:07:57
 *  Author: Admin
 */


#ifndef TASKS_H_
#define TASKS_H_

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

kReturnValue_t tasks_createTaskStatic(kStackPtr_t taskMemory, kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, kBaseType_t priority, kTaskType_t type, char* name);
kReturnValue_t tasks_createTaskDynamic(kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, kBaseType_t priority, kTaskType_t type, char* name);

void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state);
kTaskState_t tasks_getTaskState(kTaskHandle_t task);
void tasks_blockTask(kTaskHandle_t task, volatile struct kLinkedListStruct_t* blockList);
void tasks_unblockTask(kTaskHandle_t task);

kReturnValue_t tasks_setTaskPriority(kTaskHandle_t task, kBaseType_t priority);
kBaseType_t tasks_getTaskPriority(kTaskHandle_t task);

#endif /* TASKS_H_ */