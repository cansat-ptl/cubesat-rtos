/*
 * taskcreation.c
 * 
 * Created: 04.02.2021 08:23:41
 * Author: ThePetrovich
 */


#include <types.h>
#include <config.h>
#include <common.h>
#include <tasks/tasks.h>
#include <tasks/sched.h>
#include <tasks/dispatch.h>
#include <mem/module.h>
#include <arch/module.h>

static kSpinlock_t kTaskOpSpinlock;

static volatile uint16_t kGlobalPid = 0;

void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state)
{
	if (task != NULL) {
		arch_enterCriticalSection();

		tasks_updateSchedulingList(task, state);
		task->state = state;

		arch_exitCriticalSection();
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

kReturnValue_t tasks_setTaskPriority(kTaskHandle_t task, kBaseType_t priority)
{
	kReturnValue_t kresult = KRESULT_ERR_NULLPTR;

	if (task != NULL) {
		if (priority <= CFG_NUMBER_OF_PRIORITIES) {
			arch_enterCriticalSection();

			task->priority = priority;
			if (task->state == KSTATE_READY) {
				tasks_updateSchedulingList(task, KSTATE_READY);
			}
			kresult = KRESULT_SUCCESS;

			arch_exitCriticalSection();
		}
		else {
			kresult = CFG_NUMBER_OF_PRIORITIES;
		}
	}

	return kresult;
}

kBaseType_t tasks_getTaskPriority(kTaskHandle_t task) 
{
	kBaseType_t priority = 0;

	if (task != NULL) {
		priority = task->priority;
	}

	return priority;
}

kReturnValue_t tasks_createTaskStatic(kStackPtr_t taskMemory, kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, kBaseType_t priority, kTaskType_t type, char* name)
{
	kReturnValue_t kresult = KRESULT_ERR_GENERIC;

	arch_spinlockAcquire(&kTaskOpSpinlock);

	if (entry != NULL) {
		if (taskMemory != NULL) {
			((kTaskHandle_t)taskMemory)->activeTaskListItem.data = taskMemory;

			kStackPtr_t stackInitialPtr = taskMemory + sizeof(struct kTaskStruct_t);

			#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
				#if CFG_STACK_GROWTH_DIRECTION == 0
					stackInitialPtr += CFG_STACK_SAFETY_MARGIN;
					memory_prepareProtectionRegion((void*)(stackInitialPtr), CFG_STACK_SAFETY_MARGIN);
				#else
					memory_prepareProtectionRegion((void*)(stackInitialPtr + stackSize), CFG_STACK_SAFETY_MARGIN);
				#endif
			#endif

			((kTaskHandle_t)taskMemory)->stackPtr = arch_prepareStackFrame(stackInitialPtr, stackSize, entry, args);
			((kTaskHandle_t)taskMemory)->stackBegin = stackInitialPtr;
			((kTaskHandle_t)taskMemory)->stackSize = stackSize;
			((kTaskHandle_t)taskMemory)->entry = entry;
			((kTaskHandle_t)taskMemory)->args = args;
			((kTaskHandle_t)taskMemory)->type = type;
			((kTaskHandle_t)taskMemory)->pid = kGlobalPid;
			((kTaskHandle_t)taskMemory)->name = name;

			if (priority < CFG_NUMBER_OF_PRIORITIES) {
				((kTaskHandle_t)taskMemory)->priority = priority;
			}
			else {
				((kTaskHandle_t)taskMemory)->priority = CFG_NUMBER_OF_PRIORITIES-1;
			}

			tasks_setTaskState((kTaskHandle_t)taskMemory, KSTATE_READY);

			kGlobalPid++;
			kresult = KRESULT_SUCCESS;

			if (handle != NULL) {
				*handle = (kTaskHandle_t)taskMemory;
			}
		}
		else {
			kresult = KRESULT_ERR_NULLPTR;
		}
	}

	arch_spinlockRelease(&kTaskOpSpinlock);
	return kresult;
}

kReturnValue_t tasks_createTaskDynamic(kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, kBaseType_t priority, kTaskType_t type, char* name)
{
	kReturnValue_t kresult = KRESULT_ERR_GENERIC;

	if (stackSize < CFG_MIN_TASK_STACK_SIZE) {
		stackSize = CFG_MIN_TASK_STACK_SIZE;
	}

	size_t allocationSize = stackSize + sizeof(struct kTaskStruct_t);

	#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
		allocationSize += CFG_STACK_SAFETY_MARGIN;
	#endif

	kStackPtr_t stackPointer = (kStackPtr_t)memory_heapAlloc(allocationSize);
	kresult = tasks_createTaskStatic(stackPointer, NULL, entry, args, stackSize, priority, type, name);

	if (kresult != KRESULT_SUCCESS) {
		memory_heapFree((void*)stackPointer);
		if (memory_getFreeHeap() < allocationSize) {
			kresult = KRESULT_ERR_OUT_OF_MEMORY;
		}
	}
	else {
		*handle = (kTaskHandle_t)(stackPointer);
	}

	return kresult;
}

void tasks_blockTask(kTaskHandle_t task, volatile struct kLinkedListStruct_t* blockList) 
{
    if (task != NULL) {
        if (blockList != NULL) {
            if (task->activeTaskListItem.list != blockList) {
                tasks_setTaskState(task, KSTATE_BLOCKED);
                common_listAddBack(blockList, &(task->activeTaskListItem));
            }
        }
    }
}

void tasks_unblockTask(kTaskHandle_t task) 
{
    tasks_setTaskState(task, KSTATE_READY);
}

