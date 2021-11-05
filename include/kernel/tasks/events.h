/**
 * @file events.h
 * @brief Kernel task-to-task events.
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_EVENTS_H_
#define KERNEL_EVENTS_H_

#include <stdint.h>
#include <kernel/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup tasks
 * @brief Waits for any event from any task.
 * When there are no events pending, sets task state to KSTATE_SUSPENDED.
 * As soon as an event arrives, task state is changed to KSTATE_READY.
 * 
 * @return kBaseType_t - user flags passed via event.
 */
kBaseType_t tasks_eventWait();

/**
 * @ingroup tasks
 * @brief Sends event to task.
 * When the event is sent, target task state is set to KSTATE_READY if 
 * its current state is KSTATE_SUSPENDED.
 * 
 * @param task Target task.
 * @param flags User data.
 * @remark Can be used in interrupts.
 */
void tasks_eventSend(kTask_t *task, kBaseType_t flags);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_EVENTS_H_ */