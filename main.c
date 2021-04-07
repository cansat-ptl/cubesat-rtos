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

kTask_t *test;
kTask_t *test2;
kTask_t *test3;
kTask_t *test4;
kTask_t *test5;
kTask_t *test6;
kTask_t *test7;

kFIFO_t fifo;
kMutex_t mutex;

byte fifoBuffer[50];

void test_task3() 
{
	debug_printk("task3: Start\r\n");
	while (1)
	{
		char receiveBuffer[32] = "";
		uint8_t receiveBufferIndex = 0;

		debug_printk("task3: Reading FIFO\r\n");

		ipc_fifoReadBlocking(&fifo, (void*)receiveBuffer);

		receiveBuffer[31] = 0;

		debug_printk("task3: FIFO contents: %s\r\n", receiveBuffer);
	}
}

void test_task2() 
{
	debug_printk("Test\r\n");
	while (1)
	{
		char receiveBuffer[32] = "";
		uint8_t receiveBufferIndex = 0;

		debug_printk("task2: Reading FIFO\r\n");

		ipc_fifoReadBlocking(&fifo, (void*)receiveBuffer);

		debug_printk("task2: FIFO contents: %s\r\n", receiveBuffer);

		break;
	}
}

void test_task() 
{
	char asd[] = "10 symbols";
	debug_printk("task1: Start\r\n");
	while (1)
	{
		debug_printk("task1: Writing FIFO\r\n");
		ipc_fifoWriteBlocking(&fifo, (void*)asd);
	}
}

void test_task123() 
{
	char asd[] = "10 symbols";
	debug_printk("task123: Start\r\n");
	while (1)
	{
		debug_printk("task123: Writing FIFO\r\n");
		ipc_fifoWriteBlocking(&fifo, (void*)asd);
		_delay_ms(10);
		break;
	}
}


int main(void)
{
	kernel_init();
	ipc_fifoInit(&fifo, fifoBuffer, 31, 11, &mutex);
	test = tasks_createTaskDynamic(150, test_task, NULL, 1, KTASK_NORMAL, "test1");
	test2 = tasks_createTaskDynamic(150, test_task2, NULL, 1, KTASK_NORMAL, "test2");
	test3 = tasks_createTaskDynamic(150, test_task3, NULL, 1, KTASK_NORMAL, "test3");
	test4 = tasks_createTaskDynamic(150, test_task123, NULL, 1, KTASK_NORMAL, "test123");
	while (1)
	{
		asm volatile("nop"::);
		//debug_printk("Idling in mah main\r\n");
	}
}

