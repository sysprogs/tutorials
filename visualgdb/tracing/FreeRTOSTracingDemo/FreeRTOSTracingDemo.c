/**
  ******************************************************************************
  * @file    FreeRTOS/FreeRTOS_ThreadCreation/Src/main.c
  * @author  MCD Application Team
  * @version V1.2.2
  * @date    25-May-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stm32f4xx_hal.h>
#include <../CMSIS_RTOS/cmsis_os.h>
#include <stdio.h>
#include "UARTFunctions.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SendingThread(void const *argument);
static void ProcessingThread(void const *argument);

/* Private functions ---------------------------------------------------------*/

void USART2_IRQHandler()
{
    HAL_UART_IRQHandler(&UartHandle);
}

static QueueHandle_t s_RXQueue, s_TXQueue;
static SemaphoreHandle_t s_TXReadySemaphore;
static uint8_t s_RxCharacter, s_TxCharacter;
static int s_SendActive;



void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    BaseType_t xHigherPriorityTaskWoken;
    xSemaphoreGiveFromISR(s_TXReadySemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xQueueSendFromISR(s_RXQueue, &s_RxCharacter, &xHigherPriorityTaskWoken);
    HAL_UART_Receive_IT(&UartHandle, &s_RxCharacter, 1);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
 
    
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
	HAL_Init();  
    
    SystemClock_Config();
    UART_Init();
	
	__GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13;

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

    osThreadDef(Process, ProcessingThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
    osThreadDef(Sender, SendingThread, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE);
  
    s_RXQueue = xQueueCreate(128, 1);
    s_TXQueue = xQueueCreate(128, 1);
    s_TXReadySemaphore = xSemaphoreCreateBinary();
  
    osThreadCreate(osThread(Process), NULL);
    osThreadCreate(osThread(Sender), NULL);
  
    NVIC_SetPriority(USART2_IRQn, configLIBRARY_LOWEST_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(USART2_IRQn);
    
	/* Start scheduler */
	osKernelStart();

	  /* We should never get here as control is now taken by the scheduler */
	for (;;)
		;
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	osSystickHandler();
}


static void ProcessingThread(void const *argument)
{
    HAL_UART_Receive_IT(&UartHandle, &s_RxCharacter, 1);
    
	for (int i = 0;;i++)
	{
    	uint8_t ch;
    	static char reply[64];
    	xQueueReceive(s_RXQueue, &ch, portMAX_DELAY);
    	sprintf(reply, "You typed '%c'\r\n", ch);
    	
    	for (char *p = reply; *p; p++)
    	{
        	xQueueSend(s_TXQueue, p, portMAX_DELAY);
    	}
    	
    	if (i > 2)
        	asm("bkpt 255");
	}
}

static void SendingThread(void const *argument)
{
    for (;;)
    {
        uint8_t ch;
        xQueueReceive(s_TXQueue, &ch, portMAX_DELAY);
        
        HAL_UART_Transmit_IT(&UartHandle, &ch, 1);
        xSemaphoreTake(s_TXReadySemaphore, portMAX_DELAY);
    }
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
	while (1)
	{
	}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
