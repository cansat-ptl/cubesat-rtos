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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int testCriticalLevel;

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

TEST(TestFifo, Test_FifoInit) 
{
	uint8_t buffer[100] = {0};
	uint8_t bufferRead[100] = {0};
	kFIFO_t fifo;

	/* Initialize empty fifo with 100 byte buffer and item size 1 */
	ipc_fifoInit(&fifo, buffer, 100, 1);

	/* Check if available for reading, should return 0 */
	size_t avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(0, avail);

	/* Peek, should return 0 bytes read */
	size_t peek = ipc_fifoPeek(&fifo, (void *)bufferRead);
	CHECK_EQUAL(0, peek);

	/* Free space in fifo, should return 100 bytes */
	size_t free = ipc_fifoFreeSpace(&fifo);
	CHECK_EQUAL(100, free);
	
	CHECK_EQUAL(0, testCriticalLevel);
}

TEST(TestFifo, Test_FifoReadEmpty) 
{
	uint8_t buffer[100] = {0};
	uint8_t bufferRead[100] = {0};
	uint8_t bufferEmpty[100] = {0};
	kFIFO_t fifo;

	/* Initialize empty fifo with 100 byte buffer and item size 1 */
	ipc_fifoInit(&fifo, buffer, 100, 1);

	/* Read empty buffer, should return 0 */
	size_t read = ipc_fifoRead(&fifo, (void *)bufferRead);
	CHECK_EQUAL(0, read);

	/* Check if nothing actually was read */
	MEMCMP_EQUAL(bufferEmpty, bufferRead, 100);

	CHECK_EQUAL(0, testCriticalLevel);
}

TEST(TestFifo, Test_FifoWriteRead1) 
{
	uint8_t buffer[100] = {0};
	uint8_t bufferRead[100] = {0};
	uint8_t bufferTest[100] = {0};
	uint8_t testVal = 0xDE;
	kFIFO_t fifo;

	/* Initialize empty fifo with 100 byte buffer and item size of 1 byte */
	ipc_fifoInit(&fifo, buffer, 100, 1);

	/* Write 1 byte, should return 1 */
	size_t write = ipc_fifoWrite(&fifo, (void *)&testVal);
	CHECK_EQUAL(1, write);

	/* Write 10 bytes in a loop, should return 1 each time */
	for (int i = 0; i < 10; i++) {
		write += ipc_fifoWrite(&fifo, (void *)&testVal);
	}
	
	/* 11 bytes written in total */
	CHECK_EQUAL(11, write);

	/* Check if available for reading, should return 11 bytes */
	size_t avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(11, avail);

	/* Peek, should return 1 bytes read */
	size_t peek = ipc_fifoPeek(&fifo, (void *)bufferRead);
	CHECK_EQUAL(1, peek);
	CHECK_EQUAL(testVal, bufferRead[0]);

	/* Check if peek didn't move any pointers */
	avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(11, avail);

	/* Free space in fifo, should return 100-write bytes */
	size_t free = ipc_fifoFreeSpace(&fifo);
	CHECK_EQUAL(100-write, free);

	/* Read 10 bytes in a loop, should return 1 each time */
	/* Fill expected output buffer at the same time */
	size_t read = 0;
	for (int i = 0; i < 10; i++) {
		read += ipc_fifoRead(&fifo, (void *)&bufferRead[read]);
		bufferTest[i] = testVal;
	}

	/* 10 bytes read in total */
	CHECK_EQUAL(10, read);

	/* Check if available for reading, should return 1 byte */
	avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(1, avail);

	/* Free space in fifo, should return 99 bytes */
	free = ipc_fifoFreeSpace(&fifo);
	CHECK_EQUAL(99, free);

	/* Check if bytes were read correctly */
	MEMCMP_EQUAL(bufferTest, bufferRead, 100);

	CHECK_EQUAL(0, testCriticalLevel);
}

TEST(TestFifo, Test_FifoWriteRead10) 
{
	uint8_t buffer[100] = {0};
	uint8_t bufferRead[100] = {0};
	uint8_t bufferTest[100] = {0};
	uint8_t testVal[10] = {0xDE};
	kFIFO_t fifo;

	memset(testVal, 0xDE, 10);

	/* Initialize empty fifo with 100 byte buffer and item size of 10 bytes */
	ipc_fifoInit(&fifo, buffer, 100, 10);

	/* Write 10 bytes, should return 10 */
	size_t write = ipc_fifoWrite(&fifo, (void *)testVal);
	CHECK_EQUAL(10, write);

	/* Write 50 bytes in a loop, should return 10 each time */
	for (int i = 0; i < 5; i++) {
		write += ipc_fifoWrite(&fifo, (void *)testVal);
	}
	
	/* 60 bytes written in total */
	CHECK_EQUAL(60, write);

	/* Check if available for reading, should return 60 bytes */
	size_t avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(60, avail);

	/* Peek, should return 10 bytes read */
	size_t peek = ipc_fifoPeek(&fifo, (void *)bufferRead);
	CHECK_EQUAL(10, peek);
	MEMCMP_EQUAL(testVal, bufferRead, 10);

	/* Check if peek didn't move any pointers */
	avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(60, avail);

	/* Free space in fifo, should return 100-write bytes */
	size_t free = ipc_fifoFreeSpace(&fifo);
	CHECK_EQUAL(100-write, free);

	/* Read 50 bytes in a loop, should return 1 each time */
	size_t read = 0;
	for (int i = 0; i < 5; i++) {
		read += ipc_fifoRead(&fifo, (void *)&bufferRead[read]);
	}

	memset(bufferTest, 0xDE, 50);

	/* 50 bytes read in total */
	CHECK_EQUAL(50, read);

	/* Check if available for reading, should return 10 bytes */
	avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(10, avail);

	/* Free space in fifo, should return 90 bytes */
	free = ipc_fifoFreeSpace(&fifo);
	CHECK_EQUAL(90, free);

	/* Check if bytes were read correctly */
	MEMCMP_EQUAL(bufferTest, bufferRead, 100);

	CHECK_EQUAL(0, testCriticalLevel);
}

TEST(TestFifo, Test_FifoWriteRead10Full) 
{
	uint8_t buffer[100] = {0};
	uint8_t bufferRead[100] = {0};
	uint8_t bufferTest[100] = {0};
	uint8_t testVal[10] = {0xDE};
	kFIFO_t fifo;

	memset(testVal, 0xDE, 10);

	/* Initialize empty fifo with 100 byte buffer and item size of 10 bytes */
	ipc_fifoInit(&fifo, buffer, 100, 10);

	/* Write 10 bytes, should return 10 */
	size_t write = ipc_fifoWrite(&fifo, (void *)testVal);
	CHECK_EQUAL(10, write);

	/* Try to write 100 bytes in a loop, should return 10 each time */
	for (int i = 0; i < 10; i++) {
		write += ipc_fifoWrite(&fifo, (void *)testVal);
	}
	
	/* 100 bytes written in total */
	CHECK_EQUAL(100, write);

	/* Check if available for reading, should return 60 bytes */
	size_t avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(100, avail);

	/* Peek, should return 10 bytes read */
	size_t peek = ipc_fifoPeek(&fifo, (void *)bufferRead);
	CHECK_EQUAL(10, peek);
	MEMCMP_EQUAL(testVal, bufferRead, 10);

	/* Check if peek didn't move any pointers */
	avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(100, avail);

	/* Free space in fifo, should return 0 as fifo is full */
	size_t free = ipc_fifoFreeSpace(&fifo);
	CHECK_EQUAL(0, free);

	/* Read 50 bytes in a loop, should return 10 each time */
	size_t read = 0;
	for (int i = 0; i < 5; i++) {
		read += ipc_fifoRead(&fifo, (void *)&bufferRead[read]);
	}

	memset(bufferTest, 0xDE, 50);

	/* 50 bytes read in total */
	CHECK_EQUAL(50, read);

	/* Check if available for reading, should return 10 bytes */
	avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(50, avail);

	/* Free space in fifo, should return 50 bytes */
	free = ipc_fifoFreeSpace(&fifo);
	CHECK_EQUAL(50, free);

	/* Check if bytes were read correctly */
	MEMCMP_EQUAL(bufferTest, bufferRead, 100);

	/* Try to read 100 bytes in a loop, should return 10 each time */
	for (int i = 0; i < 100; i++) {
		read += ipc_fifoRead(&fifo, (void *)&bufferRead[read]);
	}

	memset(bufferTest, 0xDE, 100);

	/* 100 bytes read in total */
	CHECK_EQUAL(100, read);

	/* Check if available for reading, should return 0 bytes */
	avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(0, avail);

	/* Free space in fifo, should return 100 bytes */
	free = ipc_fifoFreeSpace(&fifo);
	CHECK_EQUAL(100, free);

	/* Check if bytes were read correctly */
	MEMCMP_EQUAL(bufferTest, bufferRead, 100);

	CHECK_EQUAL(0, testCriticalLevel);
}

TEST(TestFifo, Test_FifoWriteRead11Full) 
{
	uint8_t buffer[100] = {0};
	uint8_t bufferRead[100] = {0};
	uint8_t bufferTest[100] = {0};
	uint8_t testVal[11] = {0xDE};
	kFIFO_t fifo;

	memset(testVal, 0xDE, 11);

	/* Initialize empty fifo with 100 byte buffer and item size of 11 bytes */
	ipc_fifoInit(&fifo, buffer, 100, 11);

	/* Write 11 bytes, should return 11 */
	size_t write = ipc_fifoWrite(&fifo, (void *)testVal);
	CHECK_EQUAL(11, write);

	/* Try to write 110 bytes in a loop, should return 11 each time */
	for (int i = 0; i < 10; i++) {
		write += ipc_fifoWrite(&fifo, (void *)testVal);
	}
	
	/* 99 bytes written in total */
	CHECK_EQUAL(99, write);

	/* Check if available for reading, should return 99 bytes */
	size_t avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(99, avail);

	/* Peek, should return 11 bytes read */
	size_t peek = ipc_fifoPeek(&fifo, (void *)bufferRead);
	CHECK_EQUAL(11, peek);
	MEMCMP_EQUAL(testVal, bufferRead, 11);

	/* Check if peek didn't move any pointers */
	avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(99, avail);

	/* Free space in fifo, should return 0 as fifo is full */
	size_t free = ipc_fifoFreeSpace(&fifo);
	CHECK_EQUAL(0, free);

	/* Read 55 bytes in a loop, should return 10 each time */
	size_t read = 0;
	for (int i = 0; i < 5; i++) {
		read += ipc_fifoRead(&fifo, (void *)&bufferRead[read]);
	}

	memset(bufferTest, 0xDE, 55);

	/* 55 bytes read in total */
	CHECK_EQUAL(55, read);

	/* Check if available for reading, should return 44 bytes */
	avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(44, avail);

	/* Free space in fifo, should return 56 bytes */
	free = ipc_fifoFreeSpace(&fifo);
	CHECK_EQUAL(56, free);

	/* Check if bytes were read correctly */
	MEMCMP_EQUAL(bufferTest, bufferRead, 100);

	/* Try to read 110 bytes in a loop, should return 11 each time */
	for (int i = 0; i < 100; i++) {
		read += ipc_fifoRead(&fifo, (void *)&bufferRead[read]);
	}

	memset(bufferTest, 0xDE, 99);

	/* 99 bytes read in total */
	CHECK_EQUAL(99, read);

	/* Check if available for reading, should return 0 bytes */
	avail = ipc_fifoAvailable(&fifo);
	CHECK_EQUAL(0, avail);

	/* Free space in fifo, should return 100 bytes */
	free = ipc_fifoFreeSpace(&fifo);
	CHECK_EQUAL(100, free);

	/* Check if bytes were read correctly */
	MEMCMP_EQUAL(bufferTest, bufferRead, 100);

	CHECK_EQUAL(0, testCriticalLevel);
}