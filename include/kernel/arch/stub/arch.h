/*
 * arch.h
 * 
 * Created: 08.02.2021 03:33:04
 * Author: ThePetrovich
 */


#ifndef KERNEL_ARCHSTUB_H_
#define KERNEL_ARCHSTUB_H_

#include <kernel/types.h>

/* CPU frequency */
#ifndef F_CPU
#define F_CPU 16000000L
#endif

#define CFG_TIMER_COMPARE_VALUE 250
#define CFG_KERNEL_TIMER_PRESCALER 3

#define lo8(x) ((x) & 0xff)
#define hi8(x) ((x) >> 8)

#define arch_DISABLE_INTERRUPTS() 	asm volatile ("nop"::)
#define arch_ENABLE_INTERRUPTS() 	asm volatile ("nop"::)
#define arch_STATUS_REG 
#define arch_NOP() 			asm volatile ("nop"::)
#define arch_enterAtomicSection()	asm volatile ("nop"::)
#define arch_exitAtomicSection() 	asm volatile ("nop"::)
#define arch_RET() 			asm volatile ("nop"::)
#define arch_RETI() 			asm volatile ("nop"::)

#ifdef __cplusplus
extern "C" {
#endif

void __attribute__ (( naked, noinline )) arch_yield(void);

void __attribute__((weak)) arch_platformInit()
{
	return;
}

void __attribute__((weak)) arch_startScheduler()
{
	return;
}

void __attribute__((weak)) arch_enterCriticalSection()
{
	return;
}

void __attribute__((weak)) arch_exitCriticalSection()
{
	return;
}

void __attribute__((weak)) arch_spinlockAcquire(kSpinlock_t *spinlock)
{
	return;
}

void __attribute__((weak)) arch_spinlockRelease(kSpinlock_t *spinlock)
{
	return;
}

void __attribute__((weak)) arch_setupSystickTimer()
{
	return;
}

void __attribute__((weak)) arch_startSystickTimer()
{
	return;
}

void __attribute__((weak)) arch_stopSystickTimer()
{
	return;
}

kStatusRegister_t __attribute__((weak)) arch_enterCriticalSectionSafe()
{
	return 0;
}

void __attribute__((weak)) arch_exitCriticalSectionSafe(kStatusRegister_t sreg)
{
	return;
} 

void __attribute__((weak)) arch_halt()
{	
	return;
}

void __attribute__((weak)) arch_reboot()
{
	return;
}

void __attribute__((weak)) arch_yield()
{
	return;
}

void __attribute__((weak)) arch_tick()
{
	return;
}

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_ARCHSTUB_H_ */