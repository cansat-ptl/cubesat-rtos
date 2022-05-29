/*
 * uart.c
 *
 * Created: 17.12.2020 20:52:21
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_stub

#include <kernel/types.h>

void uart_init()
{
	return;
}

void uart_putc(char c)
{
	return;
}

void uart_puts(char *msg)
{
	return;
}

#endif
