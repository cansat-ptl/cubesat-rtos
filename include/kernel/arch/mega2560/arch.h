/*
 * arch.h
 * 
 * Created: 15.06.2021 07:37:34
 * Author: ThePetrovich
 */


#ifndef KERNEL_MEGA2560_H_
#define KERNEL_MEGA2560_H_

#include <avr/io.h>
#include <avr/iom2560.h>

#include <kernel/types.h>

/* CPU frequency */
#ifndef F_CPU
#define F_CPU 16000000L	
#endif

#define CFG_TIMER_COMPARE_VALUE 250
#define CFG_KERNEL_TIMER_PRESCALER 3

#define lo8(x) ((x)&0xff)
#define hi8(x) ((x)>>8)

#define arch_DISABLE_INTERRUPTS() asm volatile ("cli"::)
#define arch_ENABLE_INTERRUPTS() asm volatile ("sei"::)
#define arch_STATUS_REG SREG
#define arch_NOP() asm volatile ("nop"::)
#define arch_enterCriticalSection()	asm volatile ("lds __tmp_reg__, __SREG__ \n\t"\
					              "cli                       \n\t"\
					              "push __tmp_reg__"            ::)
#define arch_exitCriticalSection() 	asm volatile ("pop __tmp_reg__           \n\t"\
					              "sts __SREG__, __tmp_reg__"   ::)

#define arch_RET() asm volatile ("ret \n\t" ::)
#define arch_RETI() asm volatile ("reti \n\t" ::)

#ifdef __cplusplus
extern "C" {
#endif

void __attribute__ (( naked, noinline )) arch_yield(void);

void arch_setupSystickTimer();
void arch_startSystickTimer();
void arch_stopSystickTimer();

void arch_platformInit();
void arch_startScheduler();

void arch_halt();
void arch_reboot();

kStatusRegister_t arch_enterCriticalSectionSafe();
void arch_exitCriticalSectionSafe(kStatusRegister_t sreg);

void arch_spinlockAcquire(kSpinlock_t *spinlock);
void arch_spinlockRelease(kSpinlock_t *spinlock);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_MEGA2560_H_ */