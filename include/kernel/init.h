/*
 * init.h
 * 
 * Created: 24.01.2021 08:31:50
 * Author: ThePetrovich
 */


#ifndef KERNEL_INIT_H_
#define KERNEL_INIT_H_

/**
 * @brief Initializes internal kernel structures.
 * Note: scheduler will NOT run after you call this, see kernel_startScheduler().
 */
void kernel_init();

/**
 * @brief Enables systick timer and starts the scheduler.
 */
void kernel_startScheduler();

#endif