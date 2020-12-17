/*
 * init.c
 *
 * Created: 17.12.2020 20:49:07
 *  Author: ThePetrovich
 */

#include <types.h>
#include <arch/arch.h>

void kernel_init()
{
	arch_platformInit();
	uart_puts("Hello from OS init!\r\n");
}