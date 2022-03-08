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
#include <util/atomic.h>

volatile kBaseType_t kCriticalNesting = 0;		/* Must not be static - also used by arch/../arch.c */
volatile struct kSchedCPUStateStruct_t kSchedCPUState;	/* Must not be static - also used by arch/../context.S */

static volatile kSysTicks_t kTicks = 0;

void tasks_initScheduler(kTask_t *idle)
{
	kSchedCPUState.readyTaskList[0].head = &(idle->activeTaskListItem);
	kSchedCPUState.readyTaskList[0].tail = &(idle->activeTaskListItem);
	kSchedCPUState.nextTask = idle;
	kSchedCPUState.currentTask = idle;
}

kTask_t *tasks_getCurrentTask()
{
	arch_enterCriticalSection();

	kTask_t *currentTask = kSchedCPUState.currentTask;

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
			common_listAddBack((kLinkedList_t *)&kSchedCPUState.suspendedTaskList, &(task->activeTaskListItem));
			break;
		case KSTATE_SLEEPING:
			common_listAddBack((kLinkedList_t *)&kSchedCPUState.sleepingTaskList, &(task->activeTaskListItem));
			break;
		case KSTATE_READY:
			common_listAddBack((kLinkedList_t *)&kSchedCPUState.readyTaskList[task->priority], &(task->activeTaskListItem));
			break;
		case KSTATE_UNINIT:
			//kernel_panic("Uninitialized task scheduled");
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

static void tasks_tickSleeping()
{
	kLinkedListItem_t *head = kSchedCPUState.sleepingTaskList.head;

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
		head = kSchedCPUState.readyTaskList[i].head;
		if (head != NULL) {
			kSchedCPUState.nextTask = (kTask_t *)head->data;
			kSchedCPUState.taskQuantumLeft = CFG_TICKS_PER_TASK;

			common_listDropFront((kLinkedList_t *)&kSchedCPUState.readyTaskList[i]);
			common_listAddBack((kLinkedList_t *)&kSchedCPUState.readyTaskList[i], head);
			break;
		}
	}
}

static void tasks_switchContext()
{	
	kTask_t *task = kSchedCPUState.currentTask;

	if ((arch_checkProtectionRegion(task->stackBegin, task->stackSize, CFG_STACK_SAFETY_MARGIN) != KRESULT_SUCCESS \
		|| tasks_checkStackBounds(task) != KRESULT_SUCCESS) \
		&& task->pid != 0) {
		kernel_panic("Task stack corruption");
		debug_printk("Errored task handle: %x\r\n", task);
	}

	if (kSchedCPUState.nextTask == NULL) {
		kernel_panic("kNextTask is NULL");
	}

	kSchedCPUState.currentTask = kSchedCPUState.nextTask;
}

/* Note: must not be static, called in arch module */
void tasks_switchTask()
{
	if (!kSchedCPUState.quantumTicksLeft) {
		tasks_tickSleeping();
		kSchedCPUState.quantumTicksLeft = CFG_TICKRATE_MS;
		if (kSchedCPUState.taskQuantumLeft) {
			kSchedCPUState.taskQuantumLeft--;
		}
	}
	else {
		kSchedCPUState.quantumTicksLeft--;
	}

	if (kCriticalNesting == 0) {
		if (!kSchedCPUState.taskQuantumLeft) {
			tasks_search();
		}

		if (kSchedCPUState.nextTask != kSchedCPUState.currentTask) tasks_switchContext();
	}
}

/* Note: must not be static, called in arch module */
void tasks_tick()
{	
	kTicks++;
	tasks_switchTask();
}

void tasks_sleep(kTaskTicks_t sleep)
{
	arch_enterCriticalSection();

	kTask_t *task = kSchedCPUState.currentTask;
	kSchedCPUState.taskQuantumLeft = 0;
	kSchedCPUState.quantumTicksLeft = 0;

	if (sleep != 0) {
		task->sleepTime = sleep;
		tasks_setTaskState(task, KSTATE_SLEEPING);
	}
	#if CFG_ENABLE_SLEEP_WORKAROUND == 0
		arch_exitCriticalSection();
		arch_yield();
	#else
		arch_exitCriticalSection();
		arch_ENABLE_INTERRUPTS();
		while(task->state == KSTATE_SLEEPING);
	#endif
}