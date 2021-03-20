/*
 * uart.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef KERNEL_UART_H_
#define KERNEL_UART_H_

void uart_init();
void uart_putc(char c);
void uart_puts(char *msg);

#endif