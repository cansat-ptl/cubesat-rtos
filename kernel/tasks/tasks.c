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

byte kIdleMem[CFG_KERNEL_IDLE_TASK_MEMORY];

static kSpinlock_t kTaskOpSpinlock;

static kPid_t kGlobalPid = 0;

void idle0() {
	while(1) {
		;
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

	return 0;
}

kTaskHandle_t tasks_createTaskStatic(void* taskMemory, size_t memorySize, void (*entry)(void), void* args, kBaseType_t priority, kTaskType_t type, char* name)
{
	kTaskHandle_t returnHandle = NULL;

	arch_spinlockAcquire(&kTaskOpSpinlock);

	if (taskMemory != NULL) {
		if (entry != NULL) {
			((kTaskHandle_t)taskMemory)->activeTaskListItem.data = (void*)taskMemory;

			kStackPtr_t baseStackPtr = taskMemory + sizeof(struct kTaskStruct_t);
			size_t stackSize = memorySize - sizeof(struct kTaskStruct_t) - 1;

			#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
				#if CFG_STACK_GROWTH_DIRECTION == 0
					stackInitialPtr += CFG_STACK_SAFETY_MARGIN;
					common_prepareProtectionRegion((void*)(stackInitialPtr), CFG_STACK_SAFETY_MARGIN);
				#else
					common_prepareProtectionRegion((void*)(stackInitialPtr + stackSize), CFG_STACK_SAFETY_MARGIN);
				#endif
			#endif

			((kTaskHandle_t)taskMemory)->stackPtr = arch_prepareStackFrame(baseStackPtr, stackSize, entry, args);
			((kTaskHandle_t)taskMemory)->stackBegin = baseStackPtr;
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

			returnHandle = (kTaskHandle_t)taskMemory;
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

	#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
		allocationSize += CFG_STACK_SAFETY_MARGIN;
	#endif

	void* taskMemory = common_heapAlloc(memorySize, NULL);
	
	return tasks_createTaskStatic(taskMemory, memorySize, entry, args, priority, type, name);
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

kBaseType_t tasks_getTaskPriority(kTaskHandle_t task) 
{
	kBaseType_t priority = 0;

	if (task != NULL) {
		priority = task->priority;
	}

	return priority;
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
