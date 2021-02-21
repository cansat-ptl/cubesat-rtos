/*
 * init.c
 *
 * Created: 17.12.2020 20:49:07
 *  Author: ThePetrovich
 */


#include <rtos/types.h>
#include <rtos/arch/arch.h>
#include <rtos/tasks/tasks.h>

void kernel_init()
{
	arch_platformInit();
	tasks_init();
	uart_puts("Hello from OS init!\r\n");
}