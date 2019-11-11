#include "mbed.h"
#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

using namespace utest::v1;

static control_t MbedLibraryTest1(const size_t call_count) 
{
    //TODO: Place your test logic here
    TEST_ASSERT_EQUAL(4, 2 * 2);

    return CaseNext;
}

utest::v1::status_t greentea_setup(const size_t number_of_cases) 
{
    GREENTEA_SETUP(60, "default_auto");

    return greentea_test_setup_handler(number_of_cases);
}

static Case s_TestCases[] = 
{
    Case("MbedLibraryTest1", MbedLibraryTest1),
	//TODO: add more unit tests here
};

static Specification s_TestSpecification(greentea_setup, s_TestCases);

/*  If you get the 'multiple definition of main()' error here, make sure 
    your main application's main() function is inside the following block:
   
    #ifndef MBED_TEST_MODE
    #endif 

    This will ensure that the regular main() function won't be compiled
    when building tests. */

int main() 
{
    return !Harness::run(s_TestSpecification);
}