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

#include <kernel/types.h>

/* CPU frequency */
#ifndef F_CPU
#define F_CPU 16000000L
#endif

#define CFG_TIMER_COMPARE_VALUE 250
#define CFG_KERNEL_TIMER_PRESCALER 3

#define lo8(x) ((x) & 0xff)
#define hi8(x) ((x) >> 8)

#define arch_DISABLE_INTERRUPTS() asm volatile ("cli"::)
#define arch_ENABLE_INTERRUPTS() asm volatile ("sei"::)
#define arch_STATUS_REG SREG
#define arch_NOP() asm volatile ("nop"::)
#define arch_enterAtomicSection()	asm volatile ("in __tmp_reg__, __SREG__ \n\t"\
					              "cli                       \n\t"\
					              "push __tmp_reg__"            ::)
#define arch_exitAtomicSection() 	asm volatile ("pop __tmp_reg__           \n\t"\
					              "sei                       \n\t"\
					              "out __SREG__, __tmp_reg__"   ::)

#define arch_RET() asm volatile ("ret" ::)
#define arch_RETI() asm volatile ("reti" ::)

#ifdef __cplusplus
extern "C" {
#endif

void __attribute__ (( naked, noinline )) arch_yield(void);

void arch_enterCriticalSection();
void arch_exitCriticalSection();

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

#endif /* KERNEL_MEGA128_H_ */