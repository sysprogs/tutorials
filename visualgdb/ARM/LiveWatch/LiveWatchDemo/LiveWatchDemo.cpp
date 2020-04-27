#include <stm32_hal_legacy.h>
#include <stm32f4xx_hal.h>

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

#include <math.h>
volatile float g_Arg;
volatile float g_Sin;
volatile float g_Cos;

int main(void)
{
    HAL_Init();
    __GPIOD_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

    for (g_Arg = 0;; g_Arg += 0.01F)
    {
        g_Sin = sinf(g_Arg);
        g_Cos = cosf(g_Arg);

        int totalCycles = 5000;
        int onCycles = (int)(totalCycles * (g_Sin + 1)) / 2;
        int offCycles = totalCycles - onCycles;

        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
        for (int i = 0; i < onCycles; i++)
            asm("nop");
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
        for (int i = 0; i < offCycles; i++)
            asm("nop");
    }
}