/*
 * timer.c
 *
 * Created: 17.12.2020 21:53:44
 *  Author: Admin
 */
#include <avr/io.h>
#include <avr/iom128.h>

#include <types.h>
#include <arch/mega128/mega128.h>

void arch_setupSystickTimer()
{
	kStatusRegister_t sreg = arch_STATUS_REG;
	arch_DISABLE_INTERRUPTS();
	TCCR0 |= (CFG_KERNEL_TIMER_PRESCALER << CS00); // prescaler 64 cs11 & cs10 = 1
	TCNT0 = 0;
	OCR0 = CFG_TIMER_COMPARE_VALUE; //Corrected accordingly to ISR execution time
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