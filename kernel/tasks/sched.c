/*
 * sched.c
 * 
 * Created: 02.01.2021 11:58:19
 * Author: ThePetrovich
 */


#include <rtos/types.h>
#include <rtos/config.h>
#include <rtos/tasks/tasks.h>
#include <rtos/tasks/sched.h>
#include <rtos/arch/arch.h>
#include <rtos/common/lists.h>

volatile struct kSchedCPUStateStruct_t kSchedCPUState;

void __attribute__ (( naked, noinline )) arch_yield(void);

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

void tasks_updateSchedulingList(kTaskHandle_t task, kTaskState_t state) /* TODO: think about better implementation. This is super dumb. */
{
	if (task != NULL) {
		common_listDeleteAny(task->activeTaskListItem.list, &(task->activeTaskListItem));

		switch (state) {
			case KSTATE_SUSPENDED:
				common_listAddBack((kLinkedList_t*)&kSchedCPUState.kSuspendedTaskList, &(task->activeTaskListItem));
			break;
			case KSTATE_SLEEPING:
				common_listAddBack((kLinkedList_t*)&kSchedCPUState.kSleepingTaskList, &(task->activeTaskListItem));
			break;
			case KSTATE_READY:
				common_listAddBack((kLinkedList_t*)&kSchedCPUState.kReadyTaskList[task->priority], &(task->activeTaskListItem));
			break;
			default:
				/* Do nothing */
			break;
		}
	}
}

static inline void tasks_tickTasks()
{
	kLinkedListItem_t* head = kSchedCPUState.kSleepingTaskList.head;

	while (head != NULL) {
		if (((kTaskHandle_t)(head->data))->sleepTime) {
			((kTaskHandle_t)(head->data))->sleepTime--;
		}
		else {
			tasks_updateSchedulingList((kTaskHandle_t)head->data, KSTATE_READY);
			((kTaskHandle_t)(head->data))->state = KSTATE_READY;
		}
		head = head->next;
	}
}

static inline void tasks_search()
{
	for (kIterator_t i = CFG_NUMBER_OF_PRIORITIES-1; i >= 0; i--) {
		kLinkedListItem_t* head = kSchedCPUState.kReadyTaskList[i].head;
		if (head != NULL) {
			kSchedCPUState.kNextTask = (kTaskHandle_t)head->data;
			kSchedCPUState.kTaskActiveTicks = CFG_TICKS_PER_TASK;

			common_listDropFront((kLinkedList_t*)&kSchedCPUState.kReadyTaskList[i]);
			common_listAddBack((kLinkedList_t*)&kSchedCPUState.kReadyTaskList[i], head);
			break;
		}
	}
}

static inline void tasks_runScheduler()
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
	return;
}

static void tasks_switchContext()
{
	/* TODO: task protection checks */
	kSchedCPUState.kCurrentTask = kSchedCPUState.kNextTask;
}

void tasks_switchTask()
{
	tasks_runScheduler();
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