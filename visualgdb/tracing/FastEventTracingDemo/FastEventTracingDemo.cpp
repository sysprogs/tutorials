#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

static TIM_HandleTypeDef s_Timer = { TIM1 };

int main(void)
{
	HAL_Init();

	__GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_12;

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

    __GPIOA_CLK_ENABLE();
  
    GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    
    __TIM1_CLK_ENABLE();
    s_Timer.Init.Period = 0xFFFF;
    HAL_TIM_Base_Init(&s_Timer);
    HAL_TIM_Base_Start(&s_Timer);
    
    for (;;)
    {
    }
}

extern "C" void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (__HAL_TIM_GET_FLAG(&s_Timer, TIM_FLAG_UPDATE) != RESET || TIM1->CNT >= 10000)
    {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
    }
    else
    {
        asm("nop");
    }
    
    TIM1->CNT = 0;
    __HAL_TIM_CLEAR_FLAG(&s_Timer, TIM_FLAG_UPDATE);
}