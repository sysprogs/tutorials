#include <stm32f4xx_hal.h>
#include <TinyEmbeddedTest.h>
#include <SysprogsTestHooks.h>

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

int main(void)
{
	HAL_Init();

	if (IsRunningUnitTests())
	{
	    RunAllTests();
	}
	return 0;
}
