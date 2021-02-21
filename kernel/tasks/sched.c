/*f
 * scheduler.c
 *
 * Created: 18.12.2020 6:27:24
 *  Author: Admin
 */


#include <rtos/types.h>
#include <rtos/config.h>
#include <rtos/tasks/tasks.h>
#include <rtos/tasks/sched.h>
#include <rtos/tasks/dispatch.h>
#include <rtos/arch/arch.h>
#include <rtos/common/lists.h>

volatile struct kSchedCPUStateStruct_t kSchedCPUState;

void tasks_initScheduler(kTaskHandle_t idle)
{
	kSchedCPUState.kReadyTaskList[0].head = &(idle->activeTaskListItem);
	kSchedCPUState.kReadyTaskList[0].tail = &(idle->activeTaskListItem);
}

void tasks_updateSchedulingList(kTaskHandle_t task, kTaskState_t state)
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

/* WHAT THE HELL AM I DOING SOMEBODY PLEASE HELP ME */
static inline void tasks_tickTasks()
{
	kLinkedListItem_t* temp = kSchedCPUState.kSleepingTaskList.head;

	while (temp != NULL) {
		if (((kTaskHandle_t)(temp->data))->sleepTime) {
			((kTaskHandle_t)(temp->data))->sleepTime--;
		}
		else {
			tasks_updateSchedulingList((kTaskHandle_t)temp->data, KSTATE_READY);
			((kTaskHandle_t)(temp->data))->state = KSTATE_READY;
		}
		temp = temp->next;
	}
}

static inline void tasks_search()
{
	for (kIterator_t i = CFG_NUMBER_OF_PRIORITIES-1; i >= 0; i--) {
		if (kSchedCPUState.kReadyTaskList[i].head != NULL) {
			kSchedCPUState.kNextTask = kSchedCPUState.kReadyTaskList[i].head->data;
			kSchedCPUState.kTaskActiveTicks = CFG_TICKS_PER_TASK;

			kLinkedListItem_t* temp = kSchedCPUState.kReadyTaskList[i].head;
			common_listDropFront((kLinkedList_t*)&kSchedCPUState.kReadyTaskList[i]);
			common_listAddBack((kLinkedList_t*)&kSchedCPUState.kReadyTaskList[i], temp);
			break;
		}
	}
}

void tasks_runScheduler()
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