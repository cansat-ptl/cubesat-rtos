/*
 * init.c
 *
 * Created: 17.12.2020 20:49:07
 *  Author: ThePetrovich
 */


#include <kernel/kernel.h>
#include <kernel/mem/heap.h>
#include <kernel/types.h>
#include <kernel/debug/printk.h>
#include <kernel/arch/arch.h>
#include <kernel/tasks/tasks.h>

void kernel_init()
{	
	arch_platformInit();
	debug_printk("[init] Board init OK\r\n");

	mem_heapInit();
	debug_printk("[init] Memmgr init OK\r\n");

	tasks_init();
	debug_printk("[init] Taskmgr init OK\r\n");

	debug_printk("[init] yktsat-rtos " VERSION_STRING " arch " KERNEL_ARCH "/" KERNEL_MCU "\r\n");
}

void kernel_startScheduler()
{
	debug_printk("[init] Starting scheduler\r\n");
	arch_startScheduler();
}