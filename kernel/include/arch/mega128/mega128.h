/*
 * mega128.h
 *
 * Created: 17.12.2020 21:23:51
 *  Author: Admin
 */


#ifndef MEGA128_H_
#define MEGA128_H_

#include <avr/io.h>
#include <avr/iom128.h>

#include <types.h>

#ifndef F_CPU
#define F_CPU 16000000L						//CPU frequency
#endif

#define CFG_KERNEL_STACK_FRAME_REGISTER_OFFSET -6  //Memory offset, from which General Purpose Registers are stored during stack initialization
#define CFG_KERNEL_STACK_FRAME_END_OFFSET -31 //Last GPR position on stack
#define CFG_TIMER_COMPARE_VALUE 250
#define CFG_KERNEL_TIMER_PRESCALER 3

#define lo8(x) ((x)&0xff)
#define hi8(x) ((x)>>8)

#define arch_DISABLE_INTERRUPTS() asm volatile ("cli"::)
#define arch_ENABLE_INTERRUPTS() asm volatile ("sei"::)
#define arch_STATUS_REG SREG
#define arch_NOP() asm volatile ("nop"::)

#define arch_RET() asm volatile ("ret \n\t" ::)
#define arch_RETI() asm volatile ("reti \n\t" ::)

void arch_setupSystickTimer();
void arch_startSystickTimer();
void arch_stopSystickTimer();

void arch_platformInit();

kStatusRegister_t arch_startAtomicOperation();
void arch_endAtomicOperation(kStatusRegister_t sreg);

void __attribute__ (( naked, noinline )) arch_yield(void);

#endif /* MEGA128_H_ */