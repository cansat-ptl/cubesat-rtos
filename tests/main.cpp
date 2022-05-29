/*
 * cubesat-rtos.c
 *
 * Created: 22.11.2020 20:06:27
 * Author : ThePetrovich
 */

#include <CppUTest/CommandLineTestRunner.h>
#include <kernel/kernel.h>
#include <stdio.h>

extern "C" {
	void uart_init()
	{
		return;
	}

	void uart_putc(char c)
	{
		printf("%c", c);
		return;
	}

	void uart_puts(char *msg)
	{
		printf("%s", msg);
		return;
	}
}

int main(int argc, char** argv)
{	
	kernel_init();

	MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
