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
	mem_heapInit();
	tasks_init();

	arch_DISABLE_INTERRUPTS();

	debug_printk("[init] Init OK\r\n");
	debug_printk("[init] Starting yktsat-rtos " KERNEL_VERSION "\r\n");

	arch_ENABLE_INTERRUPTS();
}