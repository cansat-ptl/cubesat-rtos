/*
 * cubesat-rtos.c
 *
 * Created: 22.11.2020 20:06:27
 * Author : Admin
 */

#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <rtos/rtos.h>
#include <rtos/arch/arch.h>

kTaskHandle_t test;
kTaskHandle_t test2;
kTaskHandle_t test3;
kTaskHandle_t test4;
kTaskHandle_t test5;
kTaskHandle_t test6;
kTaskHandle_t test7;

kMutex_t mutex;

void test_task7() {
	_delay_ms(5000);
	uart_puts("Commiting a war crime from task 7\r\n");
	tasks_deleteTaskDynamic(test);
	while (1)
	{
		uart_puts("Chillin' in task 7\r\n");
		tasks_sleep(500);
	}
}

void test_task6() {
	while (1)
	{
		uart_puts("Chillin' in task 6\r\n");
		tasks_sleep(500);
	}
}

void test_task5() {
	while (1)
	{
		uart_puts("Chillin' in task 5\r\n");
		tasks_sleep(500);
	}
}

void test_task4() {
	while (1)
	{
		uart_puts("Chillin' in task 4\r\n");
		tasks_sleep(500);
	}
}

void test_task3() {
	uart_puts("Creating task 5");
	test5 = tasks_createTaskDynamic(100, test_task5, NULL, 3, KTASK_NORMAL, "test5");
	while (1)
	{
		uart_puts("Chillin' in task 3\r\n");
		tasks_sleep(500);
	}
}

void test_task2() {
	uart_puts("Creating task 6");
	test4 = tasks_createTaskDynamic(100, test_task6, NULL, 3, KTASK_NORMAL, "test6");
	while (1)
	{
		uart_puts("Chillin' in task 2\r\n");
		tasks_sleep(500);
	}
}

void test_task() {
	uart_puts("Creating task 2 & 3");
	test2 = tasks_createTaskDynamic(100, test_task2, NULL, 3, KTASK_NORMAL, "test2");
	test3 = tasks_createTaskDynamic(100, test_task3, NULL, 3, KTASK_NORMAL, "test3");
	while (1)
	{
		uart_puts("Chillin' in task 1\r\n");
		tasks_sleep(500);
	}
}

int main(void)
{
    kernel_init();
	test = tasks_createTaskDynamic(100, test_task, NULL, 1, KTASK_NORMAL, "test1");
	test7 = tasks_createTaskDynamic(100, test_task7, NULL, 1, KTASK_NORMAL, "test7");
	ipc_mutexInit(&mutex);
    while (1)
    {
		asm volatile("nop"::);
		//uart_puts("Idling in mah main\r\n");
    }
}

