/*
 * config.h
 *
 * Created: 17.12.2020 22:20:51
 *  Author: Admin
 */


#ifndef KERNELCONFIG_H_
#define KERNELCONFIG_H_

	#define CFG_NUMBER_OF_PRIORITIES 8
	#define CFG_TICKS_PER_TASK 5

	#define CFG_MIN_TASK_STACK_SIZE 60

	/* Memory protection settings */
	#define CFG_MEMORY_PROTECTION_MODE 3 /* 0 - disabled, 1 - check task stack bounds only, 2 - check task stack protection regions only, 3 - check both */
	#define CFG_STACK_SAFETY_MARGIN 16
	#define CFG_KERNEL_RESERVED_MEMORY 100
	#define CFG_KERNEL_IDLE_TASK_MEMORY 100
	#define CFG_STACK_GROWTH_DIRECTION -1 /* -1 - towards lower addresses, 1 - towards higher */

	#define CFG_PROTECT_FROM_INVALID_HEAP_FREE 1

	#define CFG_KERNEL_PANIC_ACTION 3 /* 0 - restart, 1 - print error details and restart, 2 - print error details and halt */
	#define CFG_STACK_CORRUPTION_ACTION 1 /* 0 - panic, 1 - restart affected task, panic if critical, 2 - restart all tasks */
	#define CFG_TASK_RETURN_ACTION 1 /* 0 - panic, 1 - delete affected task, panic if critical */

	/* Memory manager settings */
	#define CFG_HEAP_SIZE 2000
	#define CFG_PLATFORM_BYTE_ALIGNMENT 1
	#define CFG_PLATFORM_BYTE_ALIGNMENT_MASK 0x0000
	#define CFG_MIN_BLOCK_SIZE 4

	/* Timer settings */
	#define CFG_TICKRATE_MS 1

	/* Debug logging options */
	#define CFG_LOGGING 1
	#define CFG_VERBOSE 0
	#define CFG_PROFILING 0
	#define CFG_DEBUG_PRINT_STAGES 1
	#define CFG_DEBUG_PRINT_LEVELS 1

	#define CFG_KERNEL_TIMER_PRESCALER 3

#endif /* CONFIG_H_ */