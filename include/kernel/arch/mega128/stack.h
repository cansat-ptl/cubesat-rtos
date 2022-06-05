/*
 * stack.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef AVRM128_STACK_H_
#define AVRM128_STACK_H_

#include <avr/io.h>
#include <avr/iom128.h>
#include <avr/common.h>

#ifdef __cplusplus
extern "C" {
#endif

kStackPtr_t arch_prepareStackFrame(kStackPtr_t stackPointer, kStackSize_t stackSize, void (*entry)(void), void *args);

kStackPtr_t arch_prepareProtectionRegion(kStackPtr_t basePtr, kStackSize_t stackSize, kStackSize_t regionSize);
kReturnValue_t arch_checkProtectionRegion(kStackPtr_t basePtr, kStackSize_t stackSize, kStackSize_t regionSize);

#ifdef __cplusplus
}
#endif

#endif /* AVRM128_STACK_H_ */