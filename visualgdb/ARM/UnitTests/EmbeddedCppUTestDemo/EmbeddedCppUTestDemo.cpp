#include <stm32f4xx_hal.h>
#include <CppUTest/CommandLineTestRunner.h>

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

class MyClass
{
private:
    int ThisField;
    MyClass *Next;
	
public:
    void Test(int byValue, int *byPointer, int &byRef, int &&byMove, int byArr[4]);
    MyClass(int x, int y, int z)
    {
    }
};

class Child : MyClass
{
public:
    Child()
        : MyClass(1, 2, 3)
    {
    }
};

int main(void)
{
	HAL_Init();

	const char *p = "";
	CommandLineTestRunner::RunAllTests(0, &p);
	return 0;
}
