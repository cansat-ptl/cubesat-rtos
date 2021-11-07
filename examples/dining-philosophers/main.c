/* Original author: narayananclover */
/* https://medium.com/@narayan.1979/dining-philosophers-implemented-in-freertos-989354ce993e */
/* https://gist.github.com/narayananclover/4c170f968ef4c41d3762d12e2f255bc2 */
#include <kernel/kernel.h>
#include <kernel/arch/arch.h>
#include <util/delay.h>
#include <stdio.h>
#include <util/atomic.h>

#define NUM_OF_PHILOSOPHERS (10)
#define MAX_NUMBER_ALLOWED (NUM_OF_PHILOSOPHERS-1)

kMutex_t forks[NUM_OF_PHILOSOPHERS];
kMutex_t print_mutex;
kSemaphore_t entry_sem;
kTask_t *philosophers[NUM_OF_PHILOSOPHERS];

#define left(i) (i)
#define right(i) ((i + 1) % NUM_OF_PHILOSOPHERS)

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
 
static int uart_putchar(char c, FILE *stream)
{
	uart_putc(c);
	return 0;
}
 
void take_fork(int i) 
{
	ipc_mutexLock(&(forks[left(i)]));
	ipc_mutexLock(&(forks[right(i)]));

	ipc_mutexLock(&print_mutex);
	printf("Philosopher %d got the fork %d and %d\r\n", i, left(i), right(i));
	ipc_mutexUnlock(&print_mutex);
}

void put_fork(int i) 
{
	ipc_mutexLock(&print_mutex);
	printf("Philosopher %d Gave up the fork %d and %d\r\n", i, left(i), right(i));
	ipc_mutexUnlock(&print_mutex);
	
	ipc_mutexUnlock(&(forks[left(i)]));
	ipc_mutexUnlock(&(forks[right(i)]));
}

void philosophers_task(void *param) 
{
	int i = *(int *)param;
	kTaskTicks_t starveTime = 0;
	kTaskTicks_t eatTime = 0;

	while (1) {
		starveTime = tasks_getSysTickCount() - eatTime;

		if (starveTime > 95) {
			ipc_mutexLock(&print_mutex);
			printf("--------> Philosopher %d starved to death, time: %d\r\n", i, starveTime);
			ipc_mutexUnlock(&print_mutex);
			break;
		}

		ipc_mutexLock(&print_mutex);
		printf("Philosopher %d is eating, starve time: %d\r\n", i, starveTime);
		ipc_mutexUnlock(&print_mutex);

		ipc_semaphoreWait(&entry_sem);
		take_fork(i);

		tasks_sleep(5);
		eatTime = tasks_getSysTickCount();

		put_fork(i);

		ipc_semaphoreSignal(&entry_sem);
    
		tasks_sleep(5);
	}
}

int main() 
{
	int i;
	int param[NUM_OF_PHILOSOPHERS];

	stdout = &mystdout;

	kernel_init();
	ipc_mutexInit(&(print_mutex));

	// Create Five Semaphores for the five shared resources. 
	// Which is the fork in this case.
	for (i = 0; i < NUM_OF_PHILOSOPHERS; i++) {
		ipc_mutexInit(&(forks[i]));
	}

	// This is the critical piece to avoid deadlock.
	// If one less philosopher is allowed to act then there will no deadlock.
	// As one philosopher will always get two forks and so it will go on.

	ipc_semaphoreInit(&entry_sem, MAX_NUMBER_ALLOWED);

	for (i = 0; i < NUM_OF_PHILOSOPHERS; i++) {
		// Ofcourse, you can just pass i as every thread needs it's own
		// address to store the parameter.
		param[i] = i;
		philosophers[i] = tasks_createTaskDynamic(120, philosophers_task, &(param[i]), 5, KTASK_NORMAL, "task");
	}

	kernel_startScheduler();

	while(1);
}