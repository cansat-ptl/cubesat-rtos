/*
 * arch.c
 * 
 * Created: 15.06.2021 07:37:23
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_avr
#ifdef KERNEL_MCU_atmega2560

#include <kernel/types.h>
#include <kernel/config.h>
#include <kernel/arch/mega2560/uart.h>
#include <kernel/arch/mega2560/arch.h>
#include <avr/wdt.h>

static volatile uint8_t mcusr_mirror __attribute__ ((section (".noinit")));
volatile byte kReservedMemory[CFG_KERNEL_RESERVED_MEMORY];
kStackPtr_t kStackPointer = &kReservedMemory[CFG_KERNEL_RESERVED_MEMORY-2];

void arch_platformInit()
{
	uart_init();
	arch_setupSystickTimer();
}

void arch_startScheduler()
{
	arch_startSystickTimer();
	arch_ENABLE_INTERRUPTS();
}

void arch_spinlockAcquire(kSpinlock_t *spinlock)
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

void arch_spinlockRelease(kSpinlock_t *spinlock)
{
	arch_enterCriticalSection();
	*spinlock = 0;
	arch_exitCriticalSection();
}

void arch_setupSystickTimer()
{
	kStatusRegister_t sreg = arch_STATUS_REG;
	arch_DISABLE_INTERRUPTS();
	TCCR0B |= (CFG_KERNEL_TIMER_PRESCALER << CS10); /* prescaler 64 cs11 & cs10 = 1 */
	TCNT0 = 0;
	OCR0A = 250; /* CFG_TIMER_COMPARE_VALUE; Corrected according to ISR execution time */
	arch_ENABLE_INTERRUPTS();
	arch_STATUS_REG = sreg;
}

void arch_startSystickTimer()
{
	kStatusRegister_t sreg = arch_STATUS_REG;
	arch_DISABLE_INTERRUPTS();
	TIMSK0 |= (1 << OCIE0A);
	arch_ENABLE_INTERRUPTS();
	arch_STATUS_REG = sreg;
}

void arch_stopSystickTimer()
{
	kStatusRegister_t sreg = arch_STATUS_REG;
	arch_DISABLE_INTERRUPTS();
	TIMSK0 &= ~(1 << OCIE0A);
	arch_ENABLE_INTERRUPTS();
	arch_STATUS_REG = sreg;
}

kStatusRegister_t arch_enterCriticalSectionSafe()
{
	kStatusRegister_t sreg = arch_STATUS_REG;
	arch_DISABLE_INTERRUPTS();
	return sreg;
}

void arch_exitCriticalSectionSafe(kStatusRegister_t sreg)
{
	arch_ENABLE_INTERRUPTS();
	arch_STATUS_REG = sreg;
} 

void arch_halt()
{	
	arch_DISABLE_INTERRUPTS();
	wdt_disable();
	while(1) {
		; /* wait for reset */
	}
}

void arch_reboot()
{
	arch_DISABLE_INTERRUPTS();
	wdt_enable(WDTO_15MS);
	while(1) {
		; /* wait for reset */
	}
}

static void arch_disableWatchdogOnStart(void) __attribute__((naked, section(".init3")));

static void arch_disableWatchdogOnStart(void)
{
	mcusr_mirror = MCUSR;
	MCUSR = 0;
	wdt_disable();
}

#endif
#endif