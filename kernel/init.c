/*
 * init.c
 *
 * Created: 17.12.2020 20:49:07
 * Author: ThePetrovich
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
	debug_printk_p(ROMSTR("[init] Board init 				[OK]\r\n"));
	debug_printk_p(ROMSTR("[init] Platform byte alignment = %d\r\n"), CFG_PLATFORM_BYTE_ALIGNMENT);
	debug_printk_p(ROMSTR("[init] Platform byte alignment mask = %d\r\n"), CFG_PLATFORM_BYTE_ALIGNMENT_MASK);

	mem_heapInit();
	debug_printk_p(ROMSTR("[init] Memmgr init 				[OK]\r\n"));
	debug_printk_p(ROMSTR("[init] Heap region size = %d\r\n"), CFG_HEAP_SIZE);
	debug_printk_p(ROMSTR("[init] Heap alloc struct size = %d\r\n"), mem_HEAP_STRUCT_SIZE);
	debug_printk_p(ROMSTR("[init] Heap min block size = %d\r\n"), CFG_MIN_BLOCK_SIZE);

	tasks_init();
	debug_printk_p(ROMSTR("[init] Taskmgr init 				[OK]\r\n"));
	debug_printk_p(ROMSTR("[init] Task struct size = %d\r\n"), tasks_TASK_STRUCT_SIZE);
	debug_printk_p(ROMSTR("[init] Task stack min size = %d\r\n"), CFG_MIN_TASK_STACK_SIZE);
	debug_printk_p(ROMSTR("[init] Number of task priorities = %d\r\n"), CFG_NUMBER_OF_PRIORITIES);

	debug_printk_p(ROMSTR("[init] yktsat-rtos " VERSION_STRING " arch " KERNEL_ARCH "/" KERNEL_MCU "\r\n"));
}

void kernel_startScheduler()
{
	debug_printk_p(ROMSTR("[init] Starting scheduler\r\n"));
	arch_startScheduler();
}