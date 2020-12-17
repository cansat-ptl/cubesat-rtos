/*
 * config.h
 *
 * Created: 17.12.2020 22:20:51
 *  Author: Admin
 */


#ifndef CONFIG_H_
#define CONFIG_H_

	#define CFG_NUMBER_OF_PRIORITIES 8
	#define CFG_TICKS_PER_TASK 5

	//Memory protection settings
	#define CFG_MEMORY_PROTECTION_MODE 3
	#define CFG_ALLOW_STATIC_TASK_ALLOCATION 0
	#define CFG_STACK_SAFETY_MARGIN 16
	#define CFG_KERNEL_RESERVED_MEMORY 300
	#define CFG_REGISTER_RESERVED_SPACE 50
	#define CFG_STACK_GROWTH_DIRECTION 0 //0 - towards lower addresses, 1 - towards higher

	#define CFG_KERNEL_PANIC_ACTION 1

	//Memory manager settings
	#define CFG_HEAP_SIZE 2500
	#define CFG_PLATFORM_BYTE_ALIGNMENT 1
	#define CFG_PLATFORM_BYTE_ALIGNMENT_MASK 0x0000
	#define CFG_MIN_BLOCK_SIZE 4

	//Timer settings
	#define CFG_TICKRATE_MS 1

	//Debug logging options
	#define CFG_LOGGING 1
	#define CFG_VERBOSE 0
	#define CFG_PROFILING 0
	#define CFG_DEBUG_PRINT_STAGES 1
	#define CFG_DEBUG_PRINT_LEVELS 1

	#define CFG_KERNEL_TIMER_PRESCALER 3

#endif /* CONFIG_H_ */