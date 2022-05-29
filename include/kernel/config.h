/**
 * @file config.h
 * @brief Kernel configuration. 
 * 
 * @date 17.12.2020 22:20:51
 * @author ThePetrovich
 */


#ifndef KERNELCONFIG_H_
#define KERNELCONFIG_H_

/** @addtogroup config
 *  @brief Kernel configuration
 *  @ingroup kernel
 *  @{ */
	/** Number of priorities for scheduling. Setting this to high values will increase
	 * memory consumption. 8 is recommended. */
	#define CFG_NUMBER_OF_PRIORITIES 8

	/** Sheduling time quantum size in ticks. Controls how long will task run before 
	 * getting pre-empted. */
	#define CFG_TICKS_PER_TASK 1

	/** Minimum task stack size. For AVR, leave this at 60. */
	#define CFG_MIN_TASK_STACK_SIZE 60

	/** Memory protection mode.
	 * @param 0 Memory protection disabled.
	 * @param 1 Check stack bounds only.
	 * @param 2 Check protection regions only.
	 * @param 3 Check everything. */
	#define CFG_MEMORY_PROTECTION_MODE 3

	/** When CFG_MEMORY_PROTECTION_MODE is set to 2 or 3, controls length of
	 * memory protection region in bytes. */
	#define CFG_STACK_SAFETY_MARGIN 8

	/** Memory reserved for kernel operations (ISRs, syscalls, etc.). */
	#define CFG_KERNEL_RESERVED_MEMORY 200

	/** Idle task memory size. Must not be less than CFG_MIN_TASK_STACK_SIZE. */
	#define CFG_KERNEL_IDLE_TASK_MEMORY 200

	/** Stack growth direction for selected architecture. Do not change unless you
	 * know what you are doing. -1 is towards lower addresses, 1 towards higher. */
	#define CFG_STACK_GROWTH_DIRECTION -1

	/** Kernel panic action.
	 * @param 0 Reset without printing the error message.
	 * @param 1 Print error details and reset.
	 * @param 2 Print error details and halt. 
	 * @param 3 Do nothing */
	#define CFG_KERNEL_PANIC_ACTION 3

	/** Action to take when a task exits.
	 * @param 0 Throw kernel panic.
	 * @param 1 Delete exited task, panic if the task was KTASK_CRITICAL. */
	#define CFG_TASK_RETURN_ACTION 1

	/** Kernel heap size. Used to dynamically allocate tasks and kernel objects.
	 * See mem module for more info. */
	#define CFG_HEAP_SIZE 2500

	/** Archiecture byte alignment. Leave 1 for AVR. */
	#define CFG_PLATFORM_BYTE_ALIGNMENT 8

	/** Architecture byte alignment mask. Leave 0 for AVR. */
	#define CFG_PLATFORM_BYTE_ALIGNMENT_MASK 0x0007

	/** Minimum heap allocation block size. */
	#define CFG_MIN_BLOCK_SIZE 4

	#define CFG_HEAP_ALLOCATION_TRACKING 1

	/** Systick interrupts per 1 kernel tick. */
	#define CFG_TICKRATE_MS 1
	
	/** If set to 0, kernel will use its own libc implementation from
	 * common module. */
	#define CFG_USE_PLATFORM_LIBC_IN_KERNEL 0

	/** Systick timer prescaler value. */
	#define CFG_KERNEL_TIMER_PRESCALER 3

	#define CFG_ENABLE_SLEEP_WORKAROUND 0
/**  @} */

#endif