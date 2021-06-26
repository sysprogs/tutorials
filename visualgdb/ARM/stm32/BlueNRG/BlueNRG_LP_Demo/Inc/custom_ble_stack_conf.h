/**
  ******************************************************************************
  * @file    custom_ble_stack_config.h
  * @author  AMS - RF Application team
  * @version V1.0.0
  * @date    07 April 2020
  * @brief   Custom BLE stack modular configuration options header file
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
  * <h2><center>&copy; COPYRIGHT STMicroelectronics</center></h2>
  ******************************************************************************
* \section BLE_Config BLE stack configuration options  

    - The BLE stack v3.1 supports the following outstanding features:
      -- Controller Privacy
      -- LE Secure Connections
      -- Controller Master
      -- Controller Data Length Extension
      -- LE 2M PHY
      -- LE Coded PHY
      -- Extended Advertising
      -- Periodic Advertising and Synchronizer
      -- L2CAP Connection Oriented Channels
      -- Constant Tone Extension (not for BlueNRG-LP; reserved for future use)
      -- LE Power Control
    
    - In order to configure the BLE stack v3.1 or later the following options are available: 

        - CONTROLLER_MASTER_ENABLED                 : Master role                               (1:ENABLED; 0: DISABLED) 
        - CONTROLLER_PRIVACY_ENABLED                : Controller Privacy feature                (1:ENABLED; 0: DISABLED) 
        - SECURE_CONNECTIONS_ENABLED                : LE Secure Connections feature             (1:ENABLED; 0: DISABLED) 
        - CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED  : Data Length Extension feature             (1:ENABLED; 0: DISABLED) 
        - CONTROLLER_2M_CODED_PHY_ENABLED           : LE 2M + Coded PHYs                        (1:ENABLED; 0: DISABLED) 
        - CONTROLLER_EXT_ADV_SCAN_ENABLED           : Extended Advertising and Scanning feature (1:ENABLED; 0: DISABLED) 
        - L2CAP_COS_ENABLED                         : L2CAP COS feature                         (1:ENABLED; 0: DISABLED) 
        - CONTROLLER_PERIODIC_ADV_ENABLED           : Periodic Advertising and Synchronizer     (1:ENABLED; 0: DISABLED) 
        - CONTROLLER_CTE_ENABLED                    : Constant Tone Extension                   (1:ENABLED; 0: DISABLED) 
        - CONTROLLER_POWER_CONTROL_ENABLED          : LE Power Control                          (1:ENABLED; 0: DISABLED) 
**/

#ifndef _CUSTOM_BLE_STACK_CONF_H_
#define _CUSTOM_BLE_STACK_CONF_H_

/* --------------------- BLE stack configuration options --------------------------------------------------- */
/* This specific set of modular configuration options is used for configuring the Bluetooth Low Energy stack 
   ONLY when BLE_STACK_CUSTOM_CONF is defined on user application prepropoessor options */

#define CONTROLLER_MASTER_ENABLED                 (0U) 
#define CONTROLLER_PRIVACY_ENABLED                (0U) 
#define SECURE_CONNECTIONS_ENABLED                (0U) 
#define CONTROLLER_DATA_LENGTH_EXTENSION_ENABLED  (0U) 
#define CONTROLLER_2M_CODED_PHY_ENABLED           (0U)

#if defined(EXTENDED_ADV) || defined(PERIODIC_ADV)
/* ExtendedAdv demo configuration or PeriodicAdv demo configuration */
#define CONTROLLER_EXT_ADV_SCAN_ENABLED           (1U) 
#else
#define CONTROLLER_EXT_ADV_SCAN_ENABLED           (0U) 
#endif 

#define L2CAP_COS_ENABLED                         (0U) 

#if defined(PERIODIC_ADV) 
/* PeriodicAdv demo configuration */
#define CONTROLLER_PERIODIC_ADV_ENABLED           (1U) 
#else
#define CONTROLLER_PERIODIC_ADV_ENABLED           (0U) 
#endif 

#define CONTROLLER_CTE_ENABLED                    (0U)
#define CONTROLLER_POWER_CONTROL_ENABLED          (0U) 

#endif // _CUSTOM_BLE_STACK_CONF_H_
