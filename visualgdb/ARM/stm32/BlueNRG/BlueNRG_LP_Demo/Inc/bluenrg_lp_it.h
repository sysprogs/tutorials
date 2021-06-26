/**
  ******************************************************************************
  * @file    bluenrg_lp_it.h
  * @author  AMS RF Application Team
  * @version V1.1.0
  * @date    22-April-2021
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BlueNRGLP_IT_H
#define BlueNRGLP_IT_H

/* Includes ------------------------------------------------------------------*/
#include "BlueNRG_LPx.h"

/* Exported defines ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void HardFault_IRQHandler(void);
void NMI_IRQHandler(void); 
void SysTick_IRQHandler(void);


#endif /* BlueNRGLP_IT_H */

/******************* (C) COPYRIGHT 2021 STMicroelectronics *****END OF FILE****/
