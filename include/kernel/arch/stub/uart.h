/*
 * uart.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef ARCHSTUB_UART_H_
#define ARCHSTUB_UART_H_

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

#endif /* ARCHSTUB_UART_H_ */