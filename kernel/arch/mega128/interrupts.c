/*
 * interrupts.c
 *
 * Created: 17.12.2020 20:52:49
 *  Author: Admin
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>

ISR(TIMER0_COMP_vect, ISR_NAKED)
{
	//arch_tick();
	//platform_RETI();
}