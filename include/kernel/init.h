/**
 * @file init.h
 * @brief Kernel startup and initialization.
 * 
 * @date 24.01.2021 08:31:50
 * @author ThePetrovich
 */


#ifndef KERNEL_INIT_H_
#define KERNEL_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup kernel
 * @brief Initializes internal kernel structures.
 * @note Scheduler will NOT run after you call this, see kernel_startScheduler().
 */
void kernel_init();

/**
 * @ingroup kernel
 * @brief Enables systick timer and starts the scheduler. Needs to be called after
 * kernel_init().
 */
void kernel_startScheduler();

#ifdef __cplusplus
}
#endif

#endif