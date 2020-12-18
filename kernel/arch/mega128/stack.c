/*
 * stack.c
 *
 * Created: 17.12.2020 20:52:33
 *  Author: Admin
 */

#include <types.h>
#include <arch/mega128/mega128.h>

kStackPtr_t arch_prepareStackFrame(kStackPtr_t stackPointer, kStackSize_t stackSize, kTask_t taskPointer, void* args)
{
	//TODO: 3-byte PC support
	stackPointer += stackSize-2;
	*(stackPointer--) = 0; //(uint16_t)kernel_taskReturnHook & 0xFF;	// Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits
	*(stackPointer--) = 0; //(uint16_t)kernel_taskReturnHook >> 8;
	*(stackPointer--) = (uint16_t)taskPointer & 0xFF;	// Function address - will be grabbed by RETI when the task executes for first time, lower 8 bits
	*(stackPointer--) = (uint16_t)taskPointer >> 8;		// Upper 8 bits
	*(stackPointer--) = 0;								// R0 initial value, overwritten by SREG during context switch, should be initialized separately
	*(stackPointer--) = 0x80;							// SREG initial value - interrupts enabled
	*(stackPointer--) = 0x00;							// R1
	*(stackPointer--) = 0x00;							// R2
	*(stackPointer--) = 0x00;							// R3
	*(stackPointer--) = 0x00;							// R4
	*(stackPointer--) = 0x00;							// R5
	*(stackPointer--) = 0x00;							// R6
	*(stackPointer--) = 0x00;							// R7
	*(stackPointer--) = 0x00;							// R8
	*(stackPointer--) = 0x00;							// R9
	*(stackPointer--) = 0x00;							// R10
	*(stackPointer--) = 0x00;							// R11
	*(stackPointer--) = 0x00;							// R12
	*(stackPointer--) = 0x00;							// R13
	*(stackPointer--) = 0x00;							// R14
	*(stackPointer--) = 0x00;							// R15
	*(stackPointer--) = 0x00;							// R16
	*(stackPointer--) = 0x00;							// R17
	*(stackPointer--) = 0x00;							// R18
	*(stackPointer--) = 0x00;							// R19
	*(stackPointer--) = 0x00;							// R20
	*(stackPointer--) = 0x00;							// R21
	*(stackPointer--) = 0x00;							// R22
	*(stackPointer--) = 0x00;							// R23
	*(stackPointer--) = (uint16_t)args & 0xFF;			// R24, low byte of args pointer
	*(stackPointer--) = (uint16_t)args >> 8;			// R25, high byte of args pointer
	*(stackPointer--) = 0x00;							// R26
	*(stackPointer--) = 0x00;							// R27
	*(stackPointer--) = 0x00;							// R28
	*(stackPointer--) = 0x00;							// R29
	*(stackPointer--) = 0x00;							// R30
	*(stackPointer--) = 0x00;							// R31

	return stackPointer;
}