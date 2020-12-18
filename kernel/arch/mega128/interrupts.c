/*
 * interrupts.c
 *
 * Created: 17.12.2020 20:52:49
 *  Author: Admin
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <arch/mega128/stack.h>
#include <arch/mega128/mega128.h>

void tasks_tick();
void tasks_switchTask();

void __attribute__ (( naked )) _arch_save_context();
void __attribute__ (( naked )) _arch_restore_context();

void __attribute__ (( naked, noinline )) arch_yield(void)
{
	_arch_save_context();

	tasks_switchTask();

	_arch_restore_context();
	arch_RET();
}

void __attribute__ (( naked, noinline )) arch_tick()
{
	_arch_save_context();

	tasks_tick();

	_arch_restore_context();
	arch_RET();
}

ISR(TIMER0_COMP_vect, ISR_NAKED)
{
	arch_tick();
	arch_RETI();
}