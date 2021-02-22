/*
 * heap.c
 * 
 * Created: 08.02.2021 10:59:07
 * Author: ThePetrovich
 */


#include <rtos/types.h>
#include <rtos/common/heap.h>
#include <rtos/tasks/tasks.h>
#include <rtos/tasks/sched.h>
#include <rtos/arch/arch.h>

void* tasks_malloc(size_t size) 
{
    kTaskHandle_t currentTask = tasks_getCurrentTask();
    kLinkedList_t* allocList = NULL;

    if (currentTask != NULL) {
        allocList = &(currentTask->allocList);
    }

    return common_heapAlloc(size, allocList);
}

void tasks_free(void* pointer) 
{
    common_heapFree(pointer);
}