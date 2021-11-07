#include <kernel/kernel.h>

kMutex_t exampleMutex; // Mutex declaration

void task1(void* args)
{
	debug_printk("task1: Task 1 starts\r\n");

	while (1) {
		ipc_mutexLock(&exampleMutex); // Locking mutex by its handle
		debug_printk("task1: Hello from task 1!\r\n");

		tasks_sleep(50);

		debug_printk("task1: Goodbye from task 1!\r\n");
		ipc_mutexUnlock(&exampleMutex); // Unlocking mutex

		tasks_sleep(50);
	}
}

void task2(void* args)
{
	debug_printk("task2: Task 2 starts\r\n");

	while (1) {
		ipc_mutexLock(&exampleMutex);
		debug_printk("task2: Hello from task 2!\r\n");

		tasks_sleep(100);

		debug_printk("task2: Goodbye from task 2!\r\n");
		ipc_mutexUnlock(&exampleMutex);

		tasks_sleep(100);
	}
}

void task3(void* args)
{
	debug_printk("task3: Task 3 starts\r\n");

	while (1) {
		ipc_mutexLock(&exampleMutex);
		debug_printk("task3: Hello from task 3! %s\r\n", (char*)args);
                
		tasks_sleep(100);

		debug_printk("task3: Goodbye from task 3!\r\n");
		ipc_mutexUnlock(&exampleMutex);

		tasks_sleep(100);
	}
}

int main()
{       
        kernel_init();

        ipc_mutexInit(&exampleMutex); // Mutex init

        static char exampleParameter[] = "Spaghetti and meatballs";
	tasks_createTaskDynamic(250, task1, NULL, 3, KTASK_NORMAL, "task1");
	tasks_createTaskDynamic(250, task2, NULL, 2, KTASK_NORMAL, "task2");
	tasks_createTaskDynamic(250, task3, (void*)exampleParameter, 3, KTASK_NORMAL, "task3");

        kernel_startScheduler();

	while (1);
}