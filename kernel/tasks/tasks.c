/*
 * tasks.c
 * 
 * Created: 04.02.2021 08:26:28
 * Author: ThePetrovich
 */


#include <types.h>
#include <config.h>
#include <common.h>
#include <tasks/tasks.h>
#include <tasks/sched.h>
#include <tasks/dispatch.h>
#include <tasks/taskctrl.h>
#include <mem/module.h>
#include <arch/module.h>

byte kIdleMem[150];

void idle0() {
	while(1) {
		;
	}
}

kReturnValue_t tasks_init()
{
	memory_heapInit();
	kTaskHandle_t idleTask;
	tasks_createTaskStatic(kIdleMem, &idleTask, idle0, NULL, 64, 0, KTASK_CRITICAL_STATIC, "idle");

	if (idleTask == NULL) {
		//debug_logMessage(PGM_PUTS, L_FATAL, PSTR("\r\ntaskmgr: Startup failed, could not create idle task.\r\n"));
		while(1);
	}

	tasks_initScheduler(idleTask);
	tasks_setCurrentTask(idleTask);
	tasks_setNextTask(idleTask);

	return 0;
}