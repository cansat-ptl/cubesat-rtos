/*
 * heap.h
 * 
 * Created: 08.02.2021 11:06:19
 * Author: ThePetrovich
 */


#ifndef KERNEL_TASKHEAP_H_
#define KERNEL_TASKHEAP_H_

#include <rtos/types.h>

void* tasks_malloc(size_t size);
void tasks_free(void* pointer);

#endif