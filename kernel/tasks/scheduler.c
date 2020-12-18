/*
 * scheduler.c
 *
 * Created: 18.12.2020 6:27:24
 *  Author: Admin
 */


#include <types.h>
#include <config.h>
#include <common.h>
#include <arch/arch.h>
#include <tasks/tasks.h>
#include <tasks/scheduler.h>
#include <tasks/dispatcher.h>

struct kSchedCPUStateStruct_t kSchedCPUState;

void tasks_initScheduler(kTaskHandle_t idle)
{
	kSchedCPUState.kReadyTaskList[0].head = &(idle->activeTaskListItem);
	kSchedCPUState.kReadyTaskList[9].tail = &(idle->activeTaskListItem);
}

void tasks_rescheduleTask(kTaskHandle_t task, kTaskState_t state)
{
	switch (state) {
		case KSTATE_UNINIT:
			common_listDeleteAny(task->activeTaskListItem.list, &(task->activeTaskListItem));
		break;
		case KSTATE_SUSPENDED:
			common_listDeleteAny(task->activeTaskListItem.list, &(task->activeTaskListItem));
			common_listAddBack(&kSchedCPUState.kSuspendedTaskList, &(task->activeTaskListItem));
		break;
		case KSTATE_SLEEPING:
			common_listDeleteAny(task->activeTaskListItem.list, &(task->activeTaskListItem));
			common_listAddBack(&kSchedCPUState.kSleepingTaskList, &(task->activeTaskListItem));
		break;
		case KSTATE_BLOCKED:
			common_listDeleteAny(task->activeTaskListItem.list, &(task->activeTaskListItem));
		break;
		case KSTATE_READY:
			common_listDeleteAny(task->activeTaskListItem.list, &(task->activeTaskListItem));
			common_listAddBack(&kSchedCPUState.kReadyTaskList[task->priority], &(task->activeTaskListItem));
		break;
		default:
			return;
		break;
	}
}

//WHAT THE HELL AM I DOING SOMEBODY PLEASE HELP ME
static inline void tasks_tickTasks()
{
	volatile struct kListItemStruct_t* temp = kSchedCPUState.kSleepingTaskList.head;

	while (temp != NULL) {
		if (((kTaskHandle_t)(temp->data))->sleepTime) {
			((kTaskHandle_t)(temp->data))->sleepTime--;
		}
		else {
			tasks_rescheduleTask((kTaskHandle_t)temp->data, KSTATE_READY);
			((kTaskHandle_t)(temp->data))->state = KSTATE_READY;
		}
		temp = temp->next;
	}
}

static inline void tasks_search()
{
	for (kIterator_t i = CFG_NUMBER_OF_PRIORITIES-1; i >= 0; i--) {
		if (kSchedCPUState.kReadyTaskList[i].head != NULL) {

			#if CFG_MEMORY_PROTECTION_MODE != 0
			//if (memory_pointerSanityCheck((void*)kSchedCPUState.kReadyTaskList[i].head) != 0) {
				//kernel_panic(PSTR("Memory access violation in scheduler: priorityQueues.head is out of bounds\r\n"));
			//}
			#endif

			tasks_setNextTask(kSchedCPUState.kReadyTaskList[i].head->data);
			kSchedCPUState.kTaskActiveTicks = CFG_TICKS_PER_TASK;

			volatile struct kListItemStruct_t* temp = kSchedCPUState.kReadyTaskList[i].head;
			common_listDropFront(&kSchedCPUState.kReadyTaskList[i]);
			common_listAddBack(&kSchedCPUState.kReadyTaskList[i], temp);
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