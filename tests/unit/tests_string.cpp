/*
 * tests_string.cpp
 * 
 * Created: 04.06.2022 08:58:08
 * Author: ThePetrovich
 */


#ifdef __INTELLISENSE__
	#pragma diag_suppress 384
#endif

#include <CppUTest/TestHarness.h>
#include <kernel/kernel.h>
#include <kernel/common/string.h>
#include <string.h>

TEST_GROUP(TestString) 
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

/* These tests are self explanatory */

TEST(TestString, TEST_StringIsDigit) 
{
	CHECK_EQUAL(0, common_isdigit('A'));
	CHECK_EQUAL(0, common_isdigit('.'));
	CHECK_EQUAL(0, common_isdigit('\n'));
	CHECK_EQUAL(0, common_isdigit('\0'));
	CHECK_EQUAL(1, common_isdigit('0'));
	CHECK_EQUAL(1, common_isdigit('9'));
}

TEST(TestString, TEST_StringStrlen) 
{
	char empty[] = "";
	char test1[] = "test";
	char test2[] = "aaaaaaaaa\0aaa";

	CHECK_EQUAL(0, common_strlen(empty));
	CHECK_EQUAL(4, common_strlen(test1));
	CHECK_EQUAL(9, common_strlen(test2));
}

TEST(TestString, TEST_StringStrcmp) 
{
	CHECK_EQUAL(0, common_strcmp("", ""));
	CHECK_EQUAL(0, common_strcmp("aaa", "aaa"));
	CHECK_EQUAL(-1, common_strcmp("aa", "aaa"));
	CHECK_EQUAL(-1, common_strcmp("", "aaa"));
	CHECK_EQUAL(1, common_strcmp("aaa", "aa"));
	CHECK_EQUAL(1, common_strcmp("aaa", ""));
	CHECK_EQUAL(1, common_strcmp("aaa", "aa\0"));
	CHECK_EQUAL(1, common_strcmp("aaa", "a\0a"));
}

TEST(TestString, TEST_StringMemcmp) 
{
	CHECK_EQUAL(0, common_memcmp((void *)"", (void *)"", 0));
	CHECK_EQUAL(0, common_memcmp((void *)"aaa", (void *)"aaa", 3));
	CHECK_EQUAL('\0' - 'a', common_memcmp((void *)"aa\0", (void *)"aaa", 3));
	CHECK_EQUAL('a' - '\0', common_memcmp((void *)"aaa", (void *)"aa\0", 3));
}

TEST(TestString, TEST_StringMemset) 
{
	uint8_t buft1[10] = {0};
	uint8_t buft2[10] = {0};
	uint8_t buft3[10] = {0};
	uint8_t buft4[10] = {0};

	memset((void *)buft1, 0xDE, 10);

	common_memset((void *)buft2, 0xDE, 10);

	MEMCMP_EQUAL((void *)buft1, (void *)buft2, 10);

	common_memset((void *)buft3, 0xDE, 0);

	MEMCMP_EQUAL((void *)buft4, (void *)buft3, 10);
}

TEST(TestString, TEST_StringMemcpy) 
{
	uint8_t buft1[10] = {0};
	uint8_t buft2[10] = {0};
	uint8_t buft3[10] = {0};

	memset((void *)buft1, 0xDE, 10);

	common_memcpy((void *)buft2, (void *)buft1, 10);

	MEMCMP_EQUAL((void *)buft1, (void *)buft2, 10);

	common_memcpy((void *)buft2, (void *)buft3, 0);

	MEMCMP_EQUAL((void *)buft1, (void *)buft2, 10);
}