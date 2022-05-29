/*
 * uart.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_STUB_UART_H_
#define KERNEL_STUB_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

void __attribute__((weak)) uart_init()
{
	return;
}

void __attribute__((weak)) uart_putc(char c)
{
	return;
}

void __attribute__((weak)) uart_puts(char *msg)
{
	return;
}

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_STUB_UART_H_ */