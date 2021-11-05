/**
 * @file sched.h
 * @brief Kernel scheduling control functions.
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_SCHED_H_
#define KERNEL_SCHED_H_

#include <kernel/types.h>
#include <kernel/common/lists.h>
#include <kernel/config.h>

/**
 * @ingroup tasks
 * @brief Main scheduler structure. 
 * Contains scheduler internal state information for a single processor.
 */
struct kSchedCPUStateStruct_t
{
	kTask_t *currentTask; 			/**< Task currently executed by the processor. 
							Used for context switching.*/
	kTask_t *nextTask;			/**< Next task to be executed by the processor.
							Used for context switching.*/

	kBaseType_t quantumTicksLeft;		/**< Ticks left for the current time quantum. */	
	kTaskTicks_t taskQuantumLeft;		/**< Time quantums left for the current task to execute. */

	/** Priority queue for tasks in KSTATE_READY state. Part of O(1) scheduler implementation. */
	kLinkedList_t readyTaskList[CFG_NUMBER_OF_PRIORITIES];	
	kLinkedList_t sleepingTaskList;		/**< List of tasks in KSTATE_SLEEPING state. */
	kLinkedList_t suspendedTaskList;	/**< List of tasks in KSTATE_SUSPENDED state. */
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup tasks
 * @brief Initializes internal scheduler structures.
 * 
 * @param idle Idle task handle.
 */
void tasks_initScheduler(kTask_t *idle);

/**
 * @ingroup tasks
 * @brief Adds task to the corresponding state queue and prepares it for scheduling.
 * 
 * @param task Task to schedule.
 * @param state Initial task state. Allowed values are KSTATE_SUSPENDED, 
 * KSTATE_READY, KSTATE_SLEEPING.
 * @warning Passing KSTATE_UNINIT will cause kernel panic.
 */
void tasks_scheduleTask(kTask_t *task, kTaskState_t state);

/**
 * @ingroup tasks
 * @brief Removes task from all scheduling queues and prevents it from running.
 * 
 * @param task Task to unschedule.
 */
void tasks_unscheduleTask(kTask_t *task);

/**
 * @ingroup tasks
 * @brief Returns task currently executed by the processor.
 * 
 * @return kTask_t* - current task handle.
 */
kTask_t *tasks_getCurrentTask();

/**
 * @ingroup tasks
 * @brief Returns number of ticks since kernel startup.
 * 
 * @return kSysTicks_t - number of ticks.
 */
kSysTicks_t tasks_getSysTickCount();

/**
 * @ingroup tasks
 * @brief Puts current task to sleep for N ticks. The task will resume
 * execution automatically after N ticks have passed.
 * 
 * @param sleep Task sleep time in ticks.
 */
void tasks_sleep(kTaskTicks_t sleep);

/**
 * @ingroup tasks
 * @brief Performs context switch. Used in arch module.
 * @note For in-kernel use only. 
 */
void tasks_switchTask();

/**
 * @ingroup tasks
 * @brief Systick interrupt service routine. Used in arch module.
 * @note For in-kernel use only.
 */
void tasks_tick();

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_SCHED_H_ */