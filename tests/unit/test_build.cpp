/*
 * test_build.cpp
 * 
 * Created: 29.05.2022 06:25:08
 * Author: ThePetrovich
 */


#ifdef __INTELLISENSE__
	#pragma diag_suppress 384
#endif

#include <CppUTest/TestHarness.h>

TEST_GROUP(TestBuild) 
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

TEST(TestBuild, Test_BuildBasic) 
{
	LONGS_EQUAL(1, 1);
}