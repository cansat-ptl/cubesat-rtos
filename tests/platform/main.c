/*
 * cubesat-rtos.c
 *
 * Created: 22.11.2020 20:06:27
 * Author : ThePetrovich
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

void test_task3(void* args) 
{
	debug_printk_p(ROMSTR("task3: Start\r\n"));
	while (1)
	{
		char receiveBuffer[32] = "";
		uint8_t receiveBufferIndex = 0;

		debug_printk_p(ROMSTR("task3: Reading FIFO\r\n"));

		while (!ipc_fifoAvailable(&fifo));
		ipc_fifoRead(&fifo, (void*)receiveBuffer);

		receiveBuffer[31] = 0;

		debug_printk_p(ROMSTR("task3: FIFO contents: %s\r\n"), receiveBuffer);
	}
}

void test_task2(void* args) 
{
	debug_printk_p(ROMSTR("Test\r\n"));
	while (1)
	{
		char receiveBuffer[32] = "";
		uint8_t receiveBufferIndex = 0;

		debug_printk_p(ROMSTR("task2: Reading FIFO\r\n"));

		while (!ipc_fifoAvailable(&fifo));
		ipc_fifoRead(&fifo, (void*)receiveBuffer);

		receiveBuffer[31] = 0;

		debug_printk_p(ROMSTR("task2: FIFO contents: %s\r\n"), receiveBuffer);
	}
}

void test_task(void* args) 
{
	char asd[] = "10 symbols";
	debug_printk_p(ROMSTR("task1: Start\r\n"));
	while (1)
	{
		debug_printk_p(ROMSTR("task1: Writing FIFO\r\n"));
		while (!ipc_fifoFreeSpace(&fifo));
		ipc_fifoWrite(&fifo, (void*)asd);
	}
}

void test_task123(void* args) 
{
	char asd[] = "10 symbols";
	debug_printk_p(ROMSTR("task123: Start\r\n"));
	while (1)
	{
		debug_printk_p(ROMSTR("task123: Writing FIFO\r\n"));
		while (!ipc_fifoFreeSpace(&fifo));
		ipc_fifoWrite(&fifo, (void*)asd);
		break;
	}
}


int main(void)
{
	kernel_init();
	ipc_fifoInit(&fifo, fifoBuffer, 31, 11);
	test = tasks_createTaskDynamic(150, test_task, NULL, 1, KTASK_NORMAL, "test1");
	test2 = tasks_createTaskDynamic(150, test_task2, NULL, 1, KTASK_NORMAL, "test2");
	test3 = tasks_createTaskDynamic(150, test_task3, NULL, 1, KTASK_NORMAL, "test3");
	test4 = tasks_createTaskDynamic(150, test_task123, NULL, 1, KTASK_NORMAL, "test123");
	kernel_startScheduler();
	while (1)
	{
		asm volatile("nop"::);
		//debug_printk_p(ROMSTR("Idling in mah main\r\n");
	}
}