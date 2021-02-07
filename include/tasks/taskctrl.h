/*
 * taskctrl.h
 * 
 * Created: 08.02.2021 03:15:06
 * Author: ThePetrovich
 */


#ifndef KERNEL_TASKCONTROL_H_
#define KERNEL_TASKCONTROL_H_

#include <types.h>

void tasks_setTaskState(kTaskHandle_t task, kTaskState_t state);
kTaskState_t tasks_getTaskState(kTaskHandle_t task);

kReturnValue_t tasks_setTaskPriority(kTaskHandle_t task, kBaseType_t priority);
kBaseType_t tasks_getTaskPriority(kTaskHandle_t task);

kReturnValue_t tasks_createTaskStatic(kStackPtr_t taskMemory, kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, kBaseType_t priority, kTaskType_t type, char* name);
kReturnValue_t tasks_createTaskDynamic(kTaskHandle_t* handle, kTask_t entry, void* args, kStackSize_t stackSize, kBaseType_t priority, kTaskType_t type, char* name);

void tasks_blockTask(kTaskHandle_t task, volatile struct kLinkedListStruct_t* blockList);
void tasks_unblockTask(kTaskHandle_t task);

#endif