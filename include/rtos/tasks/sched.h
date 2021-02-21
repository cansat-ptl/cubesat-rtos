/*
 * sched.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_SCHEDULER_H_
#define KERNEL_SCHEDULER_H_

#include <rtos/types.h>
#include <rtos/common/lists.h>
#include <rtos/config.h>

void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state);

struct kSchedCPUStateStruct_t
{
	kTaskHandle_t kCurrentTask;
	kTaskHandle_t kNextTask;

	volatile kBaseType_t kTickRate;
	volatile kTaskTicks_t kTaskActiveTicks;

	volatile kLinkedList_t kReadyTaskList[CFG_NUMBER_OF_PRIORITIES];
	volatile kLinkedList_t kSleepingTaskList;
	volatile kLinkedList_t kSuspendedTaskList;
};

void tasks_initScheduler(kTaskHandle_t idle);

void tasks_updateSchedulingList(kTaskHandle_t task, kTaskState_t state);

void tasks_runScheduler();

#endif