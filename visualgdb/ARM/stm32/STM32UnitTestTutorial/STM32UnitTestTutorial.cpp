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

void MeasureSinTime()
{
    volatile double arg = 0.1;
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;
    volatile float result = sinf(arg);
    volatile unsigned cycles = DWT->CYCCNT;
    asm("nop");
}

int main(void)
{
	HAL_Init();

    MeasureSinTime();

    if (IsRunningUnitTests())
	{
	    RunAllTests();
	}
	return 0;
}
