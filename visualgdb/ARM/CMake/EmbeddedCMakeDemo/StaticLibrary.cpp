#include <stm32f4xx_hal.h>
#include <../CMSIS_RTOS/cmsis_os.h>

extern osThreadId LEDThread1Handle, LEDThread2Handle;

/**
  * @brief  Toggle LED1
  * @param  thread not used
  * @retval None
  */
extern "C" void LED_Thread1(void const *argument)
{
    (void)argument;

    for (;;)
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
        osDelay(2000);

        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
        osThreadSuspend(LEDThread2Handle);
        osDelay(2000);

        osThreadResume(LEDThread2Handle);
    }
}