/*
 * stack.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_STUB_STACK_H_
#define KERNEL_STUB_STACK_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

kStackPtr_t __attribute__((weak)) arch_prepareStackFrame(kStackPtr_t stackPointer, kStackSize_t stackSize, void (*entry)(void), void *args)
{
	return stackPointer;
}

kStackPtr_t __attribute__((weak)) arch_prepareProtectionRegion(kStackPtr_t basePtr, kStackSize_t stackSize, kStackSize_t regionSize)
{
	return basePtr;
}

kReturnValue_t __attribute__((weak)) arch_checkProtectionRegion(kStackPtr_t basePtr, kStackSize_t stackSize, kStackSize_t regionSize)
{
	return 0;
}

void __attribute__((weak)) arch_taskReturnHook()
{
    	return;
}

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_STUB_STACK_H_ */