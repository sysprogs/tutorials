/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_dpm_user.c
  * @author  MCD Application Team
  * @brief   USBPD DPM user code
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#define USBPD_DPM_USER_C
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbpd_core.h"
#include "usbpd_dpm_user.h"
#if defined(_TRACE)
#include "usbpd_trace.h"
#include "string.h"
#include "stdio.h"
#endif /* _TRACE */
/* USER CODE BEGIN Includes */
#include "usbpd_dpm_core.h"
#include "usbpd_usb_if.h"
/* USER CODE END Includes */

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @addtogroup STM32_USBPD_APPLICATION_DPM_USER
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN Private_Typedef */

/* USER CODE END Private_Typedef */

/* Private define ------------------------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_DEFINES USBPD USER Private Defines
  * @{
  */
/* USER CODE BEGIN Private_Define */

/* USER CODE END Private_Define */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_MACROS USBPD USER Private Macros
  * @{
  */
#if defined(_TRACE)
#define DPM_USER_DEBUG_TRACE_SIZE       50u
#define DPM_USER_DEBUG_TRACE(_PORT_, ...)  do {                                                                \
      char _str[DPM_USER_DEBUG_TRACE_SIZE];                                                                    \
      uint8_t _size = snprintf(_str, DPM_USER_DEBUG_TRACE_SIZE, __VA_ARGS__);                                  \
      if (_size < DPM_USER_DEBUG_TRACE_SIZE)                                                                   \
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, strlen(_str));                \
      else                                                                                                     \
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, DPM_USER_DEBUG_TRACE_SIZE);   \
  } while(0)

#else
#define DPM_USER_DEBUG_TRACE(_PORT_, ...)
#endif /* _TRACE */
/* USER CODE BEGIN Private_Macro */

/* USER CODE END Private_Macro */
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_VARIABLES USBPD USER Private Variables
  * @{
  */

/* USER CODE BEGIN Private_Variables */

/* USER CODE END Private_Variables */
/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_FUNCTIONS USBPD USER Private Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */

/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS_Prototypes */
/**
  * @}
  */

/* Exported functions ------- ------------------------------------------------*/
/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS USBPD USER Exported Functions
  * @{
  */
/* USER CODE BEGIN USBPD_USER_EXPORTED_FUNCTIONS */

/* USER CODE END USBPD_USER_EXPORTED_FUNCTIONS */

/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP1 USBPD USER Exported Functions called by DPM CORE
  * @{
  */
/* USER CODE BEGIN USBPD_USER_EXPORTED_FUNCTIONS_GROUP1 */

/* USER CODE END USBPD_USER_EXPORTED_FUNCTIONS_GROUP1 */

/**
  * @brief  Initialize DPM (port power role, PWR_IF, CAD and PE Init procedures)
  * @retval USBPD Status
  */

/**
  * @brief  User delay implementation which is OS dependent
  * @param  Time time in ms
  * @retval None
  */
void USBPD_DPM_WaitForTime(uint32_t Time)
{
  HAL_Delay(Time);
}

/**
  * @brief  User processing time, it is recommended to avoid blocking task for long time
  * @param  argument  DPM User event
  * @retval None
  */
void USBPD_DPM_UserExecute(void const *argument)
{
/* USER CODE BEGIN USBPD_DPM_UserExecute */

/* USER CODE END USBPD_DPM_UserExecute */
}

/**
  * @brief  UserCableDetection reporting events on a specified port from CAD layer.
  * @param  PortNum The handle of the port
  * @param  State CAD state
  * @retval None
  */
void USBPD_DPM_UserCableDetection(uint8_t PortNum, USBPD_CAD_EVENT State)
{
/* USER CODE BEGIN USBPD_DPM_UserCableDetection */
DPM_USER_DEBUG_TRACE(PortNum, "ADVICE: update USBPD_DPM_UserCableDetection");
/* USER CODE END USBPD_DPM_UserCableDetection */
}

/**
  * @}
  */

/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP2 USBPD USER Exported Callbacks functions called by PE
  * @{
  */

/**
  * @brief  Callback function called by PE to inform DPM about PE event.
  * @param  PortNum The current port number
  * @param  EventVal @ref USBPD_NotifyEventValue_TypeDef
  * @retval None
  */
void USBPD_DPM_Notification(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal)
{
/* USER CODE BEGIN USBPD_DPM_Notification */
  /* Manage event notified by the stack? */
  switch(EventVal)
  {
    case USBPD_NOTIFY_POWER_EXPLICIT_CONTRACT :
      break;
    case USBPD_NOTIFY_REQUEST_ACCEPTED:
      break;
    case USBPD_NOTIFY_REQUEST_REJECTED:
    case USBPD_NOTIFY_REQUEST_WAIT:
      break;
    case USBPD_NOTIFY_POWER_SWAP_TO_SNK_DONE:
      break;
    case USBPD_NOTIFY_STATE_SNK_READY:
      break;
    case USBPD_NOTIFY_HARDRESET_RX:
    case USBPD_NOTIFY_HARDRESET_TX:
      break;
    case USBPD_NOTIFY_STATE_SRC_DISABLED:
      break;
    case USBPD_NOTIFY_ALERT_RECEIVED :
      break;
    case USBPD_NOTIFY_CABLERESET_REQUESTED :
      break;
    case USBPD_NOTIFY_MSG_NOT_SUPPORTED :
      break;
    case USBPD_NOTIFY_PE_DISABLED :
      break;
    case USBPD_NOTIFY_USBSTACK_START:
      USBPD_USBIF_DeviceStart(PortNum);
      break;
    case USBPD_NOTIFY_USBSTACK_STOP:
      USBPD_USBIF_DeviceStop(PortNum);
      break;
    case USBPD_NOTIFY_DATAROLESWAP_DFP :
      break;
    case USBPD_NOTIFY_DATAROLESWAP_UFP :
      break;
    default:
      DPM_USER_DEBUG_TRACE(PortNum, "ADVICE: USBPD_DPM_Notification:%d", EventVal);
      break;
  }
/* USER CODE END USBPD_DPM_Notification */
}

/** @addtogroup USBPD_USER_PRIVATE_FUNCTIONS
  * @{
  */

/* USER CODE BEGIN USBPD_USER_PRIVATE_FUNCTIONS */

/* USER CODE END USBPD_USER_PRIVATE_FUNCTIONS */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
