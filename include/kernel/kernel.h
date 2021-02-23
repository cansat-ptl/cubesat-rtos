/*
 * rtos.h
 * 
 * Created: 08.02.2021 06:58:35
 * Author: ThePetrovich
 */


#ifndef KERNEL_RTOS_H_
#define KERNEL_RTOS_H_

#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/init.h>

#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>
#include <kernel/tasks/heap.h>

#include <kernel/ipc/ipc.h>
#include <kernel/ipc/mutex.h>
#include <kernel/ipc/semaphore.h>
#include <kernel/ipc/fifo.h>
#include <kernel/ipc/lifo.h>

#endif