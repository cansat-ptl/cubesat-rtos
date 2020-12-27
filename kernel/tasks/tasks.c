/*
 * tasks.c
 *
 * Created: 18.12.2020 6:06:10
 *  Author: Admin
 */


#include <types.h>
#include <config.h>
#include <common.h>
#include <tasks/tasks.h>
#include <tasks/scheduler.h>
#include <tasks/dispatcher.h>
#include <mem/heap.h>
#include <mem/protection.h>
#include <arch/arch.h>

static volatile uint16_t kGlobalPid = 0;

static kSpinlock_t kTaskOpSpinlock;

byte kIdleMem[150];

void idle0() {
	while(1) {
		;
	}
}

kReturnValue_t tasks_init()
{
	memory_heapInit();
	kTaskHandle_t idleTask;
	tasks_createTaskStatic(kIdleMem, &idleTask, idle0, NULL, 64, 0, KTASK_CRITICAL_STATIC, "idle");

	if (idleTask == NULL) {
		//debug_logMessage(PGM_PUTS, L_FATAL, PSTR("\r\ntaskmgr: Startup failed, could not create idle task.\r\n"));
		while(1);
	}

	tasks_initScheduler(idleTask);
	tasks_setCurrentTask(idleTask);
	tasks_setNextTask(idleTask);

	return 0;
}

void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state)
{
	if (task != NULL) {
		arch_enterCriticalSection();

		tasks_updateSchedulingList(task, state);
		task->state = state;

		arch_exitCriticalSection();
	}
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
			((kTaskHandle_t)taskMemory)->activeLock = NULL;

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
	}
	else {
		*handle = (kTaskHandle_t)(stackPointer);
	}

	return kresult;
}
