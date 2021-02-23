/*
 * uart.c
 *
 * Created: 17.12.2020 20:52:21
 *  Author: Admin
 */


#include <avr/io.h>
#include <avr/iom128.h>

#include <kernel/types.h>
#include <kernel/arch/mega128/uart.h>

void uart_init()
{
	UBRR0H = (24 >> 8);
	UBRR0L = (24 & 0xFF);
	UCSR0A |= (1 << U2X);
	UCSR0B = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE)|(0<<TXCIE)|(0<<UDRIE);
	UCSR0C = (0<<USBS)|(1<<UCSZ00)|(1<<UCSZ01)|(0<<UCSZ02)|(0<<UPM00)|(0<<UPM01)|(0<<UMSEL0);
}

void uart_putc(char c)
{
	UDR0 = c;
	while (!(UCSR0A & (1<<UDRE)));
}

void uart_puts(char * msg)
{
	kIterator_t i = 0;
	while (msg[i] != '\0') {
		uart_putc(msg[i]);
		i++;
	}
}
