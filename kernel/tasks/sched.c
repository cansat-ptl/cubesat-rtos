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
#include <kernel/arch/arch.h>
#include <kernel/common/lists.h>
#include <kernel/debug/printk.h>
#include <kernel/panic.h>

volatile struct kSchedCPUStateStruct_t kSchedCPUState; /* Must not be static - also used by arch/../context.S */

static volatile kSysTicks_t kTicks = 0;

void tasks_initScheduler(kTask_t *idle)
{
	kSchedCPUState.kReadyTaskList[0].head = &(idle->activeTaskListItem);
	kSchedCPUState.kReadyTaskList[0].tail = &(idle->activeTaskListItem);
	kSchedCPUState.kNextTask = idle;
	kSchedCPUState.kCurrentTask = idle;
}

kTask_t *tasks_getCurrentTask()
{
	arch_enterCriticalSection();

	kTask_t *currentTask = kSchedCPUState.kCurrentTask;

	arch_exitCriticalSection();
	return currentTask;
}

kSysTicks_t tasks_getSysTickCount()
{
	arch_enterCriticalSection();

	kSysTicks_t ticks = (kSysTicks_t)kTicks;

	arch_exitCriticalSection();
	return ticks;
}

void tasks_scheduleTask(kTask_t *task, kTaskState_t state)
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

void tasks_unscheduleTask(kTask_t *task)
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
		if (head->data != NULL) {
			if (((kTask_t *)(head->data))->sleepTime) {
				((kTask_t *)(head->data))->sleepTime--;
			}
			else {
				tasks_setTaskState(head->data, KSTATE_READY);
			}
			head = head->next;
		}
		else {
			kernel_panic("tickTasks: head->data = NULL");
		}
	}
}

static inline void tasks_search()
{
	kLinkedListItem_t *head = NULL;

	for (kIterator_t i = CFG_NUMBER_OF_PRIORITIES-1; i >= 0; i--) {
		head = kSchedCPUState.kReadyTaskList[i].head;
		if (head != NULL) {
			kSchedCPUState.kNextTask = (kTask_t *)head->data;
			kSchedCPUState.kTaskActiveTicks = CFG_TICKS_PER_TASK;

			common_listDropFront((kLinkedList_t *)&kSchedCPUState.kReadyTaskList[i]);
			common_listAddBack((kLinkedList_t *)&kSchedCPUState.kReadyTaskList[i], head);
			break;
		}
	}
}

static inline void tasks_switchContext()
{	
	kTask_t *task = kSchedCPUState.kCurrentTask;

	if ((arch_checkProtectionRegion(task->stackBegin, task->stackSize, CFG_STACK_SAFETY_MARGIN) != KRESULT_SUCCESS \
		|| tasks_checkStackBounds(task) != KRESULT_SUCCESS) \
		&& task->pid != 0) {
		kernel_panic("Task stack corruption");
	}

	if (kSchedCPUState.kNextTask == NULL) {
		kernel_panic("kNextTask is NULL");
	}

	kSchedCPUState.kCurrentTask = kSchedCPUState.kNextTask;
}

/* Note: must not be static, called in arch module */
inline void tasks_switchTask()
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

/* Note: must not be static, called in arch module */
inline void tasks_tick()
{
	kTicks++;
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