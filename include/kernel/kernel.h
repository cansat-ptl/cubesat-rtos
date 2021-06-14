/*
 * rtos.h
 * 
 * Created: 08.02.2021 06:58:35
 * Author: ThePetrovich
 */


#ifndef KERNEL_RTOS_H_
#define KERNEL_RTOS_H_

#define KERNEL_VERSION "v0.0.1-git-00000000"

#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/init.h>

#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>

#include <kernel/mem/malloc.h>

#include <kernel/ipc/ipc.h>
#include <kernel/ipc/mutex.h>
#include <kernel/ipc/semaphore.h>
#include <kernel/ipc/fifo.h>
#include <kernel/ipc/lifo.h>

#include <kernel/debug/printk.h>

#endif