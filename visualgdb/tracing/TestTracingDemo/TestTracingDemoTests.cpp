#include <TinyEmbeddedTest.h>
#include <MemoryLeakTests.h>
#include <stdio.h>

/*
    This is a very basic sample demonstrating the Sysprogs TinyEmbeddedTest framework.
    The framework is a minimalistic embedded testing framework that aims at minimizing the memory footprint.
    It uses assertion macros and test/test group syntax from the CppUTest framework [https://cpputest.github.io/manual.html]
*/

TEST_GROUP(DemoTestGroup)
{
    void setup()
    {
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
};

class MyNumber
{
private:
    int Value;
    
public:
    MyNumber(int x)
        : Value(x)
    {
    }
    
    MyNumber operator+(const MyNumber &right) const
    {
        return MyNumber(Value + right.Value);
    }
    
    MyNumber operator*(const MyNumber &right) const
    {
        return MyNumber(Value * right.Value);
    }    
    
    bool operator==(const MyNumber &right) const
    {
        return Value == right.Value;
    }
};


void TestBinomial(MyNumber x, MyNumber y)
{
    CHECK_EQUAL((x + y) * (x + y), (x * x) + MyNumber(2) * x * y + (y * y));
}

TEST(DemoTestGroup, MathTest)
{
    TestBinomial(1, 2);
    TestBinomial(3, 4);
}
