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

void ConfigureI2CPins()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    __GPIOB_CLK_ENABLE();
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

int main(void)
{
    HAL_Init();
    ConfigureI2CPins();

    I2C_HandleTypeDef hI2C = I2C_HandleTypeDef();
    hI2C.Instance = I2C1;

    hI2C.Init.AddressingMode = I2C_ADDRESSINGMODE_10BIT;
    hI2C.Init.ClockSpeed = 10240;
    hI2C.Init.OwnAddress1 = 0x15B;
    hI2C.Init.DutyCycle = I2C_DUTYCYCLE_2;

    __I2C1_CLK_ENABLE();

    if (HAL_I2C_Init(&hI2C) != HAL_OK)
        asm("bkpt 255");

    uint8_t buffer[4096];

    for (;;)
    {
        HAL_I2C_Slave_Receive(&hI2C, buffer, sizeof(buffer), HAL_MAX_DELAY);
    }
}
