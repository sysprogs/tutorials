/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "sys_app.h"
#include "demo_sensor.h"
#include "version.h"
#include "stm32_seq.h"
#include "stm32_timer.h"
#include "adc_if.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SENS_LED_BLINK 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

UTIL_TIMER_Object_t LedTimer;        /**<Timer which turns LEDs off*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/**
 * @brief Turn all LEDs off.
 * Used as a timer callback.
 */
static void LedsOff(void* context);

/**
 * @brief Do a short blink with a LED.
 */
static void LedBlink(void);

/**
 * @brief Callback to write sensor data right before transmission.
 */
static void WriteDataCallback(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
  SystemApp_Init();

  /*Init LEDs*/
  BSP_LED_Init(LED_RED);
  BSP_LED_Off(LED_RED);
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Off(LED_BLUE);
  UTIL_TIMER_Create(&LedTimer, 125, UTIL_TIMER_ONESHOT, LedsOff, NULL);

  /************************************************
   *
   *             Sensor
   *
   ************************************************/

  APP_LOG(TS_OFF, VLEVEL_L,
          "--------------------------\r\n"
          "    Sensor Reset\r\n"
          "     %s\r\n"
          "--------------------------\r\n", VERSION_STR);
#if 0
  /*Init meters*/
  if(Meters_Init() != 0)
  {
    APP_LOG(TS_ON, VLEVEL_L, "Cannot init sensor!\r\n");
  }
#endif
  /*Put error data to be sent before first measurement*/

  /*Init sensor stuff and start scanning for beacon*/
  SENS_Init(WriteDataCallback, LedBlink);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    UTIL_SEQ_Run( UTIL_SEQ_DEFAULT );

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * @brief Callback to write sensor data right before transmission.
 */

static DEMO_data_1_0_t data =
  {
      .temperature      = DEMO_DATA_1_0_ERROR_TEMPERATURE,
      .voltage          = DEMO_DATA_1_0_ERROR_VOLTAGE,
  };

static void WriteDataCallback(void)
{
  data.voltage=SYS_GetBatteryLevel()/50;
  data.temperature=(SYS_GetTemperatureLevel()*100)>>8;

  SENS_WriteSensorData(&data);
}

/**
 * @brief Turn all LEDs off.
 * Used as a timer callback.
 */
static void LedsOff(void* context)
{
  UNUSED(context);

#if SENS_LED_BLINK != 0
  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_BLUE);
#endif /*SENS_LED_BLINK*/
}

/**
 * @brief Do a short blink with a LED.
 */
static void LedBlink(void)
{
#if SENS_LED_BLINK != 0
  if(UTIL_TIMER_IsRunning(&LedTimer))    /*Timer is already running, LED flashing is too fast*/
  {
    switch(SENS_GetState())
    {
      case SENS_STATE_Connect: BSP_LED_Toggle(LED_GREEN); BSP_LED_Off(LED_RED);   BSP_LED_Off(LED_BLUE);  break;
      case SENS_STATE_Lost:   BSP_LED_Toggle(LED_RED);   BSP_LED_Off(LED_GREEN); BSP_LED_Off(LED_BLUE);  break;
      case SENS_STATE_Scan:   BSP_LED_Toggle(LED_RED);   BSP_LED_Off(LED_GREEN); BSP_LED_Off(LED_BLUE);  break;
      case SENS_STATE_Sync:   BSP_LED_Toggle(LED_BLUE);  BSP_LED_Off(LED_RED);   BSP_LED_Off(LED_GREEN); break;
      default: break;
    }
    UTIL_TIMER_Stop(&LedTimer);
    UTIL_TIMER_Start(&LedTimer);
  }
  else
  {
    switch(SENS_GetState())
    {
      case SENS_STATE_Connect: BSP_LED_On(LED_GREEN); break;
      case SENS_STATE_Lost: BSP_LED_On(LED_RED); break;
      case SENS_STATE_Scan: BSP_LED_On(LED_RED); break;
      case SENS_STATE_Sync: BSP_LED_On(LED_BLUE); break;
      default: break;
    }
    UTIL_TIMER_Start(&LedTimer);
  }
#endif /*SENS_LED_BLINK*/
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while (1)
  {
  }

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
