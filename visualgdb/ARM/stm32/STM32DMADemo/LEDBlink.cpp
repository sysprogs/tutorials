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

void __attribute__((noinline)) CalculateFibonacci()
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

void __attribute__((noinline)) CopyMemoryWithDMAAndCalclateFibonacci()
{
    s_DMAHandle.Instance = DMA2_Stream0;
    s_DMAHandle.Init.Channel = DMA_CHANNEL_0;

    s_DMAHandle.Init.Direction = DMA_MEMORY_TO_MEMORY;
    s_DMAHandle.Init.PeriphInc = DMA_PINC_ENABLE;
    s_DMAHandle.Init.MemInc = DMA_MINC_ENABLE;
    s_DMAHandle.Init.Mode = DMA_NORMAL;
    s_DMAHandle.Init.Priority = DMA_PRIORITY_VERY_HIGH;

    s_DMAHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    s_DMAHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;

    s_DMAHandle.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    s_DMAHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;

    __DMA2_CLK_ENABLE();

    HAL_StatusTypeDef status = HAL_DMA_Init(&s_DMAHandle);
    if (status != HAL_OK)
        asm("bkpt 255");

    status = HAL_DMA_Start(&s_DMAHandle, (uint32_t)s_Buffer1, (uint32_t)s_Buffer2, sizeof(s_Buffer1) / 4);
    if (status != HAL_OK)
        asm("bkpt 255");

    CalculateFibonacci();

    HAL_DMA_PollForTransfer(&s_DMAHandle, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
}

int main(void)
{
    HAL_Init();

    FillMemory();
    //CopyMemory();
    CopyMemoryWithDMAAndCalclateFibonacci();

    volatile int x = s_Buffer2[4];
}
