/*
 * stack.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_STACK_H_
#define KERNEL_STACK_H_

#include <avr/io.h>
#include <avr/iom128.h>
#include <avr/common.h>

#include <types.h>

kStackPtr_t arch_prepareStackFrame(kStackPtr_t stackPointer, kStackSize_t stackSize, kTask_t taskPointer, void* args);

#endif