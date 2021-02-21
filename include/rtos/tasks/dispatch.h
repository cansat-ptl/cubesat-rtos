/*
 * dispatch.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_DISPATCHER_H_
#define KERNEL_DISPATCHER_H_

#include <rtos/types.h>

kTaskHandle_t tasks_getCurrentTask();
void tasks_setCurrentTask(kTaskHandle_t taskHandle);

kTaskHandle_t tasks_getNextTask();
void tasks_setNextTask(kTaskHandle_t taskHandle);

void tasks_sleep(kTaskTicks_t sleep);

void tasks_switchTask();

void tasks_tick();

#endif