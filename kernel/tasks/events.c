/*
 * events.c
 *
 * Created: 18.12.2020 6:06:21
 *  Author: ThePetrovich
 */ 


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>
#include <kernel/arch/arch.h>

kBaseType_t tasks_eventWait()
{
	kBaseType_t returnValue = 0;

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

void tasks_eventSend(kTask_t *task, kBaseType_t flags)
{
	if (task != NULL) {
		arch_enterCriticalSection();

		if (task->state == KSTATE_SUSPENDED) {
			tasks_setTaskState(task, KSTATE_READY);
		}

		task->event.state = KEVENT_FIRED;
		task->event.eventFlags = flags;

		arch_exitCriticalSection();
	}

	return;
}
