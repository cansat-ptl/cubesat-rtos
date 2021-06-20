/*
 * interrupts.c
 * 
 * Created: 15.06.2021 07:37:23
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_avr
#ifdef KERNEL_MCU_atmega2560

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom2560.h>
#include <kernel/arch/mega2560/arch.h>

void __attribute__ (( naked, noinline )) arch_tick();

ISR(TIMER0_COMPA_vect, ISR_NAKED) 
{
	arch_tick();
	arch_RETI();
}

#endif
#endif