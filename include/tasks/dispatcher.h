/*
 * dispatcher.h
 *
 * Created: 18.12.2020 23:31:37
 *  Author: Admin
 */


#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include <types.h>

kTaskHandle_t tasks_getCurrentTask();
void tasks_setCurrentTask(kTaskHandle_t taskHandle);

kTaskHandle_t tasks_getNextTask();
void tasks_setNextTask(kTaskHandle_t taskHandle);

void tasks_sleep(kTaskTicks_t sleep);

#endif /* DISPATCHER_H_ */