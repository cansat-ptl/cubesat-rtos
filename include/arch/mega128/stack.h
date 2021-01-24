/*
 * context.h
 *
 * Created: 17.12.2020 22:18:37
 *  Author: Admin
 */


#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <avr/io.h>
#include <avr/iom128.h>
#include <avr/common.h>

#include <types.h>

kStackPtr_t arch_prepareStackFrame(kStackPtr_t stackPointer, kStackSize_t stackSize, kTask_t taskPointer, void* args);

#endif /* CONTEXT_H_ */