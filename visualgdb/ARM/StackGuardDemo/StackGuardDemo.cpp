#include <stdio.h>
#include <stdlib.h>
#include <stm32f4xx_hal.h>
#include <string.h>
#include <SysprogsProfiler.h>

void func3()
{
    char data[2048];
    memset(data, 0xA5, sizeof(data));
}

void func2()
{
    char data[2048];
    memset(data, 0xAA, sizeof(data));
    func3();
}

void func1()
{
    char data[2048];
    memset(data, 0x55, sizeof(data));
    func2();
}

volatile int g_Tick;
extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
    if (!(g_Tick++ % 10))
        func1();
}


int main(void)
{
    HAL_Init();
        InitializeInstrumentingProfiler();

    const int bufferCount = 128;
    void *buffers[bufferCount] = {
        0,
    };

    printf("Starting...\n");

    for (;;)
    {
        for (int i = 0; i < bufferCount; i++)
        {
            buffers[i] = malloc(1024);
            if (!buffers[i])
            {
                printf("Allocated %d buffers\n", i);
                break;
            }
        }
        for (int i = 0; i < bufferCount; i++)
            if (buffers[i])
            {
                free(buffers[i]);
                buffers[i] = NULL;
            }
    }
}