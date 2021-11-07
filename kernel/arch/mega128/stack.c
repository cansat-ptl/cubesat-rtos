/*
 * stack.c
 *
 * Created: 17.12.2020 20:52:33
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_avr
#ifdef KERNEL_MCU_atmega128

#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/arch/mega128/stack.h>
#include <kernel/tasks/tasks.h>
#include <kernel/tasks/sched.h>

void arch_taskReturnHook();

kStackPtr_t arch_prepareStackFrame(kStackPtr_t stackPointer, kStackSize_t stackSize, void (*entry)(void), void *args)
{
	stackPointer += stackSize - 1;
	*(stackPointer--) = (uint16_t)arch_taskReturnHook & 0xFF; 	/* Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits */
	*(stackPointer--) = (uint16_t)arch_taskReturnHook >> 8; 	/* higher 8 bits */
	*(stackPointer--) = (uint16_t)entry & 0xFF;			/* Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits */
	*(stackPointer--) = (uint16_t)entry >> 8;			/* Upper 8 bits */
	*(stackPointer--) = 0;						/* R0 initial value, overwritten by SREG during context switch, should be initialized separately */
	*(stackPointer--) = 0x80;					/* SREG initial value - interrupts enabled */
	*(stackPointer--) = 0x00;					/* R1, needs to be 0 in gcc */
	*(stackPointer--) = 0x02;					/* R2 */
	*(stackPointer--) = 0x03;					/* R3 */
	*(stackPointer--) = 0x04;					/* R4 */
	*(stackPointer--) = 0x05;					/* R5 */
	*(stackPointer--) = 0x06;					/* R6 */
	*(stackPointer--) = 0x07;					/* R7 */
	*(stackPointer--) = 0x08;					/* R8 */
	*(stackPointer--) = 0x09;					/* R9 */
	*(stackPointer--) = 0x0A;					/* R10 */
	*(stackPointer--) = 0x0B;					/* R11 */
	*(stackPointer--) = 0x0C;					/* R12 */
	*(stackPointer--) = 0x0D;					/* R13 */
	*(stackPointer--) = 0x0E;					/* R14 */
	*(stackPointer--) = 0x0F;					/* R15 */
	*(stackPointer--) = 0x10;					/* R16 */
	*(stackPointer--) = 0x11;					/* R17 */
	*(stackPointer--) = 0x12;					/* R18 */
	*(stackPointer--) = 0x13;					/* R19 */
	*(stackPointer--) = 0x14;					/* R20 */
	*(stackPointer--) = 0x15;					/* R21 */
	*(stackPointer--) = 0x16;					/* R22 */
	*(stackPointer--) = 0x17;					/* R23 */
	*(stackPointer--) = (uint16_t)args & 0xFF;			/* R24, low byte of args pointer */ //-V542
	*(stackPointer--) = (uint16_t)args >> 8;			/* R25, high byte of args pointer */ //-V542
	*(stackPointer--) = 0x1A;					/* R26 */
	*(stackPointer--) = 0x1B;					/* R27 */
	*(stackPointer--) = 0x1C;					/* R28 */
	*(stackPointer--) = 0x1D;					/* R29 */
	*(stackPointer--) = 0x1E;					/* R30 */
	*(stackPointer--) = 0x1F;					/* R31 */

	return stackPointer;
}

kStackPtr_t arch_prepareProtectionRegion(kStackPtr_t basePtr, kStackSize_t stackSize, kStackSize_t regionSize)
{
	#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
		if (basePtr != NULL) {
			for (size_t i = 0; i < regionSize; i++) {
				*(byte *)((byte *)basePtr + i) = 0xFE;
			}
		}
	
		return basePtr + regionSize;
	#else
		return basePtr;
	#endif
}

kReturnValue_t arch_checkProtectionRegion(kStackPtr_t basePtr, kStackSize_t stackSize, kStackSize_t regionSize)
{
	#if CFG_MEMORY_PROTECTION_MODE == 2 || CFG_MEMORY_PROTECTION_MODE == 3
		kReturnValue_t kresult = KRESULT_SUCCESS;

		if (basePtr != NULL) {
			basePtr -= regionSize;

			for (size_t i = 0; i < regionSize; i++) {
				if (*(byte*)((byte*)basePtr + i) != 0xFE) {
					kresult = KRESULT_ERR_MEMORY_VIOLATION;
					break;
				}
			}
		}
		
		return kresult;
	#else
		return KRESULT_SUCCESS;
	#endif
}

void arch_taskReturnHook()
{
    kTask_t *currentTask = tasks_getCurrentTask();

	tasks_deleteTask(currentTask);

	while (1) {
		; /* Do nothing */
	}
}

#endif
#endif