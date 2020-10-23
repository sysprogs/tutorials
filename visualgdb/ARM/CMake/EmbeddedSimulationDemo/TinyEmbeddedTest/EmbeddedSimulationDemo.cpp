#ifndef SIMULATION
#include <stm32f4xx_hal.h>
#endif

#include <TinyEmbeddedTest.h>
#include <SysprogsTestHooks.h>

#ifdef __cplusplus
extern "C"
#endif
    void
    SysTick_Handler(void)
{
#ifndef SIMULATION
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
#endif
}

int main(void)
{
#ifndef SIMULATION
    HAL_Init();
#endif

    if (IsRunningUnitTests())
    {
        RunAllTests();
    }
    return 0;
}
