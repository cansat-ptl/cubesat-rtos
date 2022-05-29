/*
 * test_build.cpp
 * 
 * Created: 29.05.2022 06:25:08
 * Author: ThePetrovich
 */


#include <CppUTest/TestHarness.h>
#include <kernel/kernel.h>
#include <stdio.h>

TEST_GROUP(TestAlloc) 
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

TEST(TestAlloc, Test_AllocTooMuch) 
{
	CHECK_EQUAL(NULL, mem_malloc(CFG_HEAP_SIZE));
}

TEST(TestAlloc, Test_AllocHalfTwiceFree) 
{
	void *mem = mem_malloc(CFG_HEAP_SIZE / 2);
	CHECK(mem != NULL);

	CHECK_EQUAL(NULL, mem_malloc(CFG_HEAP_SIZE / 2));
	mem_free(mem);
}

TEST(TestAlloc, Test_AllocHalfFreeRealloc) 
{
	void *mem = mem_malloc(CFG_HEAP_SIZE / 2);
	CHECK(mem != NULL);
	mem_free(mem);

	mem = mem_malloc(CFG_HEAP_SIZE / 2);
	CHECK(mem != NULL);
	mem_free(mem);
}

TEST(TestAlloc, Test_AllocDoubleFreeRealloc) 
{
	void *mem = mem_malloc(CFG_HEAP_SIZE / 2);
	CHECK(mem != NULL);
	mem_free(mem);
	mem_free(mem);
	
	mem = mem_malloc(CFG_HEAP_SIZE / 2);
	CHECK(mem != NULL);
	mem_free(mem);
}