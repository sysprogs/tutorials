#include <FastSemihosting.h>
#include <SysprogsTestHooks.h>

#include <TinyEmbeddedTest.h>
#include <stdio.h>

/*
    This is a very basic sample demonstrating the Sysprogs TinyEmbeddedTest framework.
    The framework is a minimalistic embedded testing framework that aims at minimizing the memory footprint.
    It uses assertion macros and test/test group syntax from the CppUTest framework [https://cpputest.github.io/manual.html]
*/

TEST_GROUP(DemoTestGroup){
	void setup(){
		asm("nop");
}

void teardown()
{
	asm("nop");
}

void TestSetup(TestInstance *)
{
	asm("nop");
}

void TestTeardown(TestInstance *)
{
	asm("nop");
}
}
;

TEST(DemoTestGroup, FailingTest)
{
	OutputTestMessage("Hello from the failing test\n");
	LONGS_EQUAL(1, 1);
	FAIL("This test failed");
}

TEST(DemoTestGroup, SuccessfulTest1)
{
	//This test should succeed
	CHECK(true);
	CHECK_FALSE(false);
	CHECK_EQUAL(1, 1);
	STRCMP_EQUAL("abc", "abc");
	STRNCMP_EQUAL("abc1", "abc2", 3);
	STRCMP_NOCASE_EQUAL("abc", "ABC");
	STRCMP_CONTAINS("123", "test123");

	LONGS_EQUAL(-2, -2);
	UNSIGNED_LONGS_EQUAL(3, 3);
	UNSIGNED_LONGS_EQUAL_WITHIN(10, 11, 2);

	BYTES_EQUAL(-1, 255);
	POINTERS_EQUAL(0, 0);
	MEMCMP_EQUAL("string", "string", 6);
}

TEST(DemoTestGroup, SuccessfulTest2)
{
	//This test should succeed;
	OutputTestMessage("Hello from Test #2\n");
}

int main()
{
	if (IsRunningUnitTests())
	{
		RunAllTests();
	}
	return 0;
}