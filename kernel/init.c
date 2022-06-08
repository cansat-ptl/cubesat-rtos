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
	debug_printk_p(ROMSTR("init: Board init:                                 [OK]\r\n"));
	debug_printk_p(ROMSTR("init: Platform byte alignment = %d\r\n"), CFG_PLATFORM_BYTE_ALIGNMENT);
	debug_printk_p(ROMSTR("init: Platform byte alignment mask = 0x%04X\r\n"), CFG_PLATFORM_BYTE_ALIGNMENT_MASK);
	debug_printk_p(ROMSTR("init: Platform reserved memory = %d\r\n"), CFG_KERNEL_RESERVED_MEMORY);
	debug_printk_p(ROMSTR("init: Platform stack growth direction = %d\r\n"), CFG_STACK_GROWTH_DIRECTION);

	mem_heapInit();
	debug_printk_p(ROMSTR("init: Memory manager init                         [OK]\r\n"));
	debug_printk_p(ROMSTR("init: Heap region size = %d\r\n"), CFG_HEAP_SIZE);
	debug_printk_p(ROMSTR("init: Heap alloc struct size = %d\r\n"), mem_HEAP_STRUCT_SIZE);
	debug_printk_p(ROMSTR("init: Heap min block size = %d\r\n"), CFG_MIN_BLOCK_SIZE);

	tasks_init();
	debug_printk_p(ROMSTR("init: Task manager init:                          [OK]\r\n"));
	debug_printk_p(ROMSTR("init: Task struct size = %d\r\n"), tasks_TASK_STRUCT_SIZE);
	debug_printk_p(ROMSTR("init: Task stack min size = %d\r\n"), CFG_MIN_TASK_STACK_SIZE);
	debug_printk_p(ROMSTR("init: Idle task stack size = %d\r\n"), CFG_KERNEL_IDLE_TASK_MEMORY);
	debug_printk_p(ROMSTR("init: Task stack protection region size = %d\r\n"), CFG_STACK_SAFETY_MARGIN);
	debug_printk_p(ROMSTR("init: Number of task priorities = %d\r\n"), CFG_NUMBER_OF_PRIORITIES);
	debug_printk_p(ROMSTR("init: Ticks per task = %d\r\n"), CFG_TICKS_PER_TASK);
	debug_printk_p(ROMSTR("init: Kernel build options:\r\n"), CFG_NUMBER_OF_PRIORITIES);
	debug_printk_p(ROMSTR("      Memory protection mode = %d\r\n"), CFG_MEMORY_PROTECTION_MODE);
	debug_printk_p(ROMSTR("      Kernel panic action = %d\r\n"), CFG_KERNEL_PANIC_ACTION);
	debug_printk_p(ROMSTR("      Task return action = %d\r\n"), CFG_TASK_RETURN_ACTION);
	debug_printk_p(ROMSTR("      Child task tracking = %d\r\n"), CFG_ENABLE_CHILD_TASK_TRACKING);
	debug_printk_p(ROMSTR("      Task memory allocation tracking = %d\r\n"), CFG_HEAP_ALLOCATION_TRACKING);
	debug_printk_p(ROMSTR("      Use platform vendor libc = %d\r\n"), CFG_USE_PLATFORM_LIBC_IN_KERNEL);

	debug_printk_p(ROMSTR("init: yktsat-rtos " VERSION_STRING " arch " KERNEL_ARCH "/" KERNEL_MCU "\r\n"));
}

void kernel_startScheduler()
{
	debug_printk_p(ROMSTR("init: Starting scheduler\r\n"));
	arch_startScheduler();
}