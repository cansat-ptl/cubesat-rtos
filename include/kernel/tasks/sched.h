/*
 * sched.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_SCHED_H_
#define KERNEL_SCHED_H_

#include <kernel/types.h>
#include <kernel/common/lists.h>
#include <kernel/config.h>

struct kSchedCPUStateStruct_t
{
	kTask_t *kCurrentTask;
	kTask_t *kNextTask;

	kBaseType_t kTickRate;
	kTaskTicks_t kTaskActiveTicks;

	kLinkedList_t kReadyTaskList[CFG_NUMBER_OF_PRIORITIES];
	kLinkedList_t kSleepingTaskList;
	kLinkedList_t kSuspendedTaskList;
};

#ifdef __cplusplus
extern "C" {
#endif

void tasks_initScheduler(kTask_t *idle);

void tasks_scheduleTask(kTask_t *task, kTaskState_t state);
void tasks_unscheduleTask(kTask_t *task);

kTask_t *tasks_getCurrentTask();
kSysTicks_t tasks_getSysTickCount();

void tasks_sleep(kTaskTicks_t sleep);

void tasks_switchTask();
void tasks_tick();

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_SCHED_H_ */