/*
 * init.c
 *
 * Created: 17.12.2020 20:49:07
 *  Author: ThePetrovich
 */


#include <kernel/types.h>
#include <kernel/arch/arch.h>
#include <kernel/tasks/tasks.h>

void kernel_init()
{
	arch_platformInit();
	tasks_init();
	uart_puts("Hello from OS init!\r\n");
}