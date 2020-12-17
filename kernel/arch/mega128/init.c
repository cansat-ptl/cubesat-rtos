/*
 * init.c
 *
 * Created: 17.12.2020 20:53:05
 *  Author: Admin
 */

#include <types.h>
#include <arch/mega128/uart.h>

void arch_platformInit()
{
	uart_init();
}