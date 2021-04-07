/*
 * events.c
 *
 * Created: 18.12.2020 6:06:21
 *  Author: Admin
 */ 


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>
#include <kernel/tasks/mem.h>
#include <kernel/arch/arch.h>
#include <kernel/common/lists.h>
#include <kernel/debug/printk.h>
#include <kernel/panic.h>

uint16_t tasks_notificationWait()
{
	uint16_t returnValue = 0;
	while (1) {
		arch_enterCriticalSection();
		kTask_t *currentTask = tasks_getCurrentTask();

		if (currentTask->event.state == KEVENT_FIRED) {
			currentTask->event.state = KEVENT_NONE;
			returnValue = currentTask->event.eventFlags;
			arch_exitCriticalSection();
			break;
		}
		else {
			tasks_setTaskState(currentTask, KSTATE_SUSPENDED);
			arch_exitCriticalSection();
			tasks_sleep(0);
		}
	}
	return returnValue;
}

void tasks_notificationSend(kTask_t *taskToNotify, uint16_t flags)
{
	if (taskToNotify != NULL) {
		arch_enterCriticalSection();

		if (taskToNotify->state == KSTATE_SUSPENDED) {
			tasks_setTaskState(taskToNotify, KSTATE_READY);
		}

		taskToNotify->event.state = KEVENT_FIRED;
		taskToNotify->event.eventFlags = flags;

		arch_exitCriticalSection();
	}

	return;
}