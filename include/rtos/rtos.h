/*
 * rtos.h
 * 
 * Created: 08.02.2021 06:58:35
 * Author: ThePetrovich
 */


#ifndef KERNEL_RTOS_H_
#define KERNEL_RTOS_H_

#include <rtos/types.h>
#include <rtos/config.h>
#include <rtos/init.h>

#include <rtos/tasks/tasks.h>
#include <rtos/tasks/sched.h>
#include <rtos/tasks/heap.h>

#include <rtos/ipc/ipc.h>
#include <rtos/ipc/mutex.h>
#include <rtos/ipc/semaphore.h>
#include <rtos/ipc/fifo.h>
#include <rtos/ipc/lifo.h>

#endif