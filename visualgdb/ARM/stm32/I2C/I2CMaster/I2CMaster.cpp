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

    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    __GPIOB_CLK_ENABLE();
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

I2C_HandleTypeDef hI2C;
static DMA_HandleTypeDef s_DMAHandle = DMA_HandleTypeDef();

extern "C" void I2C1_EV_IRQHandler()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
    HAL_I2C_EV_IRQHandler(&hI2C);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
}

extern "C" void DMA1_Stream6_IRQHandler()
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_DMA_IRQHandler(&s_DMAHandle);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
}

extern "C" void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    asm("bkpt 255");
}

int main(void)
{
	HAL_Init();
    ConfigureI2CPins();

    hI2C.Instance = I2C1;

    hI2C.Init.AddressingMode = I2C_ADDRESSINGMODE_10BIT;
    hI2C.Init.ClockSpeed = 10240;
    hI2C.Init.DutyCycle = I2C_DUTYCYCLE_2;

    __I2C1_CLK_ENABLE();

    if (HAL_I2C_Init(&hI2C) != HAL_OK)
        asm("bkpt 255");

    uint8_t tempBuffer[4096];
    for (int i = 0; i < sizeof(tempBuffer); i++)
        tempBuffer[i] = i;

    __DMA1_CLK_ENABLE();
    s_DMAHandle.Instance = DMA1_Stream6;
    s_DMAHandle.Init.Channel = DMA_CHANNEL_1;

    s_DMAHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    s_DMAHandle.Init.PeriphInc = DMA_PINC_DISABLE;
    s_DMAHandle.Init.MemInc = DMA_MINC_ENABLE;
    s_DMAHandle.Init.Mode = DMA_NORMAL;

    s_DMAHandle.Init.Priority = DMA_PRIORITY_VERY_HIGH;

    s_DMAHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    s_DMAHandle.Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;

    if (HAL_DMA_Init(&s_DMAHandle) != HAL_OK)
        asm("bkpt 255");

    __HAL_LINKDMA(&hI2C, hdmatx, s_DMAHandle);
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
    NVIC_EnableIRQ(I2C1_EV_IRQn);

    if (HAL_I2C_Master_Transmit_DMA(&hI2C, 0x15B, tempBuffer, sizeof(tempBuffer)) != HAL_OK)
    {
        asm("bkpt 255");
    }

    while (HAL_I2C_GetState(&hI2C) != HAL_I2C_STATE_READY)
    {
    }

    asm("bkpt 255");
}
