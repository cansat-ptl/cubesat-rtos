/*
 * events.h
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

uint16_t tasks_notificationWait();
void tasks_notificationSend(kTask_t *taskToNotify, uint16_t flags);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_EVENTS_H_ */