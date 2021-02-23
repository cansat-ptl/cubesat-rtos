/*
 * cubesat-rtos.c
 *
 * Created: 22.11.2020 20:06:27
 * Author : Admin
 */

#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <kernel/kernel.h>
#include <kernel/arch/arch.h>
#include <string.h>

kTaskHandle_t test;
kTaskHandle_t test2;
kTaskHandle_t test3;
kTaskHandle_t test4;
kTaskHandle_t test5;
kTaskHandle_t test6;
kTaskHandle_t test7;

kFIFO_t fifo;
kMutex_t mutex;

byte fifoBuffer[50];

void test_task3() 
{
	uart_puts("task3: Start\r\n");
	while (1)
	{
		char receiveBuffer[32] = "";
		uint8_t receiveBufferIndex = 0;

		uart_puts("task3: Reading FIFO\r\n");

		ipc_fifoReadBlocking(&fifo, (void*)receiveBuffer);

		uart_puts("task3: FIFO contents: ");
		uart_puts(receiveBuffer);
		uart_puts("\r\n");

		receiveBuffer[31] = 0;

		tasks_sleep(100);
	}
}

void test_task2() 
{
	uart_puts("task2: Start\r\n");
	while (1)
	{
		char receiveBuffer[32] = "";
		uint8_t receiveBufferIndex = 0;

		uart_puts("task2: Reading FIFO\r\n");

		ipc_fifoReadBlocking(&fifo, (void*)receiveBuffer);

		uart_puts("task2: FIFO contents: ");
		uart_puts(receiveBuffer);
		uart_puts("\r\n");

		receiveBuffer[31] = 0;

		tasks_sleep(200);
	}
}

void test_task() 
{
	char asd[] = "10 symbols";
	uart_puts("task1: Start\r\n");
	while (1)
	{
		uart_puts("task1: Writing FIFO\r\n");
		ipc_fifoWriteBlocking(&fifo, (void*)asd);
		tasks_sleep(100);
	}
}

int main(void)
{
    kernel_init();
	ipc_fifoInit(&fifo, fifoBuffer, 31, 10, &mutex);
	test = tasks_createTaskDynamic(100, test_task, NULL, 1, KTASK_NORMAL, "test1");
	test2 = tasks_createTaskDynamic(100, test_task2, NULL, 1, KTASK_NORMAL, "test2");
	test3 = tasks_createTaskDynamic(100, test_task3, NULL, 1, KTASK_NORMAL, "test3");
    while (1)
    {
		asm volatile("nop"::);
		//uart_puts("Idling in mah main\r\n");
    }
}

