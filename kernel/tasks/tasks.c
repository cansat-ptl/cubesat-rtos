/*
 * tasks.c
 * 
 * Created: 04.02.2021 08:26:28
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/panic.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>
#include <kernel/mem/heap.h>
#include <kernel/common/lists.h>
#include <kernel/arch/arch.h>

kLinkedList_t kGlobalTaskList;

byte kIdleMem[CFG_KERNEL_IDLE_TASK_MEMORY];

static kPid_t kGlobalPid = 0;

void idle0() 
{
	while(1) {
		;/* Do nothing */
	}
}

void tasks_init()
{	
	kTask_t *idleTask = NULL;

	idleTask = tasks_createTaskStatic((void *)kIdleMem,
					(sizeof(kTask_t) + 100 + CFG_STACK_SAFETY_MARGIN),
					idle0,
					NULL,
					0,
					KTASK_CRITICAL,
					"idle");
	if (idleTask == NULL) {
		kernel_panic_p(ROMSTR("Failed to create idle task"));
		while(1);
	}
	tasks_initScheduler(idleTask);
}

kTask_t *tasks_createTaskStatic(void *taskMemory, 
				size_t memorySize, 
				void (*entry)(void *), 
				void *args, 
				kBaseType_t priority, 
				kTaskType_t type, 
				char *name)
{
	kTask_t *returnHandle = NULL;
	void *baseStackPtr = NULL;
	size_t stackSize = 0;

	if (taskMemory != NULL && entry != NULL) {
		if (memorySize - tasks_TASK_STRUCT_SIZE >= CFG_MIN_TASK_STACK_SIZE) {
			arch_enterCriticalSection();

			returnHandle = (kTask_t *)taskMemory;

			baseStackPtr = taskMemory + tasks_TASK_STRUCT_SIZE;
			stackSize = memorySize - tasks_TASK_STRUCT_SIZE;

			#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
				stackSize -= CFG_STACK_SAFETY_MARGIN;
			#endif

			baseStackPtr = (void *)arch_prepareProtectionRegion((kStackPtr_t)baseStackPtr, stackSize, CFG_STACK_SAFETY_MARGIN);

			returnHandle->stackPtr = arch_prepareStackFrame(baseStackPtr, stackSize, entry, args);
			returnHandle->stackBegin = baseStackPtr;
			returnHandle->stackSize = stackSize;
			returnHandle->entry = entry;
			returnHandle->args = args;
			returnHandle->type = type;
			returnHandle->pid = kGlobalPid++;
			returnHandle->name = name;

			returnHandle->activeTaskListItem.data = (void *)returnHandle;
			returnHandle->globalTaskListItem.data = (void *)returnHandle;

			if (priority < CFG_NUMBER_OF_PRIORITIES) {
				returnHandle->priority = priority;
			}
			else {
				returnHandle->priority = CFG_NUMBER_OF_PRIORITIES-1;
			}

			common_listAddBack(&kGlobalTaskList, &(returnHandle->globalTaskListItem));

			#if CFG_ENABLE_CHILD_TASK_TRACKING == 1
				returnHandle->childTaskListItem.data = (void *)returnHandle;

				kTask_t *currentTask = tasks_getCurrentTask();
				if (currentTask != NULL) {
					common_listAddBack(&(currentTask->childTaskList), &(returnHandle->childTaskListItem));
				}
			#endif

			tasks_setTaskState(returnHandle, KSTATE_READY);

			arch_exitCriticalSection();
		}
	}

	return returnHandle;
}

kTask_t *tasks_createTaskDynamic(size_t stackSize, 
				void (*entry)(void *), 
				void *args, 
				kBaseType_t priority, 
				kTaskType_t type, 
				char *name)
{	
	kTask_t *returnHandle = NULL;
	void *taskMemory = NULL;
	size_t memorySize = 0;

	arch_enterCriticalSection();

	if (stackSize < CFG_MIN_TASK_STACK_SIZE) {
		stackSize = CFG_MIN_TASK_STACK_SIZE;
	}

	memorySize = stackSize + tasks_TASK_STRUCT_SIZE;

	#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
		memorySize += CFG_STACK_SAFETY_MARGIN + 1;
	#endif

	taskMemory = mem_heapAlloc(memorySize, NULL);
	returnHandle = tasks_createTaskStatic(taskMemory, memorySize, entry, args, priority, type, name);
	
	if (returnHandle != NULL) {
		returnHandle->flags |= KTASKFLAG_DYNAMIC;
	}

	arch_exitCriticalSection();

	return returnHandle;
}

void tasks_deleteTask(kTask_t *task) 
{	
	kLinkedListItem_t *head = NULL;
	kLinkedListItem_t *next = NULL;
	
	if (task != NULL && task->mutexCount == 0) {
		arch_enterCriticalSection();

		tasks_setTaskState(task, KSTATE_UNINIT);

		#if CFG_ENABLE_CHILD_TASK_TRACKING == 1
			head = task->childTaskList.head;
			
			while (head != NULL) {
				next = head->next;
				tasks_deleteTask((kTask_t *)(head->data));
				head = next;
			}
		#endif
		
		#if CFG_HEAP_ALLOCATION_TRACKING == 1
			head = task->allocList.head;
			
			while(head != NULL) {
				mem_heapFree(head->data);
				head = head->next;
			}
		#endif

		common_listDeleteAny(&kGlobalTaskList, &(task->globalTaskListItem));

		#if CFG_ENABLE_CHILD_TASK_TRACKING == 1
			common_listDeleteAny(&(task->childTaskList), &(task->childTaskListItem));
		#endif

		if (task->flags & KTASKFLAG_DYNAMIC) {
			mem_heapFree((void *)task);
		}

		arch_exitCriticalSection();
	}
}

kBaseType_t tasks_getTaskPriority(kTask_t *task) 
{
	kBaseType_t priority = 0;

	if (task != NULL) {
		arch_enterCriticalSection();

		priority = task->priority;

		arch_exitCriticalSection();
	}

	return priority;
}

kTaskState_t tasks_getTaskState(kTask_t *task) 
{
	kTaskState_t state = KSTATE_UNINIT;

	if (task != NULL) {
		arch_enterCriticalSection();

		state = task->state;

		arch_exitCriticalSection();
	}

	return state;
}

kTaskType_t tasks_getTaskType(kTask_t *task)
{	
	kTaskType_t type = KTASK_UNINIT;
	if (task != NULL) {
		arch_enterCriticalSection();

		type = task->type;

		arch_exitCriticalSection();
	}

	return type;
}

kLinkedList_t *tasks_getTaskAllocList(kTask_t *task)
{	
	kLinkedList_t *allocList = NULL;

	#if CFG_HEAP_ALLOCATION_TRACKING == 1
		if (task != NULL) {
			arch_enterCriticalSection();

			allocList = &(task->allocList);

			arch_exitCriticalSection();
		}
	#endif

	return allocList;
}

kBaseType_t tasks_getHeldMutexCount(kTask_t *task)
{
	kBaseType_t mutexCount = 0;

	if (task != NULL) {
		arch_enterCriticalSection();

		mutexCount = task->mutexCount;

		arch_exitCriticalSection();
	}

	return mutexCount;
}

const char *tasks_getTaskName(kTask_t *task)
{
	const char *name = NULL;

	if (task != NULL) {
		arch_enterCriticalSection();

		name = task->name;

		arch_exitCriticalSection();
	}

	return name;
}

kPid_t tasks_getTaskPid(kTask_t *task)
{
	kPid_t pid = 0;

	if (task != NULL) {
		arch_enterCriticalSection();

		pid = task->pid;

		arch_exitCriticalSection();
	}

	return pid;
}

kTask_t *tasks_getTaskByPid(kPid_t pid)
{
	kTask_t *task = NULL;
	kLinkedListItem_t *head = kGlobalTaskList.head;

	arch_enterCriticalSection();

	while (head != NULL) {
		if (head->data != NULL) {
			if (((kTask_t *)head->data)->pid == pid) {
				task = (kTask_t *)head->data;
			}
		}
		else {
			kernel_panic_p(ROMSTR("tasks_getTaskByPid: head->data = NULL"));
		}

		head = head->next;
	}

	arch_exitCriticalSection();

	return task;
}

kTask_t *tasks_getTaskByName(const char *name)
{
	kTask_t *task = NULL;
	kLinkedListItem_t *head = kGlobalTaskList.head;

	arch_enterCriticalSection();
	
	while (head != NULL) {
		if (head->data != NULL) {
			if (!common_strcmp(((kTask_t *)head->data)->name, name)) {
				task = (kTask_t *)head->data;
			}
		}
		else {
			kernel_panic_p(ROMSTR("tasks_getTaskByName: head->data = NULL"));
		}

		head = head->next;
	}

	arch_exitCriticalSection();

	return task;
}

void tasks_setTaskPriority(kTask_t *task, kBaseType_t priority)
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

void tasks_setTaskState(kTask_t *task, kTaskState_t state)
{
	if (task != NULL) {
		arch_enterCriticalSection();

		tasks_scheduleTask(task, state);
		task->state = state;

		arch_exitCriticalSection();
	}
}

void tasks_setHeldMutexCount(kTask_t *task, kBaseType_t mutexCount)
{
	if (task != NULL) {
		arch_enterCriticalSection();

		task->mutexCount = mutexCount;

		arch_exitCriticalSection();
	}
}

void tasks_blockTask(kTask_t *task, kLinkedList_t *blockList)
{
	if (task != NULL && blockList != NULL) {
		arch_enterCriticalSection();

		if (task->activeTaskListItem.list != blockList) {
			tasks_setTaskState(task, KSTATE_BLOCKED);
			common_listAddBack(blockList, &(task->activeTaskListItem));
		}

		arch_exitCriticalSection();
	}
}

void tasks_unblockTask(kTask_t *task) 
{
	tasks_setTaskState(task, KSTATE_READY);
}

kReturnValue_t tasks_checkStackBounds(kTask_t *task) 
{
	kReturnValue_t exitcode = KRESULT_SUCCESS;

	if (task->stackPtr < task->stackBegin || task->stackPtr > task->stackBegin + task->stackSize) {
		exitcode = KRESULT_ERR_GENERIC;
	}

	return exitcode;
}

