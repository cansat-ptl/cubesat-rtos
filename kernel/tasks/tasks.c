/*
 * tasks.c
 * 
 * Created: 04.02.2021 08:26:28
 * Author: ThePetrovich
 */


#include <rtos/types.h>
#include <rtos/config.h>
#include <rtos/tasks/tasks.h>
#include <rtos/tasks/sched.h>
#include <rtos/common/heap.h>
#include <rtos/common/lists.h>
#include <rtos/arch/arch.h>

kLinkedList_t kGlobalTaskList;

byte kIdleMem[CFG_KERNEL_IDLE_TASK_MEMORY];

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
	common_heapInit();
	kTaskHandle_t idleTask = tasks_createTaskStatic((void*)kIdleMem, CFG_KERNEL_IDLE_TASK_MEMORY, idle0, NULL, 0, KTASK_CRITICAL_STATIC, "idle");

	if (idleTask == NULL) {
		/* debug_logMessage(PGM_PUTS, L_FATAL, PSTR("\r\ntaskmgr: Startup failed, could not create idle task.\r\n")); */
		while(1);
	}

	tasks_initScheduler(idleTask);

	return 0; /* TODO: remove return */
}

/* TODO: proper architecture-independent structure size calculation */
kTaskHandle_t tasks_createTaskStatic(void* taskMemory, size_t memorySize, void (*entry)(void), void* args, kBaseType_t priority, kTaskType_t type, char* name)
{
	kTaskHandle_t returnHandle = NULL;

	arch_spinlockAcquire(&kTaskOpSpinlock);

	if (taskMemory != NULL) {
		if (entry != NULL) {
			returnHandle = (kTaskHandle_t)taskMemory;

			kStackPtr_t baseStackPtr = taskMemory + sizeof(struct kTaskStruct_t);
			size_t stackSize = memorySize - sizeof(struct kTaskStruct_t) - 1;

			/* TODO: task memory protection */
			#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
				#if CFG_STACK_GROWTH_DIRECTION == 0
					stackInitialPtr += CFG_STACK_SAFETY_MARGIN;
					common_prepareProtectionRegion((void*)(stackInitialPtr), CFG_STACK_SAFETY_MARGIN);
				#else
					common_prepareProtectionRegion((void*)(stackInitialPtr + stackSize), CFG_STACK_SAFETY_MARGIN);
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

			returnHandle->activeTaskListItem.data = (void*)returnHandle;
			returnHandle->childTaskListItem.data = (void*)returnHandle;
			returnHandle->globalTaskListItem.data = (void*)returnHandle;

			if (priority < CFG_NUMBER_OF_PRIORITIES) {
				returnHandle->priority = priority;
			}
			else {
				returnHandle->priority = CFG_NUMBER_OF_PRIORITIES-1;
			}

			common_listAddBack(&kGlobalTaskList, &(returnHandle->globalTaskListItem));

			kTaskHandle_t currentTask = tasks_getCurrentTask();
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

kTaskHandle_t tasks_createTaskDynamic(size_t stackSize, void (*entry)(void), void* args, kBaseType_t priority, kTaskType_t type, char* name)
{
	if (stackSize < CFG_MIN_TASK_STACK_SIZE) {
		stackSize = CFG_MIN_TASK_STACK_SIZE;
	}

	size_t memorySize = stackSize + sizeof(struct kTaskStruct_t);

	/* TODO: task memory protection */
	#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
		allocationSize += CFG_STACK_SAFETY_MARGIN;
	#endif

	void* taskMemory = common_heapAlloc(memorySize, NULL);
	kTaskHandle_t returnHandle = tasks_createTaskStatic(taskMemory, memorySize, entry, args, priority, type, name);
	
	returnHandle->flags |= KTASKFLAG_DYNAMIC;

	return returnHandle;
}

void tasks_deleteTaskStatic(kTaskHandle_t task)
{
	if (task != NULL) {
		arch_enterCriticalSection();

		tasks_setTaskState(task, KSTATE_UNINIT);

		kLinkedListItem_t* head = task->childTaskList.head;

		while (head != NULL) {
			if (head->data != NULL) {
				if (((kTaskHandle_t)(head->data))->flags & KTASKFLAG_DYNAMIC) {
					tasks_deleteTaskDynamic((kTaskHandle_t)(head->data));
				}
				else {
					tasks_deleteTaskStatic((kTaskHandle_t)(head->data));
				}
				
			}
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

void tasks_deleteTaskDynamic(kTaskHandle_t task)
{
	if (task != NULL) {
		tasks_deleteTaskStatic(task);
		if (task->flags & KTASKFLAG_DYNAMIC) {
			common_heapFree((void*)task);
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

void tasks_blockTask(kTaskHandle_t task, kLinkedList_t* blockList)
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

