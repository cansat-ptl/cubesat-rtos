/*
 * uart.c
 * 
 * Created: 15.06.2021 07:37:23
 * Author: ThePetrovich
 */


#ifdef KERNEL_ARCH_avr
#ifdef KERNEL_MCU_atmega2560

#include <avr/io.h>
#include <avr/iom2560.h>
#include <kernel/types.h>
#include <kernel/arch/mega2560/uart.h>

void uart_init()
{
	UBRR0H = (24 >> 8);
	UBRR0L = (24 & 0xFF);
	UCSR0A |= (1 << U2X0);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0);
	UCSR0C = (0<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02)|(0<<UPM00)|(0<<UPM01)|(0<<UMSEL00);
}

void uart_putc(char c)
{
	UDR0 = c;
	while (!(UCSR0A & (1<<UDRE0)));
}

void uart_puts(char *msg)
{
	kIterator_t i = 0;
	while (msg[i] != '\0') {
		uart_putc(msg[i]);
		i++;
	}
}

#endif
#endif