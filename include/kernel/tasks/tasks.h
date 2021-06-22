/*
 * tasks.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_TASKS_H_
#define KERNEL_TASKS_H_

#include <stddef.h>
#include <kernel/types.h>
#include <kernel/common/lists.h>

#define KTASKFLAG_DYNAMIC ((byte)(0x01))

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

	kLinkedListItem_t activeTaskListItem;
	kLinkedListItem_t globalTaskListItem;
	kLinkedListItem_t childTaskListItem;

	kLinkedList_t childTaskList;
};

#define tasks_TASK_STRUCT_SIZE ((sizeof(struct kTaskStruct_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK))

#ifdef __cplusplus
extern "C" {
#endif

void tasks_init();

kTask_t *tasks_createTaskStatic(void *taskMemory, size_t memorySize, void (*entry)(void), void *args, kBaseType_t priority, kTaskType_t type, char *name);
kTask_t *tasks_createTaskDynamic(size_t stackSize, void (*entry)(void), void *args, kBaseType_t priority, kTaskType_t type, char *name);
void tasks_deleteTask(kTask_t *task);

void tasks_setTaskState(kTask_t *task, kTaskState_t state);
void tasks_setTaskPriority(kTask_t *task, kBaseType_t priority);

kTaskState_t tasks_getTaskState(kTask_t *task);
kBaseType_t tasks_getTaskPriority(kTask_t *task);
kTaskType_t tasks_getTaskType(kTask_t *task);
kLinkedList_t *tasks_getTaskAllocList(kTask_t *task);

void tasks_blockTask(kTask_t *task, kLinkedList_t *blockList);
void tasks_unblockTask(kTask_t *task);

kReturnValue_t tasks_checkStackBounds(kTask_t *task);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_TASKS_H_ */