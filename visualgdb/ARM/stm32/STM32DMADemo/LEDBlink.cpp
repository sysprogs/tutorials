#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>

#ifdef __cplusplus
extern "C"
#endif
    void
    SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

#include <memory.h>

static int s_Buffer1[1024], s_Buffer2[1024], s_Buffer3[1024];

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

void __attribute__((noinline)) FillMemory()
{
    for (int i = 0; i < ARRAY_SIZE(s_Buffer1); i++)
        s_Buffer1[i] = i * 3;
}

void __attribute__((noinline)) CopyMemory()
{
    memcpy(s_Buffer2, s_Buffer1, sizeof(s_Buffer2));
}

void __attribute__((noinline)) UpdateFibonacci()
{
    s_Buffer3[0] = 0;
    s_Buffer3[1] = 1;

    for (int i = 2; i < ARRAY_SIZE(s_Buffer3); i++)
    {
        s_Buffer3[i] = s_Buffer3[i - 1] + s_Buffer3[i - 2];
    }
}

#include <stm32f4xx_hal_dma.h>

static DMA_HandleTypeDef s_DMAHandle;

#include <stm32f4xx_hal_uart.h>
static UART_HandleTypeDef s_UARTHandle;

static void SetupUART()
{
    __USART2_CLK_ENABLE();
    s_UARTHandle.Instance = USART2;

    s_UARTHandle.Init.BaudRate = 115200;
    s_UARTHandle.Init.WordLength = UART_WORDLENGTH_8B;
    s_UARTHandle.Init.StopBits = UART_STOPBITS_1;
    s_UARTHandle.Init.Parity = UART_PARITY_NONE;
    s_UARTHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    s_UARTHandle.Init.Mode = UART_MODE_TX_RX;
    s_UARTHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&s_UARTHandle) != HAL_OK)
        asm("bkpt 255");

    GPIO_InitTypeDef GPIO_InitStruct;
    __GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void __attribute__((noinline)) UpdateFibonacci(int *pBuf, size_t count)
{
    static int s_Num1 = 0, s_Num2 = 1;
    
    int tmp1 = s_Num1, tmp2 = s_Num2;

    for (int i = 0; i < (count - 1); i+= 2)
    {
        pBuf[i] = tmp1;
        pBuf[i + 1] = tmp2;

        tmp1 += tmp2;
        tmp2 += tmp1;
    }

    s_Num1 = tmp1;
    s_Num2 = tmp2;
}

extern "C" {
    void DMA1_Stream6_IRQHandler()
    {
        HAL_DMA_IRQHandler(&s_DMAHandle);
    }

    void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
    {
        UpdateFibonacci(s_Buffer1, sizeof(s_Buffer1) / sizeof(s_Buffer1[0]) / 2);
    }

    void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
    {
        const int ElementCount = sizeof(s_Buffer1) / sizeof(s_Buffer1[0]);
        UpdateFibonacci(s_Buffer1 + ElementCount / 2, ElementCount / 2);
    }
}

void __attribute__((noinline)) CopyMemoryWithDMAAndCalclateFibonacci()
{
    s_DMAHandle.Instance = DMA1_Stream6;
    s_DMAHandle.Init.Channel = DMA_CHANNEL_4;

    s_DMAHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    s_DMAHandle.Init.PeriphInc = DMA_PINC_DISABLE;
    s_DMAHandle.Init.MemInc = DMA_MINC_ENABLE;
    s_DMAHandle.Init.Mode = DMA_CIRCULAR;
    s_DMAHandle.Init.Priority = DMA_PRIORITY_VERY_HIGH;

    s_DMAHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    s_DMAHandle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;

    s_DMAHandle.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    s_DMAHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;

    __DMA1_CLK_ENABLE();

    HAL_StatusTypeDef status = HAL_DMA_Init(&s_DMAHandle);
    if (status != HAL_OK)
        asm("bkpt 255");

    __HAL_LINKDMA(&s_UARTHandle, hdmatx, s_DMAHandle);
    status = HAL_UART_Transmit_DMA(&s_UARTHandle, (uint8_t *)s_Buffer1, sizeof(s_Buffer1));

    if (status != HAL_OK)
        asm("bkpt 255");

    HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

    HAL_DMA_PollForTransfer(&s_DMAHandle, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
}

int main(void)
{
    HAL_Init();

    FillMemory();
    //CopyMemory();

    SetupUART();
    uint8_t test[] = "test\n";
    HAL_UART_Transmit(&s_UARTHandle, test, 4, HAL_MAX_DELAY);

    CopyMemoryWithDMAAndCalclateFibonacci();

    volatile int x = s_Buffer2[4];
}
