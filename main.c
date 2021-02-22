/*
 * cubesat-rtos.c
 *
 * Created: 22.11.2020 20:06:27
 * Author : Admin
 */

#define F_CPU 16000000L

#include <avr/io.h>
#include <rtos/rtos.h>
#include <rtos/arch/arch.h>

kTaskHandle_t test;
kTaskHandle_t test2;

kMutex_t mutex;

void test_task() {
	while (1)
	{
		asm volatile("nop"::);
		ipc_mutexLock(&mutex);
		uart_puts("task1 Mutex locked\r\n");
		tasks_sleep(500);
		ipc_mutexUnlock(&mutex);
		uart_puts("task1 Mutex unlocked\r\n");
		tasks_sleep(500);
	}
}

void test_task2() {
	while (1)
	{
		asm volatile("nop"::);
		ipc_mutexLock(&mutex);
		uart_puts("task2 Mutex locked\r\n");
		tasks_sleep(100);
		ipc_mutexUnlock(&mutex);
		uart_puts("task2 Mutex unlocked\r\n");
		tasks_sleep(100);
	}
}

int main(void)
{
    kernel_init();
	test = tasks_createTaskDynamic(100, test_task, NULL, 1, KTASK_NORMAL, "test1");
	test2 = tasks_createTaskDynamic(100, test_task2, NULL, 3, KTASK_NORMAL, "test2");
	ipc_mutexInit(&mutex);
    while (1)
    {
		asm volatile("nop"::);
		//uart_puts("Idling in mah main\r\n");
    }
}

