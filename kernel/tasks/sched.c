/*
 * sched.c
 * 
 * Created: 02.01.2021 11:58:19
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>
#include <kernel/tasks/mem.h>
#include <kernel/arch/arch.h>
#include <kernel/common/lists.h>

volatile struct kSchedCPUStateStruct_t kSchedCPUState; /* Must not be static - also used by arch/../context.S */

void tasks_initScheduler(kTaskHandle_t idle)
{
	kSchedCPUState.kReadyTaskList[0].head = &(idle->activeTaskListItem);
	kSchedCPUState.kReadyTaskList[0].tail = &(idle->activeTaskListItem);
	kSchedCPUState.kNextTask = idle;
	kSchedCPUState.kCurrentTask = idle;
}

kTaskHandle_t tasks_getCurrentTask()
{
	return kSchedCPUState.kCurrentTask;
}

void tasks_scheduleTask(kTaskHandle_t task, kTaskState_t state)
{	
	if (task != NULL) {
		arch_enterCriticalSection();

		common_listDeleteAny(task->activeTaskListItem.list, &(task->activeTaskListItem));

		switch (state) {
		case KSTATE_SUSPENDED:
			common_listAddBack((kLinkedList_t *)&kSchedCPUState.kSuspendedTaskList, &(task->activeTaskListItem));
			break;
		case KSTATE_SLEEPING:
			common_listAddBack((kLinkedList_t *)&kSchedCPUState.kSleepingTaskList, &(task->activeTaskListItem));
			break;
		case KSTATE_READY:
			common_listAddBack((kLinkedList_t *)&kSchedCPUState.kReadyTaskList[task->priority], &(task->activeTaskListItem));
			break;
		case KSTATE_UNINIT:
			/* Do nothing */
			break;
		default:
			/* Do nothing */
			break;
		}

		arch_exitCriticalSection();
	}
}

void tasks_unscheduleTask(kTaskHandle_t task)
{
	if (task != NULL) {
		arch_enterCriticalSection();

		common_listDeleteAny(task->activeTaskListItem.list, &(task->activeTaskListItem));

		arch_exitCriticalSection();
	}
}

static inline void tasks_tickTasks()
{
	kLinkedListItem_t *head = kSchedCPUState.kSleepingTaskList.head;

	while (head != NULL) {
		if (((kTaskHandle_t)(head->data))->sleepTime) {
			((kTaskHandle_t)(head->data))->sleepTime--;
		}
		else {
			tasks_setTaskState(head->data, KSTATE_READY);
		}
		head = head->next;
	}
}

static inline void tasks_search()
{
	kLinkedListItem_t *head = NULL;

	for (kIterator_t i = CFG_NUMBER_OF_PRIORITIES-1; i >= 0; i--) {
		head = kSchedCPUState.kReadyTaskList[i].head;
		if (head != NULL) {
			kSchedCPUState.kNextTask = (kTaskHandle_t)head->data;
			kSchedCPUState.kTaskActiveTicks = CFG_TICKS_PER_TASK;

			common_listDropFront((kLinkedList_t *)&kSchedCPUState.kReadyTaskList[i]);
			common_listAddBack((kLinkedList_t *)&kSchedCPUState.kReadyTaskList[i], head);
			break;
		}
	}
}

static void tasks_switchContext()
{	
	/* TODO: remove these conditions */
	#if CFG_MEMORY_PROTECTION_MODE == 1 || CFG_MEMORY_PROTECTION_MODE == 3 
		if (tasks_checkStackBounds(kSchedCPUState.kCurrentTask) != KRESULT_SUCCESS) {
			/* kernel_stackCorruptionHook(kSchedCPUState.kCurrentTask); */
		}
	#endif

	/* TODO: remove these conditions */
	#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3 
		#if CFG_STACK_GROWTH_DIRECTION == -1
			if (arch_checkProtectionRegion((void*)(kSchedCPUState.kCurrentTask->stackBegin), CFG_STACK_SAFETY_MARGIN)) {
				/* kernel_stackCorruptionHook(kSchedCPUState.kCurrentTask); */
			}
		#else
			if (arch_checkProtectionRegion((void*)(kSchedCPUState.kCurrentTask->stackBegin + kSchedCPUState.kCurrentTask->stackSize), CFG_STACK_SAFETY_MARGIN)) {
				/* kernel_stackCorruptionHook(kSchedCPUState.kCurrentTask); */
			}
		#endif
	#endif

	if (kSchedCPUState.kNextTask == NULL) {
		/* kernel_panic(PSTR("Memory access violation in task manager: kNextTask is out of bounds\r\n")); */
	}

	kSchedCPUState.kCurrentTask = kSchedCPUState.kNextTask;
}

void tasks_switchTask()
{
	if (!kSchedCPUState.kTickRate) {
		tasks_tickTasks();
		kSchedCPUState.kTickRate = CFG_TICKRATE_MS;
		if (kSchedCPUState.kTaskActiveTicks) {
			kSchedCPUState.kTaskActiveTicks--;
		}
	}
	else {
		kSchedCPUState.kTickRate--;
	}

	if (!kSchedCPUState.kTaskActiveTicks) {
		tasks_search();
	}

	if (kSchedCPUState.kNextTask != kSchedCPUState.kCurrentTask) tasks_switchContext();
}

void tasks_tick()
{
	/* TODO: timekeeping */
	tasks_switchTask();
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