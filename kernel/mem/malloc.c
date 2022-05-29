/*
 * malloc.c
 * 
 * Created: 13.06.2021 10:37:33
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/mem/heap.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>

void *mem_malloc(size_t size) 
{
	kLinkedList_t *allocList = NULL;

	#if CFG_HEAP_ALLOCATION_TRACKING == 1
		kTask_t *currentTask = tasks_getCurrentTask();

		if (currentTask != NULL) {
			allocList = tasks_getTaskAllocList(currentTask);
		}
	#endif

	return mem_heapAlloc(size, allocList);
}

void mem_free(void *pointer) 
{
	mem_heapFree(pointer);
}
