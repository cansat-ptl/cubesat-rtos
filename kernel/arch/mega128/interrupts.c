/*
 * interrupts.c
 *
 * Created: 17.12.2020 20:52:49
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_avr
#ifdef KERNEL_MCU_atmega128

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <kernel/arch/mega128/arch.h>

/* Linker workaround */
volatile uint8_t SYSTICK_ISR;

void __attribute__ (( naked, noinline )) arch_tick();

ISR(TIMER0_COMP_vect, ISR_NAKED) 
{
	arch_tick();
	arch_RETI();
}

#endif
#endif