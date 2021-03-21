/*
 * hooks.c
 * 
 * Created: 20.03.2021 01:47:12
 * Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/hooks.h>
#include <kernel/panic.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>

void kernel_taskReturnHook()
{
    kTask_t *currentTask = tasks_getCurrentTask();

	tasks_deleteTask(currentTask);

	while (1) {
		; /* Do nothing */
	}
}
