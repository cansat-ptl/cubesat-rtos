/*
 * stack.h
 * 
 * Created: 15.06.2021 07:37:34
 * Author: ThePetrovich
 */


#ifndef KERNEL_STACK_H_
#define KERNEL_STACK_H_

#include <avr/io.h>
#include <avr/iom2560.h>
#include <avr/common.h>

#include <kernel/types.h>

#ifdef __cplusplus
extern "C" {
#endif

kStackPtr_t arch_prepareStackFrame(kStackPtr_t stackPointer, kStackSize_t stackSize, void (*entry)(void), void *args);

kStackPtr_t arch_prepareProtectionRegion(kStackPtr_t basePtr, kStackSize_t stackSize, kStackSize_t regionSize);
kReturnValue_t arch_checkProtectionRegion(kStackPtr_t basePtr, kStackSize_t stackSize, kStackSize_t regionSize);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_STACK_H_ */