/*
 * stack.c
 *
 * Created: 17.12.2020 20:52:33
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_stub

#include <kernel/types.h>
#include <kernel/config.h>

void arch_taskReturnHook();

kStackPtr_t arch_prepareStackFrame(kStackPtr_t stackPointer, kStackSize_t stackSize, void (*entry)(void), void *args)
{
	return stackPointer;
}

kStackPtr_t arch_prepareProtectionRegion(kStackPtr_t basePtr, kStackSize_t stackSize, kStackSize_t regionSize)
{
	return basePtr;
}

kReturnValue_t arch_checkProtectionRegion(kStackPtr_t basePtr, kStackSize_t stackSize, kStackSize_t regionSize)
{
	return KRESULT_SUCCESS;
}

void arch_taskReturnHook()
{
    	return;
}

#endif