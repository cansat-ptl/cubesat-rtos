/*
 * cubesat-rtos.c
 *
 * Created: 22.11.2020 20:06:27
 * Author : Admin
 */

#define F_CPU 16000000L

#include <avr/io.h>
#include <kernel.h>
#include <arch/mega128/uart.h>
#include <util/delay.h>
#include <types.h>
#include <tasks/tasks.h>

kTaskHandle_t test;
kTaskHandle_t test2;

void test_task() {
	while (1)
	{
		asm volatile("nop"::);
		uart_puts("Idling in test task yoooo\r\n");
	}
}

void test_task2() {
	while (1)
	{
		asm volatile("nop"::);
		uart_puts("Idling in test task 2 yoooo\r\n");
	}
}

int main(void)
{
    kernel_init();
	tasks_createTaskDynamic(&test, test_task, NULL, 100, 3, KTASK_NORMAL, "test1");
	tasks_createTaskDynamic(&test2, test_task2, NULL, 100, 3, KTASK_NORMAL, "test2");
    while (1)
    {
		uart_puts("Idling in mah main\r\n");
		_delay_ms(100);
    }
}

