/*
 * context.S
 *
 * Created: 19.12.2020 0:56:24
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_avr
#ifdef KERNEL_MCU_atmega128

#include <avr/io.h>
#include <avr/iom128.h>

.extern kSchedCPUState
.extern kReservedMemory
.extern kStackPointer
.extern tasks_tick
.extern tasks_switchTask

.text
.global _arch_restore_context
.global _arch_save_context
.global arch_tick
.global arch_yield

.macro _arch_restore_context_m
;-------------------Context-Restore-------------------
	lds r0, SPL					; load current stack pointer
	sts kStackPointer, r0		; and save it into kStackPointer
	lds r0, SPH
	sts kStackPointer+1, r0
	lds r26, kSchedCPUState		; load kCurrentTask handle
	lds r27, kSchedCPUState+1
	ld r0, X+					; load new stack pointer from kCurrentTask
	sts SPL, r0					; and replace the current one with it
	ld r0, X+
	sts SPH, r0
	pop r31						; load previously saved registers from stack
	pop r30
	pop r29
	pop r28
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0						; load previously saved SREG to r0
	out _SFR_IO_ADDR(SREG), r0
	pop r0						; load the actual r0 register
;-----------------------------------------------------
.endm

.macro _arch_save_context_m
;--------------------Context-Save---------------------
	push r0						; do everything from _arch_restore_context
	in r0, _SFR_IO_ADDR(SREG)				; but in reverse
	cli
	push r0
	push r1
	clr r1						; r1 needs to always be 0
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r28
	push r29
	push r30
	push r31
	lds r26, kSchedCPUState
	lds r27, kSchedCPUState+1
	lds r0, SPL
	st X+, r0
	lds r0, SPH
	st X+, r0
	lds r0, kStackPointer
	sts SPL, r0
	lds r0, kStackPointer+1
	sts SPH, r0
;-----------------------------------------------------
.endm

arch_tick:
	_arch_save_context_m
	call tasks_tick
	_arch_restore_context_m
	ret

arch_yield:
	_arch_save_context_m
	call tasks_switchTask
	_arch_restore_context_m
	ret

#endif
#endif