/*
 * scheduler.h
 *
 * Created: 18.12.2020 6:48:16
 *  Author: Admin
 */


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <types.h>
#include <common.h>

void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state);

struct kSchedCPUStateStruct_t
{
	kTaskHandle_t kCurrentTask;
	kTaskHandle_t kNextTask;

	volatile uint8_t kTickRate;
	volatile kTaskTicks_t kTaskActiveTicks;

	volatile struct kLinkedListStruct_t kReadyTaskList[CFG_NUMBER_OF_PRIORITIES];
	volatile struct kLinkedListStruct_t kSleepingTaskList;
	volatile struct kLinkedListStruct_t kSuspendedTaskList;
};

void tasks_initScheduler(kTaskHandle_t idle);

void tasks_rescheduleTask(kTaskHandle_t task, kTaskState_t state);

void tasks_runScheduler();

#endif /* SCHEDULER_H_ */