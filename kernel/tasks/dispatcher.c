/*
 * dispatcher.c
 *
 * Created: 18.12.2020 6:27:46
 *  Author: Admin
 */


#include <types.h>
#include <config.h>
#include <common.h>
#include <arch/arch.h>
#include <tasks/tasks.h>
#include <tasks/scheduler.h>
#include <arch/mega128/uart.h>

extern struct kSchedCPUStateStruct_t kSchedCPUState;

static void tasks_switchContext()
{
	/*#if CFG_MEMORY_PROTECTION_MODE == 1
	if (tasks_checkStackBounds(kCurrentTask)) {
		kernel_stackCorruptionHook(kCurrentTask);
	}
	#endif

	#if CFG_MEMORY_PROTECTION_MODE == 2
	#if CFG_STACK_GROWTH_DIRECTION == 0
	if (memory_checkProtectionRegion((void*)(kCurrentTask->stackBegin), CFG_STACK_SAFETY_MARGIN)) {
		kernel_stackCorruptionHook(kCurrentTask);
	}
	#else
	if (memory_checkProtectionRegion((void*)(kCurrentTask->stackBegin + kCurrentTask->stackSize), CFG_STACK_SAFETY_MARGIN)) {
		kernel_stackCorruptionHook(kCurrentTask);
	}
	#endif
	#endif

	#if CFG_MEMORY_PROTECTION_MODE == 3
	#if CFG_STACK_GROWTH_DIRECTION == 0
	if (tasks_checkStackBounds(kCurrentTask) || memory_checkProtectionRegion((void*)(kCurrentTask->stackBegin), CFG_STACK_SAFETY_MARGIN)) {
		kernel_stackCorruptionHook(kCurrentTask);
	}
	#else
	if (tasks_checkStackBounds(kCurrentTask) || memory_checkProtectionRegion((void*)(kCurrentTask->stackBegin + kCurrentTask->stackSize), CFG_STACK_SAFETY_MARGIN)) {
		kernel_stackCorruptionHook(kCurrentTask);
	}
	#endif
	#endif

	#if CFG_MEMORY_PROTECTION_MODE != 0
	if (memory_pointerSanityCheck((void*)kNextTask) != 0) {
		kernel_panic(PSTR("Memory access violation in task manager: kNextTask is out of bounds\r\n"));
	}
	#endif*/
	kSchedCPUState.kCurrentTask = kSchedCPUState.kNextTask;
}

kTaskHandle_t tasks_getCurrentTask()
{
	return kSchedCPUState.kCurrentTask;
}

void tasks_setCurrentTask(kTaskHandle_t taskHandle)
{
	kSchedCPUState.kCurrentTask = taskHandle;
}

kTaskHandle_t tasks_getNextTask()
{
	return kSchedCPUState.kNextTask;
}

void tasks_setNextTask(kTaskHandle_t taskHandle)
{
	kSchedCPUState.kNextTask = taskHandle;
}

void tasks_sleep(kTaskTicks_t sleep)
{
	kSchedCPUState.kTaskActiveTicks = 0;

	if (sleep != 0) {
		tasks_setTaskState(kSchedCPUState.kCurrentTask, KSTATE_SLEEPING);
		kSchedCPUState.kCurrentTask->sleepTime = sleep;
	}
	arch_yield();
}

void tasks_tick()
{
	//tasks_runScheduler();
	//if (kNextTask != kCurrentTask) tasks_switchContext();

	//kernel_timerService();

	uart_puts("ISR\r\n");
}