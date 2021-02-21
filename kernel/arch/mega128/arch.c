/*
 * init.c
 *
 * Created: 17.12.2020 20:53:05
 *  Author: Admin
 */


#include <rtos/types.h>
#include <rtos/config.h>
#include <rtos/arch/mega128/uart.h>
#include <rtos/arch/mega128/arch.h>

volatile byte kReservedMemory[CFG_KERNEL_RESERVED_MEMORY];
kStackPtr_t kStackPointer = &kReservedMemory[CFG_KERNEL_RESERVED_MEMORY-1];

void arch_platformInit()
{
	uart_init();
	arch_setupSystickTimer();
	arch_startSystickTimer();
	arch_ENABLE_INTERRUPTS();
}

void arch_spinlockAcquire(kSpinlock_t* spinlock)
{
	while(1) {
		asm volatile("": : :"memory");
		if(*spinlock == 0) {
			arch_enterCriticalSection();
			if(*spinlock == 0) {
				*spinlock = 1;
				arch_exitCriticalSection();
				return;
			}
			arch_exitCriticalSection();
		}
	}
}

void arch_spinlockRelease(kSpinlock_t* spinlock)
{
	arch_enterCriticalSection();
	*spinlock = 0;
	arch_exitCriticalSection();
}

void arch_setupSystickTimer()
{
	kStatusRegister_t sreg = arch_STATUS_REG;
	arch_DISABLE_INTERRUPTS();
	TCCR0 |= (CFG_KERNEL_TIMER_PRESCALER << CS00); // prescaler 64 cs11 & cs10 = 1
	TCNT0 = 0;
	OCR0 = 250;//CFG_TIMER_COMPARE_VALUE; Corrected accordingly to ISR execution time
	arch_ENABLE_INTERRUPTS();
	arch_STATUS_REG = sreg;
}

void arch_startSystickTimer()
{
	kStatusRegister_t sreg = arch_STATUS_REG;
	arch_DISABLE_INTERRUPTS();
	TIMSK |= (1 << OCIE0);
	arch_ENABLE_INTERRUPTS();
	arch_STATUS_REG = sreg;
}

void arch_stopSystickTimer()
{
	kStatusRegister_t sreg = arch_STATUS_REG;
	arch_DISABLE_INTERRUPTS();
	TIMSK &= ~(1 << OCIE0);
	arch_ENABLE_INTERRUPTS();
	arch_STATUS_REG = sreg;
}