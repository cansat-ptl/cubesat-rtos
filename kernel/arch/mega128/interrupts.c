/*
 * interrupts.c
 *
 * Created: 17.12.2020 20:52:49
 *  Author: Admin
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <kernel/arch/mega128/arch.h>

void __attribute__ (( naked, noinline )) arch_tick();

ISR(TIMER0_COMP_vect, ISR_NAKED) 
{
	arch_tick();
	arch_RETI();
}