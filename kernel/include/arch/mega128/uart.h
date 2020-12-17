/*
 * uart.h
 *
 * Created: 17.12.2020 21:16:24
 *  Author: Admin
 */


#ifndef UART_H_
#define UART_H_

void uart_init();
void uart_putc(char c);
void uart_puts(char* msg);

#endif /* UART_H_ */