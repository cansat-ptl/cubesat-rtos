/*
 * arch.h
 * 
 * Created: 08.02.2021 03:33:04
 * Author: ThePetrovich
 */


#ifndef KERNEL_MEGA128_H_
#define KERNEL_MEGA128_H_

#include <avr/io.h>
#include <avr/iom128.h>

#include <rtos/types.h>

#ifndef F_CPU
#define F_CPU 16000000L						/* CPU frequency */
#endif

#define CFG_TIMER_COMPARE_VALUE 250
#define CFG_KERNEL_TIMER_PRESCALER 3

#define lo8(x) ((x)&0xff)
#define hi8(x) ((x)>>8)

#define arch_DISABLE_INTERRUPTS() asm volatile ("cli"::)
#define arch_ENABLE_INTERRUPTS() asm volatile ("sei"::)
#define arch_STATUS_REG SREG
#define arch_NOP() asm volatile ("nop"::)
#define arch_enterCriticalSection() asm volatile (	"lds __tmp_reg__, __SREG__ \n\t" \
													"cli\n\t" \
													"push	__tmp_reg__ \n\t" ::)
#define arch_exitCriticalSection() asm volatile (	"pop __tmp_reg__ \n\t"	\
													"sei \n\t" \
													"sts __SREG__, __tmp_reg__ \n\t" ::)

#define arch_RET() asm volatile ("ret \n\t" ::)
#define arch_RETI() asm volatile ("reti \n\t" ::)

void __attribute__ (( naked, noinline )) arch_yield(void);

void arch_setupSystickTimer();
void arch_startSystickTimer();
void arch_stopSystickTimer();

void arch_platformInit();

void arch_spinlockAcquire(kSpinlock_t* spinlock);
void arch_spinlockRelease(kSpinlock_t* spinlock);

#endif