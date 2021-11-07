/* Original author: narayananclover */
/* https://medium.com/@narayan.1979/dining-philosophers-implemented-in-freertos-989354ce993e */
/* https://gist.github.com/narayananclover/4c170f968ef4c41d3762d12e2f255bc2 */
#include <kernel/kernel.h>

#define NUM_OF_PHILOSOPHERS (5)
#define MAX_NUMBER_ALLOWED (NUM_OF_PHILOSOPHERS - 1)

kMutex_t forks[NUM_OF_PHILOSOPHERS];
kSemaphore_t entry_sem;
kTask_t *philosophers[NUM_OF_PHILOSOPHERS];

#define left(i) (i)
#define right(i) ((i + 1) % NUM_OF_PHILOSOPHERS)

void take_fork(int i) 
{
	ipc_mutexLock(&(forks[left(i)]));
	ipc_mutexLock(&(forks[right(i)]));
	debug_printk("Philosopher %d got the fork %d and %d\n", i, left(i), right(i));
}

void put_fork(int i) 
{
	ipc_mutexUnlock(&(forks[left(i)]));
	ipc_mutexUnlock(&(forks[right(i)]));
	debug_printk("Philosopher %d Gave up the fork %d and %d\n", i, left(i), right(i));
}

void philosophers_task(void *param) 
{
	int i = *(int *)param;

	while (1) {
		ipc_semaphoreWait(&entry_sem);

		take_fork(i);

		debug_printk("Philosopher %d is eating\n", i);

		// Add a Delay to eat. Not Required but be practical.
    		tasks_sleep(10);

		put_fork(i);

		ipc_semaphoreSignal(&entry_sem);
    
    		// This is not required. But practical
		tasks_sleep(10);
	}
}

int main() 
{
	int i;
	int param[NUM_OF_PHILOSOPHERS];

	kernel_init();

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
		tasks_createTaskDynamic(200, philosophers_task, &(param[i]), 2, KTASK_NORMAL, "task");
	}

	kernel_startScheduler();

	while(1);
}