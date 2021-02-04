/*
 * taskfields.c
 *
 * Created: 18.12.2020 6:06:10
 *  Author: Admin
 */


#include <types.h>
#include <config.h>
#include <common.h>
#include <tasks/tasks.h>
#include <tasks/scheduler.h>
#include <tasks/dispatcher.h>
#include <mem/heap.h>
#include <mem/protection.h>
#include <arch/arch.h>

void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state)
{
	if (task != NULL) {
		arch_enterCriticalSection();

		tasks_updateSchedulingList(task, state);
		task->state = state;

		arch_exitCriticalSection();
	}
}

kTaskState_t tasks_getTaskState(kTaskHandle_t task) 
{
	kTaskState_t state = KSTATE_UNINIT;

	if (task != NULL) {
		state = task->state;
	}

	return state;
}

kReturnValue_t tasks_setTaskPriority(kTaskHandle_t task, kBaseType_t priority)
{
	kReturnValue_t kresult = KRESULT_ERR_NULLPTR;

	if (task != NULL) {
		if (priority <= CFG_NUMBER_OF_PRIORITIES) {
			arch_enterCriticalSection();

			task->priority = priority;
			if (task->state == KSTATE_READY) {
				tasks_updateSchedulingList(task, KSTATE_READY);
			}
			kresult = KRESULT_SUCCESS;

			arch_exitCriticalSection();
		}
		else {
			kresult = CFG_NUMBER_OF_PRIORITIES;
		}
	}

	return kresult;
}

kBaseType_t tasks_getTaskPriority(kTaskHandle_t task) 
{
	kBaseType_t priority = 0;

	if (task != NULL) {
		priority = task->priority;
	}

	return priority;
}

