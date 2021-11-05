 /**
 * @file panic.h
 * @brief Kernel errors.
 * 
 * @date 20.03.2021 02:02:59
 * @author ThePetrovich
 */


#ifndef KERNEL_PANIC_H_
#define KERNEL_PANIC_H_

/**
 * @ingroup kernel
 * @brief Displays error message and stops the kernel. 
 * Depending on CFG_KERNEL_PANIC_ACTION setting in config.h, will either restart or halt the cpu.
 * 
 * @param msg Error message.
 */
void kernel_panic(char *msg);

#endif