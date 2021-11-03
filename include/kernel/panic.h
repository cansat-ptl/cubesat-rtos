/*
 * panic.h
 * 
 * Created: 20.03.2021 02:02:59
 * Author: ThePetrovich
 */


#ifndef KERNEL_PANIC_H_
#define KERNEL_PANIC_H_

/**
 * @brief Displays error message and stops the kernel. 
 * Depending on CFG_KERNEL_PANIC_ACTION setting in config.h, will either restart or halt the cpu.
 * 
 * @param msg Error message.
 */
void kernel_panic(char *msg);

#endif