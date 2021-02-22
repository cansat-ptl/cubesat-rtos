/*
 * tasks.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_TASKS_H_
#define KERNEL_TASKS_H_

#include <stddef.h>
#include <rtos/types.h>
#include <rtos/common/lists.h>

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
	size_t stackSize;

	void (*entry)(void);
	void* args;

	kTaskState_t state;
	kTaskTicks_t sleepTime;
	kTaskType_t type;
	kBaseType_t priority;
	byte flags;
	kPid_t pid;
	char* name;

	volatile struct kEventStruct_t event;

	kLinkedList_t allocList;
	kLinkedList_t lockList;

	kLinkedListItem_t activeTaskListItem;
	kLinkedListItem_t globalTaskListItem;

	kLinkedList_t childList;
	kLinkedListItem_t childListItem;
};

kReturnValue_t tasks_init();

void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state);
kTaskState_t tasks_getTaskState(kTaskHandle_t task);

kReturnValue_t tasks_setTaskPriority(kTaskHandle_t task, kBaseType_t priority);
kBaseType_t tasks_getTaskPriority(kTaskHandle_t task);

kTaskHandle_t tasks_createTaskStatic(void* taskMemory, size_t memorySize, void (*entry)(void), void* args, kBaseType_t priority, kTaskType_t type, char* name);
kTaskHandle_t tasks_createTaskDynamic(size_t stackSize, void (*entry)(void), void* args, kBaseType_t priority, kTaskType_t type, char* name);

void tasks_blockTask(kTaskHandle_t task, kLinkedList_t* blockList);
void tasks_unblockTask(kTaskHandle_t task);

#endif