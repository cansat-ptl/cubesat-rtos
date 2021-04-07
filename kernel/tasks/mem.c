/*
 * heap.c
 * 
 * Created: 08.02.2021 10:59:07
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/tasks/mem.h>
#include <kernel/common/heap.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>

void *tasks_malloc(size_t size) 
{
	kTask_t *currentTask = tasks_getCurrentTask();
	kLinkedList_t *allocList = NULL;

	if (currentTask != NULL) {
		allocList = &(currentTask->allocList);
	}

	return common_heapAlloc(size, allocList);
}

void tasks_free(void *pointer) 
{
	common_heapFree(pointer);
}

kReturnValue_t tasks_checkStackBounds(kTask_t *task) 
{
	kReturnValue_t exitcode = KRESULT_SUCCESS;

	if (task->stackPtr < task->stackBegin || task->stackPtr > task->stackBegin + task->stackSize) {
		exitcode = KRESULT_ERR_GENERIC;
	}

	return exitcode;
}
