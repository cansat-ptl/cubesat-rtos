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
#include <mem/heap.h>
#include <mem/protection.h>

static volatile uint16_t kGlobalPid = 0;

kReturnValue_t tasks_init(kTask_t idle)
{
	return 0;
}

kReturnValue_t tasks_createTaskStatic(kStackPtr_t taskMemory, kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name)
{
	kReturnValue_t kresult = KRESULT_ERR_GENERIC;

	//TODO: Replace with spinlocks
	//kStatusRegister_t sreg = threads_startAtomicOperation();

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

	//threads_endAtomicOperation(sreg);
	return kresult;
}

kReturnValue_t tasks_createTaskDynamic(kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, uint8_t priority, kTaskType_t type, char* name)
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
