/*
 * uart.h
 * 
 * Created: 02.01.2021 11:58:18
 * Author: ThePetrovich
 */


#ifndef AVRM128_UART_H_
#define AVRM128_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

void uart_init();
void uart_putc(char c);
void uart_puts(char *msg);

#ifdef __cplusplus
}
#endif

#endif /* AVRM128_UART_H_ */