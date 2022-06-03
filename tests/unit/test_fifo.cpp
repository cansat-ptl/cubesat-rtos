/*
 * test_fifo.cpp
 * 
 * Created: 31.05.2022 11:29:22
 * Author: ThePetrovich
 */


#ifdef __INTELLISENSE__
	#pragma diag_suppress 384
#endif

#include <CppUTest/TestHarness.h>
#include <kernel/kernel.h>
#include <kernel/ipc/ipc.h>
#include <stdio.h>

TEST_GROUP(TestFifo) 
{
	void setup() 
	{
		// This gets run before every test
	}

	void teardown() 
	{
		// This gets run after every test
	}
};