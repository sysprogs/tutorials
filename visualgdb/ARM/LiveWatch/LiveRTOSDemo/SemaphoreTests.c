#include <cmsis_os.h>
#include <stm32f4xx_hal.h>

static xSemaphoreHandle s_DemoSemaphore;

static void SenderThreadBody()
{
    for (;;)
    {
        for (int i = 0; i < 5; i++)
            xSemaphoreGive(s_DemoSemaphore);

        osDelay(10000);
    }
}

static void ReceiverThreadBody()
{
    for (;;)
    {
        xSemaphoreTake(s_DemoSemaphore, portMAX_DELAY);
        osDelay(1000);
    }
}

void StartSemaphoreTestThreads()
{
    TaskHandle_t task;

    s_DemoSemaphore = xSemaphoreCreateCounting(100, 0);

    xTaskCreate(SenderThreadBody, "Sender", 128, 0, 1, &task);
    xTaskCreate(ReceiverThreadBody, "Receiver", 128, 0, 1, &task);
}