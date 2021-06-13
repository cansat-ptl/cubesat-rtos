/*
 * init.c
 *
 * Created: 17.12.2020 20:49:07
 *  Author: ThePetrovich
 */


#include <kernel/kernel.h>
#include <kernel/types.h>
#include <kernel/debug/printk.h>
#include <kernel/arch/arch.h>
#include <kernel/tasks/tasks.h>

void kernel_init()
{	
	arch_platformInit();

	debug_printk("[init] Board init OK\r\n");
	debug_printk("[init] Starting yktsat-rtos " KERNEL_VERSION "\r\n");

	mem_heapInit();

	debug_printk("[init] Memmgr init OK\r\n");

	tasks_init();

	debug_printk("[init] Taskmgr init OK\r\n");
}