/*
 * test_alloc.cpp
 * 
 * Created: 29.05.2022 07:41:30
 * Author: ThePetrovich
 */


#ifdef __INTELLISENSE__
	#pragma diag_suppress 384
#endif

#include <CppUTest/TestHarness.h>
#include <kernel/kernel.h>
#include <kernel/mem/heap.h>
#include <stdio.h>

 //extern bool verbose_;

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

TEST(TestAlloc, Test_AllocZero) 
{
	size_t freeHeapPre = mem_getFreeHeap();

	/* Try to allocate nothing, should fail */
	CHECK_EQUAL(NULL, mem_malloc(0));

	/* Check if nothing was allocated */
	CHECK_EQUAL(freeHeapPre, mem_getFreeHeap());
}

TEST(TestAlloc, Test_AllocOne) 
{
	size_t freeHeapPre = mem_getFreeHeap();

	/* Allocate 1 byte, should succeed */
	void *mem = mem_malloc(1);

	CHECK(mem != NULL);

	/* Check if byte alignment was applied */
	CHECK_EQUAL(freeHeapPre - (mem_HEAP_STRUCT_SIZE + (1 + (CFG_PLATFORM_BYTE_ALIGNMENT - (1 & CFG_PLATFORM_BYTE_ALIGNMENT_MASK)))), mem_getFreeHeap())

	mem_free(mem);

	/* Check if everything was freed */
	CHECK_EQUAL(freeHeapPre, mem_getFreeHeap());
}

TEST(TestAlloc, Test_AllocTooMuch) 
{
	size_t freeHeapPre = mem_getFreeHeap();

	/* Try to allocate all the heap space, should fail due to allocation overhead */
	CHECK_EQUAL(NULL, mem_malloc(CFG_HEAP_SIZE));

	/* Check if nothing was allocated */
	CHECK_EQUAL(freeHeapPre, mem_getFreeHeap());
}

TEST(TestAlloc, Test_AllocAlmostTooMuch) 
{
	size_t freeHeapPre = mem_getFreeHeap();

	/* Try to allocate all the free heap, should succeed */
	void *mem = mem_malloc(freeHeapPre - mem_HEAP_STRUCT_SIZE);
	
	CHECK(mem != NULL);

	/* Check if there is no free heap space */
	CHECK_EQUAL(0, mem_getFreeHeap());

	mem_free(mem);

	/* Check if everything was freed */
	CHECK_EQUAL(freeHeapPre, mem_getFreeHeap());
}

TEST(TestAlloc, Test_AllocHalfTwiceFree) 
{
	size_t freeHeapPre = mem_getFreeHeap();

	/* Allocate half of the heap space, should succeed */
	void *mem = mem_malloc(CFG_HEAP_SIZE / 2);
	CHECK(mem != NULL);

	/* Try to allocate half of the heap space again, should fail due to allocation overhead */
	CHECK_EQUAL(NULL, mem_malloc(CFG_HEAP_SIZE / 2));
	mem_free(mem);

	/* Check if everything was freed */
	CHECK_EQUAL(freeHeapPre, mem_getFreeHeap());
}

TEST(TestAlloc, Test_AllocHalfFreeRealloc) 
{
	size_t freeHeapPre = mem_getFreeHeap();

	/* Allocate half of the heap space then free, should succeed */
	void *mem = mem_malloc(CFG_HEAP_SIZE / 2);
	CHECK(mem != NULL);
	mem_free(mem);

	/* Allocate half of the heap space then free again, should succeed */
	mem = mem_malloc(CFG_HEAP_SIZE / 2);
	CHECK(mem != NULL);
	mem_free(mem);

	/* Check if everything was freed */
	CHECK_EQUAL(freeHeapPre, mem_getFreeHeap());
}

TEST(TestAlloc, Test_AllocDoubleFreeRealloc) 
{
	size_t freeHeapPre = mem_getFreeHeap();

	/* Allocate half of the heap space then double free, should succeed */
	void *mem = mem_malloc(CFG_HEAP_SIZE / 2);
	CHECK(mem != NULL);
	mem_free(mem);

	/* Double free */
	mem_free(mem);
	
	/* Allocate half of the heap space, should succeed */
	mem = mem_malloc(CFG_HEAP_SIZE / 2);
	CHECK(mem != NULL);
	mem_free(mem);
	
	/* Check if everything was freed */
	CHECK_EQUAL(freeHeapPre, mem_getFreeHeap());
}

TEST(TestAlloc, Test_AllocFreeDefragmentation) 
{
	size_t freeHeapPre = mem_getFreeHeap();

	/* Allocate 2 quarters of the heap space, both should succeed */
	void *mem1 = mem_malloc(CFG_HEAP_SIZE / 4);
	void *mem2 = mem_malloc(CFG_HEAP_SIZE / 4);

	CHECK(mem1 != NULL);
	CHECK(mem2 != NULL);

	/* Try to allocate half of the heap space, should fail */
	CHECK_EQUAL(NULL, mem_malloc(CFG_HEAP_SIZE / 2));

	/* Allocate 1 more quarter of the heap space, should succeed */
	void *mem3 = mem_malloc(CFG_HEAP_SIZE / 4);

	CHECK(mem3 != NULL);

	/* 4th quarter allocation should fail due to allocation overhead */
	CHECK_EQUAL(NULL, mem_malloc(CFG_HEAP_SIZE / 4));

	/* Free the first 2 quarters */
	mem_free(mem1);
	mem_free(mem2);

	/* Try to allocate half of the heap space, should succeed as 
	 * 2 quarters are larger than 1 half due to allocation overhead */
	void *mem4 = mem_malloc(CFG_HEAP_SIZE / 2);

	CHECK(mem4 != NULL);

	mem_free(mem3);
	mem_free(mem4);

	/* Try to allocate 2 quarter and 1 third, should succeed */
	mem1 = mem_malloc(CFG_HEAP_SIZE / 4);
	mem2 = mem_malloc(CFG_HEAP_SIZE / 3);
	mem3 = mem_malloc(CFG_HEAP_SIZE / 4);

	CHECK(mem1 != NULL);
	CHECK(mem2 != NULL);
	CHECK(mem3 != NULL);

	/* Clean up */
	mem_free(mem1);
	mem_free(mem2);
	mem_free(mem3);

	/* Check if everything was freed */
	CHECK_EQUAL(freeHeapPre, mem_getFreeHeap());
}