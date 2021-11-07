/**
 * @defgroup kernel
 * @brief General kernel control functions.
 * 
 * @file kernel.h
 * @brief Main kernel header for user applications. 
 * 
 * @date 08.02.2021 06:58:35
 * @author ThePetrovich
 */


#ifndef KERNEL_RTOS_H_
#define KERNEL_RTOS_H_

#ifndef VERSION_STRING
#define VERSION_STRING "0.0.0-unknown"
#endif

#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/init.h>

#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>
#include <kernel/tasks/events.h>

#include <kernel/mem/malloc.h>

#include <kernel/ipc/ipc.h>
#include <kernel/ipc/mutex.h>
#include <kernel/ipc/semaphore.h>
#include <kernel/ipc/fifo.h>
#include <kernel/ipc/lifo.h>

#include <kernel/debug/printk.h>

#endif