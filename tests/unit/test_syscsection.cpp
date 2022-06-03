/*
 * test_syscsection.cpp
 * 
 * Created: 03.06.2022 04:57:55
 * Author: ThePetrovich
 */


#ifdef __INTELLISENSE__
	#pragma diag_suppress 384
#endif

#include <CppUTest/TestHarness.h>
#include <kernel/kernel.h>
#include <kernel/ipc/ipc.h>
#include <stdio.h>

int testCriticalLevel = 0;

extern "C" {
        void arch_enterCriticalSection()
        {
                testCriticalLevel++;
        }

        void arch_exitCriticalSection()
        {
                testCriticalLevel--;
        }
}

TEST_GROUP(TestCriticalSection) 
{
	void setup() 
	{
                testCriticalLevel = 0;
		// This gets run before every test
	}

	void teardown() 
	{
                testCriticalLevel = 0;
		// This gets run after every test
	}
};