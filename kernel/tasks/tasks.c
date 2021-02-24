/*
 * tasks.c
 * 
 * Created: 04.02.2021 08:26:28
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>
#include <kernel/common/heap.h>
#include <kernel/common/lists.h>
#include <kernel/arch/arch.h>

kLinkedList_t kGlobalTaskList;

byte kIdleMem[sizeof(kTask_t) + CFG_MIN_TASK_STACK_SIZE + CFG_STACK_SAFETY_MARGIN];

static kSpinlock_t kTaskOpSpinlock;
static kPid_t kGlobalPid = 0;

void idle0() 
{
	while(1) {
		;/* Do nothing */
	}
}

kReturnValue_t tasks_init()
{	
	kTaskHandle_t idleTask = NULL;

	common_heapInit();

	idleTask = tasks_createTaskStatic((void *)kIdleMem, (sizeof(kTask_t) + CFG_MIN_TASK_STACK_SIZE + CFG_STACK_SAFETY_MARGIN), idle0, NULL, 0, KTASK_CRITICAL, "idle");
	if (idleTask == NULL) {
		/* debug_logMessage(PGM_PUTS, L_FATAL, PSTR("\r\ntaskmgr: Startup failed, could not create idle task.\r\n")); */
		while(1);
	}
	tasks_initScheduler(idleTask);

	return 0; /* TODO: remove return */
}

/* TODO: proper architecture-independent structure size calculation */
kTaskHandle_t tasks_createTaskStatic(void *taskMemory, size_t memorySize, void (*entry)(void), void *args, kBaseType_t priority, kTaskType_t type, char *name)
{
	kTaskHandle_t returnHandle = NULL;
	kTaskHandle_t currentTask = NULL;
	kStackPtr_t baseStackPtr = NULL;
	size_t stackSize = 0;

	arch_spinlockAcquire(&kTaskOpSpinlock);

	if (taskMemory != NULL && entry != NULL) {
		if (memorySize - sizeof(kTask_t) >= CFG_MIN_TASK_STACK_SIZE) {
			returnHandle = (kTaskHandle_t)taskMemory;

			baseStackPtr = taskMemory + sizeof(kTask_t);
			stackSize = memorySize - sizeof(kTask_t) - 1;
			
			/* TODO: remove this condition */
			#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
				#if CFG_STACK_GROWTH_DIRECTION == -1
					arch_prepareProtectionRegion((void *)(baseStackPtr), CFG_STACK_SAFETY_MARGIN);
					baseStackPtr += CFG_STACK_SAFETY_MARGIN;
					stackSize -= CFG_STACK_SAFETY_MARGIN;
				#else
					arch_prepareProtectionRegion((void *)(baseStackPtr + stackSize), CFG_STACK_SAFETY_MARGIN);
				#endif
			#endif

			returnHandle->stackPtr = arch_prepareStackFrame(baseStackPtr, stackSize, entry, args);
			returnHandle->stackBegin = baseStackPtr;
			returnHandle->stackSize = stackSize;
			returnHandle->entry = entry;
			returnHandle->args = args;
			returnHandle->type = type;
			returnHandle->pid = kGlobalPid;
			returnHandle->name = name;

			returnHandle->activeTaskListItem.data = (void *)returnHandle;
			returnHandle->childTaskListItem.data = (void *)returnHandle;
			returnHandle->globalTaskListItem.data = (void *)returnHandle;

			if (priority < CFG_NUMBER_OF_PRIORITIES) {
				returnHandle->priority = priority;
			}
			else {
				returnHandle->priority = CFG_NUMBER_OF_PRIORITIES-1;
			}

			common_listAddBack(&kGlobalTaskList, &(returnHandle->globalTaskListItem));

			currentTask = tasks_getCurrentTask();
			if (currentTask != NULL) {
				common_listAddBack(&(currentTask->childTaskList), &(returnHandle->childTaskListItem));
			}

			kGlobalPid++;

			tasks_setTaskState(returnHandle, KSTATE_READY);
		}
	}

	arch_spinlockRelease(&kTaskOpSpinlock);

	return returnHandle;
}

kTaskHandle_t tasks_createTaskDynamic(size_t stackSize, void (*entry)(void), void *args, kBaseType_t priority, kTaskType_t type, char *name)
{	
	kTaskHandle_t returnHandle = NULL;
	void *taskMemory = NULL;
	size_t memorySize = 0;

	if (stackSize < CFG_MIN_TASK_STACK_SIZE) {
		stackSize = CFG_MIN_TASK_STACK_SIZE;
	}

	memorySize = stackSize + sizeof(kTask_t);

	#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
		memorySize += CFG_STACK_SAFETY_MARGIN + 1;
	#endif

	taskMemory = common_heapAlloc(memorySize, NULL);
	returnHandle = tasks_createTaskStatic(taskMemory, memorySize, entry, args, priority, type, name);
	
	if (returnHandle != NULL) {
		returnHandle->flags |= KTASKFLAG_DYNAMIC;
	}

	return returnHandle;
}

static void tasks_deleteTaskStatic(kTaskHandle_t task)
{	
	kLinkedListItem_t *head = NULL;

	if (task != NULL) {
		arch_enterCriticalSection();

		tasks_setTaskState(task, KSTATE_UNINIT);

		head = task->childTaskList.head;

		while (head != NULL) {
			tasks_deleteTask((kTaskHandle_t)(head->data));
			head = head->next;
		}

		head = task->allocList.head;
		
		while(head != NULL) {
			common_heapFree(head->data);
			head = head->next;
		}

		common_listDeleteAny(&kGlobalTaskList, &(task->globalTaskListItem));
		common_listDeleteAny(&(task->childTaskList), &(task->childTaskListItem));

		arch_exitCriticalSection();
	}
}

void tasks_deleteTask(kTaskHandle_t task) 
{
	if (task != NULL) {
		tasks_deleteTaskStatic(task);
		if (task->flags & KTASKFLAG_DYNAMIC) {
			common_heapFree((void *)task);
		}
	}
}

kBaseType_t tasks_getTaskPriority(kTaskHandle_t task) 
{
	kBaseType_t priority = 0;

	if (task != NULL) {
		priority = task->priority;
	}

	return priority;
}

void tasks_setTaskPriority(kTaskHandle_t task, kBaseType_t priority)
{
	if (task != NULL) {
		if (priority <= CFG_NUMBER_OF_PRIORITIES) {
			arch_enterCriticalSection();

			tasks_unscheduleTask(task);
			task->priority = priority;
			tasks_scheduleTask(task, task->state);

			arch_exitCriticalSection();
		}
	}
}

kTaskState_t tasks_getTaskState(kTaskHandle_t task) 
{
	kTaskState_t state = KSTATE_UNINIT;

	if (task != NULL) {
		state = task->state;
	}

	return state;
}

void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state)
{
	if (task != NULL) {
		arch_enterCriticalSection();

		tasks_scheduleTask(task, state);
		task->state = state;

		arch_exitCriticalSection();
	}
}

void tasks_blockTask(kTaskHandle_t task, kLinkedList_t *blockList)
{
	if (task != NULL && blockList != NULL) {
		if (task->activeTaskListItem.list != blockList) {
			tasks_setTaskState(task, KSTATE_BLOCKED);
			common_listAddBack(blockList, &(task->activeTaskListItem));
		}
	}
}

void tasks_unblockTask(kTaskHandle_t task) 
{
	tasks_setTaskState(task, KSTATE_READY);
}

