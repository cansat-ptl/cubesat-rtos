/*
 * stack.c
 *
 * Created: 17.12.2020 20:52:33
 *  Author: Admin
 */


#include <rtos/types.h>
#include <rtos/arch/mega128/stack.h>

kStackPtr_t arch_prepareStackFrame(kStackPtr_t stackPointer, kStackSize_t stackSize, kTask_t taskPointer, void* args)
{
	//TODO: 3-byte PC support
	stackPointer += stackSize-1;
	*(stackPointer--) = 0; //(uint16_t)kernel_taskReturnHook & 0xFF;	// Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits
	*(stackPointer--) = 0; //(uint16_t)kernel_taskReturnHook >> 8;
	*(stackPointer--) = (uint16_t)taskPointer & 0xFF;	// Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits
	*(stackPointer--) = (uint16_t)taskPointer >> 8;		// Upper 8 bits
	*(stackPointer--) = 0;								// R0 initial value, overwritten by SREG during context switch, should be initialized separately
	*(stackPointer--) = 0x80;							// SREG initial value - interrupts enabled
	*(stackPointer--) = 0x00;							// R1, needs to be 0 in gcc
	*(stackPointer--) = 0x02;							// R2
	*(stackPointer--) = 0x03;							// R3
	*(stackPointer--) = 0x04;							// R4
	*(stackPointer--) = 0x05;							// R5
	*(stackPointer--) = 0x06;							// R6
	*(stackPointer--) = 0x07;							// R7
	*(stackPointer--) = 0x08;							// R8
	*(stackPointer--) = 0x09;							// R9
	*(stackPointer--) = 0x0A;							// R10
	*(stackPointer--) = 0x0B;							// R11
	*(stackPointer--) = 0x0C;							// R12
	*(stackPointer--) = 0x0D;							// R13
	*(stackPointer--) = 0x0E;							// R14
	*(stackPointer--) = 0x0F;							// R15
	*(stackPointer--) = 0x10;							// R16
	*(stackPointer--) = 0x11;							// R17
	*(stackPointer--) = 0x12;							// R18
	*(stackPointer--) = 0x13;							// R19
	*(stackPointer--) = 0x14;							// R20
	*(stackPointer--) = 0x15;							// R21
	*(stackPointer--) = 0x16;							// R22
	*(stackPointer--) = 0x17;							// R23
	*(stackPointer--) = (uint16_t)args & 0xFF;			// R24, low byte of args pointer
	*(stackPointer--) = (uint16_t)args >> 8;			// R25, high byte of args pointer
	*(stackPointer--) = 0x1A;							// R26
	*(stackPointer--) = 0x1B;							// R27
	*(stackPointer--) = 0x1C;							// R28
	*(stackPointer--) = 0x1D;							// R29
	*(stackPointer--) = 0x1E;							// R30
	*(stackPointer--) = 0x1F;							// R31

	return stackPointer;
}

void arch_prepareProtectionRegion(void* pointer, size_t size)
{
	if (pointer != NULL) {
		for (size_t i = 0; i < size; i++) {
			*(byte*)((byte*)pointer + i) = 0xFE;
		}
	}
}

kReturnValue_t arch_checkProtectionRegion(void* pointer, size_t size)
{
	kReturnValue_t kresult = KRESULT_SUCCESS;

	if (pointer != NULL) {
		for (size_t i = 0; i < size; i++) {
			if (*(byte*)((byte*)pointer + i) != 0xFE) {
				kresult = KRESULT_ERR_MEMORY_VIOLATION;
				break;
			}
		}
	}
	
	return kresult;
}