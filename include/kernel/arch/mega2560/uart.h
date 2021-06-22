/*
 * uart.h
 * 
 * Created: 15.06.2021 07:37:34
 * Author: ThePetrovich
 */


#ifndef KERNEL_UART_H_
#define KERNEL_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

void uart_init();
void uart_putc(char c);
void uart_puts(char *msg);

#ifdef __cplusplus
}
#endif

#endif /* KERNEL_UART_H_ */