
/******************** (C) COPYRIGHT 2021 STMicroelectronics ********************
* File Name          : BLE_Beacon_main.c
* Author             : RF Application Team
* Version            : 2.0.0
* Date               : 04-January-2021
* Description        : Code demonstrating the Bluetooth LE Beacon application
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/**
 * @file BLE_Beacon_main.c
 * @brief This is a Bluetooth LE beacon demo that shows how to configure a BlueNRG-LP device 
 * in order to advertise specific manufacturing data and allow another Bluetooth LE device to
 * know if it is in the range of the BlueNRG-LP beacon device. 
 * It  provides a reference example about how using the 
 * Bluetooth LE Over-The-Air (OTA) Service Manager firmware upgrade capability.
 * This example also allows to use the extended advertising feature in order to configure beacon also on a secondary advertising channel. 
 * Further a specific configuration (PeriodicAdv) allows to configure a beacon with periodic advertising.

* \section KEIL_project KEIL project
  To use the project with KEIL uVision 5 for ARM, please follow the instructions below:
  -# Open the KEIL uVision 5 for ARM and select Project->Open Project menu. 
  -# Open the KEIL project
     <tt>C:\\Users\\{username}\\ST\\BlueNRG-LP DK x.x.x\\Projects\\BLE_Examples\\BLE_Beacon\\MDK-ARM\\{STEVAL-IDB011V1}\\BLE_Beacon.uvprojx</tt> 
  -# Select desired configuration to build
  -# Select Project->Rebuild all target files. This will recompile and link the entire application
  -# To download the binary image, please connect an USB cable in your board (CMSIS-DAP upgrade).
  -# Select Debug->Start/Stop Debug Session  to download the related binary image.
  -# Alternatively, open the Flasher utility and download the built binary image.

* \section IAR_project IAR project
  To use the project with IAR Embedded Workbench for ARM, please follow the instructions below:
  -# Open the Embedded Workbench for ARM and select File->Open->Workspace menu. 
  -# Open the IAR project
     <tt>C:\\Users\\{username}\\ST\\BlueNRG-LP DK x.x.x\\Projects\\BLE_Examples\\BLE_Beacon\\EWARM\\{STEVAL-IDB011V1}\\BLE_Beacon.eww</tt> 
  -# Select desired configuration to build
  -# Select Project->Rebuild All. This will recompile and link the entire application
  -# To download the binary image, please connect an USB cable in your board (CMSIS-DAP upgrade).
  -# Select Project->Download and Debug to download the related binary image.
  -# Alternatively, open the Flasher utility and download the built binary image.

* \section WISE_project WiSE-Studio project
  To use the project with WiSE-Studio IDE (GCC toolchain), please follow the instructions below:
  -# Open the WiSE-Studio IDE
  -# Select File, Import, Existing Projects into Workspace
     <tt>C:\\Users\\{username}\\ST\\BlueNRG-LP DK x.x.x\\Project\\BLE_Examples\\BLE_Beacon\\WiSE-Studio\\{STEVAL-IDB011V1}</tt> 
  -# Select desired configuration to build
  -# Select Project->Build Project. This will recompile and link the entire application
  -# To download the binary image, please connect an USB cable in your board (CMSIS-DAP upgrade).
  -# Select Run->Run/Debug to download the related binary image.
  -# Alternatively, open the Flasher utility and download the built binary image.

* \subsection Project_configurations Project configurations
- \c ExtendedAdv - Extended Advertising configuration
- \c PeriodicAdv - Periodic Advertising configuration
- \c Release - Release configuration
- \c Use_OTA_ServiceManager - OTA Service Manager configuration

     
* \section Board_supported Boards supported
- \c STEVAL-IDB011V1


 * \section Power_settings Power configuration settings
@table

==========================================================================================================
|                                         STEVAL-IDB01xV1                                                |
----------------------------------------------------------------------------------------------------------
| Jumper name | Description                                                                |
| JP2         |                                                                            |
----------------------------------------------------------------------------------------------------------
| USB         | USB supply power                                                            |
| BAT         | The supply voltage must be provided through battery pins.                   |


@endtable 

* \section Jumper_settings Jumper settings
@table

========================================================================================================================================================================================
|                                                                             STEVAL-IDB01xV1                                                                                          |
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
| Jumper name |                                                                Description                                                                                             |
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------          
| JP1         | It provides the voltage to the BlueNRG-LP circuit. It must be fitted. It can be used for current measurements of the BlueNRG-LP device.                                |          
| JP2         | It is a switch between two power domains. BAT position: to provide power from battery holder; USB position: to provide power from USB connector.                       |
| JP3         | It connects the BLE_SWCLK pin of the BlueNRG-LP with the SWCLK pin of the USB_CMSISDAP. It must be fitted.                                                             |          
| JP4         | It connects the BLE_SWDIO pin of the BlueNRG-LP with the SWDIO pin of the USB_CMSISDAP. It must be fitted.                                                             |
| JP5         | It connects the BLE_RSTN pin of the BlueNRG-LP with the rest of the board (the USB_CMSISDAP and RESET push button). It must be fitted.                                 |


@endtable 

* \section Pin_settings Pin settings
@table
|            |       Release      |  Use_OTA_ServiceManager  |     ExtendedAdv    |     PeriodicAdv    |
--------------------------------------------------------------------------------------------------------
|  PIN name  |   STEVAL-IDB011V1  |      STEVAL-IDB011V1     |   STEVAL-IDB011V1  |   STEVAL-IDB011V1  |
--------------------------------------------------------------------------------------------------------
|     A1     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A11    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A12    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A13    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A14    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A15    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A4     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A5     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A6     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A7     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A8     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A8     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     A9     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     B0     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     B14    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     B2     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     B3     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     B4     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     B5     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     B7     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     B8     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     B9     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     GND    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     RST    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|    VBAT    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |

@endtable 

* \section Serial_IO Serial I/O
@table
| Parameter name  | Value            | Unit      |
----------------------------------------------------
| Baudrate        | 115200 [default] | bit/sec   |
| Data bits       | 8                | bit       |
| Parity          | None             | bit       |
| Stop bits       | 1                | bit       |
@endtable

* \section LEDs_description LEDs description
@table
|            |       Release      |  Use_OTA_ServiceManager  |     ExtendedAdv    |     PeriodicAdv    |
--------------------------------------------------------------------------------------------------------
|  LED name  |   STEVAL-IDB011V1  |      STEVAL-IDB011V1     |   STEVAL-IDB011V1  |   STEVAL-IDB011V1  |
--------------------------------------------------------------------------------------------------------
|     DL1    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     DL2    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     DL3    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     DL4    |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|     U5     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |

@endtable


* \section Buttons_description Buttons description
@table
|                |       Release      |  Use_OTA_ServiceManager  |     ExtendedAdv    |     PeriodicAdv    |
------------------------------------------------------------------------------------------------------------
|   BUTTON name  |   STEVAL-IDB011V1  |      STEVAL-IDB011V1     |   STEVAL-IDB011V1  |   STEVAL-IDB011V1  |
------------------------------------------------------------------------------------------------------------
|      PUSH1     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|      PUSH2     |      Not Used      |         Not Used         |      Not Used      |      Not Used      |
|      RESET     |  Reset BlueNRG-LP  |     Reset BlueNRG-LP     |  Reset BlueNRG-LP  |  Reset BlueNRG-LP  |

@endtable

* \section Usage Usage

The Beacon demo configures a BlueNRG-LP device in advertising mode (non-connectable mode) with specific manufacturing data.
It transmits advertisement packets at regular intervals which contain the following manufacturing data:
@table   
------------------------------------------------------------------------------------------------------------------------
| Data field              | Description                       | Notes                                                  |
------------------------------------------------------------------------------------------------------------------------
| Company identifier code | SIG company identifier (1)        | Default is 0x0030 (STMicroelectronics)                 |
| ID                      | Beacon ID                         | Fixed value                                            |
| Length                  | Length of the remaining payload   | NA                                                     |
| Location UUID           | Beacons UUID                      | It is used to distinguish specific beacons from others |
| Major number            | Identifier for a group of beacons | It is used to group a related set of beacons           |                                              
| Minor number            | Identifier for a single beacon    | It is used to identify a single beacon                 |                                       
| Tx Power                | 2's complement of the Tx power    | It is used to establish how far you are from device    |                                       
@endtable

 - (1): SIG company identifiers are available on https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers/
 - NA : Not Applicable;

This example also allows to use the extended advertising feature in order to configure beacon also on a secondary advertising channel. 
Refer to the ExtendedAdv project configuration.
User can also configure a periodic advertising of a beacon payload using the related PeriodicAdv project configuration.

NOTEs:
     - OTA Service Manager support requires to build application by enabling CONFIG_OTA_USE_SERVICE_MANAGER (preprocessor, linker) options and through files: OTA_btl.[ch] (refer to Release_with_OTA_ServiceManager IAR workspace).

**/
   
/** @addtogroup BlueNRGLP_demonstrations_applications
 *  BlueNRG-LP Beacon demo \see BLE_Beacon_main.c for documentation.
 *
 *@{
 */

/** @} */
/** \cond DOXYGEN_SHOULD_SKIP_THIS
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "bluenrg_lp_it.h"
#include "ble_const.h"
#include "bluenrg_lp_stack.h"
#include "Beacon_config.h"
#include "OTA_btl.h"
#include "rf_driver_hal_power_manager.h"
#include "rf_driver_hal_vtimer.h"
#include "bluenrg_lp_evb_com.h"
#include "bleplat.h"
#include "nvm_db.h"
#include "pka_manager.h"
#include "rng_manager.h"
#include "aes_manager.h"
#include "ble_controller.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BLE_BEACON_VERSION_STRING "2.0"

/* Set to 1 to enable the name AD data in extended advertising events (if
  extended advertising events are used).  */
#define DEVICE_NAME_IN_ADV 0

/* PHY used in extended advertising events. One between: LE_1M_PHY,
  LE_2M_PHY and LE_CODED_PHY.  */
#define EXT_ADV_PHY LE_1M_PHY


/* Private macro -------------------------------------------------------------*/
#ifdef DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif
/* Private variables ---------------------------------------------------------*/
  /* Set AD Type Flags at beginning on Advertising packet  */
static uint8_t adv_data[] = {
  /* Advertising data: Flags AD Type */
  0x02, 
  0x01, 
  0x06, 
  /* Advertising data: manufacturer specific data */
  26, //len
  AD_TYPE_MANUFACTURER_SPECIFIC_DATA,  //manufacturer type
  0x4C, 0x00, //Company identifier code
  0x02,       // ID
  0x15,       //Length of the remaining payload
  0xE2, 0x0A, 0x39, 0xF4, 0x73, 0xF5, 0x4B, 0xC4, //Location UUID
  0xA1, 0x2F, 0x17, 0xD1, 0xAD, 0x07, 0xA9, 0x61,
  0x00, 0x05, // Major number 
  0x00, 0x07, // Minor number 
  (uint8_t)-56,         // Tx power measured at 1 m of distance (in dBm)
#if DEVICE_NAME_IN_ADV
  15,       // Length of following AD data
  0x09,'E','x','t','e','n','d','e','d','B','e','a','c','o','n'
#endif
};

NO_INIT(uint32_t dyn_alloc_a[DYNAMIC_MEMORY_SIZE>>2]);

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void ModulesInit(void)
{
  uint8_t ret;
  BLE_STACK_InitTypeDef BLE_STACK_InitParams = BLE_STACK_INIT_PARAMETERS;
  
  LL_AHB_EnableClock(LL_AHB_PERIPH_PKA|LL_AHB_PERIPH_RNG);
  
  /* BlueNRG-LP stack init */
  ret = BLE_STACK_Init(&BLE_STACK_InitParams);
  if (ret != BLE_STATUS_SUCCESS) {
    printf("Error in BLE_STACK_Init() 0x%02x\r\n", ret);
    while(1);
  }
  
  BLECNTR_InitGlobal();
  
  HAL_VTIMER_InitType VTIMER_InitStruct = {HS_STARTUP_TIME, INITIAL_CALIBRATION, CALIBRATION_INTERVAL};
  HAL_VTIMER_Init(&VTIMER_InitStruct);
  
  BLEPLAT_Init();  
  if (PKAMGR_Init() == PKAMGR_ERROR)
  {
      while(1);
  }
  if (RNGMGR_Init() != RNGMGR_SUCCESS)
  {
      while(1);
  }
  
    /* Init the AES block */
  AESMGR_Init();
  
}

void ModulesTick(void)
{
  /* Timer tick */
  HAL_VTIMER_Tick();
  
  /* Bluetooth stack tick */
  BLE_STACK_Tick();
  
  /* NVM manager tick */
  NVMDB_Tick();
}

void Device_Init(void)
{
  uint8_t ret;
  uint16_t service_handle;
  uint16_t dev_name_char_handle;
  uint16_t appearance_char_handle;
  uint8_t address[CONFIG_DATA_PUBADDR_LEN] = {0x66,0x77,0x88,0xE1,0x80,0x02};
  
  aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, address);
 
  /* Set the TX Power to 0 dBm */
  ret = aci_hal_set_tx_power_level(0,25);
  if(ret != 0) {
    PRINTF ("Error in aci_hal_set_tx_power_level() 0x%04xr\n", ret);
    while(1);
  }

  /* Init the GATT */
  ret = aci_gatt_srv_init();
  if (ret != 0) 
  {
    PRINTF ("Error in aci_gatt_srv_init() 0x%04xr\n", ret);
  }
  else
  {
    PRINTF ("aci_gatt_srv_init() --> SUCCESS\r\n");
  }
  
  /* Init the GAP */
  ret = aci_gap_init(GAP_BROADCASTER_ROLE|GAP_PERIPHERAL_ROLE, 0x00, 0x08, PUBLIC_ADDR, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  if (ret != 0)
  {
    PRINTF ("Error in aci_gap_init() 0x%04x\r\n", ret);
  }
  else
  {
    PRINTF ("aci_gap_init() --> SUCCESS\r\n");
  }
}

/**
* @brief  Start beaconing
* @param  None 
* @retval None
*/
static void Start_Beaconing(void)
{  
  uint8_t ret;
  Advertising_Set_Parameters_t Advertising_Set_Parameters[2];
  uint8_t adv_sets = 0;
  
#ifdef LEGACY_ADV
   
  /* Set advertising configuration for legacy advertising. */  
  ret = aci_gap_set_advertising_configuration(0, GAP_MODE_GENERAL_DISCOVERABLE,
                                              ADV_PROP_LEGACY,
                                              160, 160,
                                              ADV_CH_ALL,
                                              0,NULL, /* No peer address */
                                              ADV_NO_WHITE_LIST_USE,
                                              0, /* 0 dBm */
                                              LE_1M_PHY, /* Primary advertising PHY */
                                              0, /* 0 skips */
                                              LE_1M_PHY, /* Secondary advertising PHY. Not used with legacy advertising. */
                                              0, /* SID */
                                              0 /* No scan request notifications */);
  if (ret != BLE_STATUS_SUCCESS)
  {
    PRINTF("Error in aci_gap_set_advertising_configuration() 0x%02x\r\n", ret);
    return;
  }
  
  ret = aci_gap_set_advertising_data(0, ADV_COMPLETE_DATA, 30, adv_data);
  if (ret != BLE_STATUS_SUCCESS)
  {
    PRINTF("Error in aci_gap_set_advertising_data() 0x%02x\r\n", ret);
    return;
  }
  
  printf("Legacy advertising configured\n");
  
  Advertising_Set_Parameters[adv_sets].Advertising_Handle = 0;
  Advertising_Set_Parameters[adv_sets].Duration = 0;
  Advertising_Set_Parameters[adv_sets].Max_Extended_Advertising_Events = 0;
  
  adv_sets++;
  
#endif
  
#ifdef EXTENDED_ADV
  /* Set advertising configuration for extended advertising. */  
  ret = aci_gap_set_advertising_configuration(1, GAP_MODE_GENERAL_DISCOVERABLE,
                                              ADV_PROP_NONE,
                                              160, 160,
                                              ADV_CH_ALL,
                                              0,NULL, /* No peer address */
                                              ADV_NO_WHITE_LIST_USE,
                                              0, /* 0 dBm */
                                              (EXT_ADV_PHY==LE_2M_PHY)?LE_1M_PHY:EXT_ADV_PHY, /* Primary advertising PHY */
                                              0, /* 0 skips */
                                              EXT_ADV_PHY, /* Secondary advertising PHY */
                                              0, /* SID */
                                              0 /* No scan request notifications */);
  if (ret != BLE_STATUS_SUCCESS)
  {
    PRINTF("Error in aci_gap_set_advertising_configuration() 0x%02x\r\n", ret);
    return;
  }
  
  ret = aci_gap_set_advertising_data(1, ADV_COMPLETE_DATA, sizeof(adv_data), adv_data);
  if (ret != BLE_STATUS_SUCCESS)
  {
    PRINTF("Error in aci_gap_set_advertising_data() 0x%02x\r\n", ret);
    return;
  }
  
  printf("Extended advertising configured\n");
  
  Advertising_Set_Parameters[adv_sets].Advertising_Handle = 1;
  Advertising_Set_Parameters[adv_sets].Duration = 0;
  Advertising_Set_Parameters[adv_sets].Max_Extended_Advertising_Events = 0;
  
  adv_sets++;
  
#endif /* EXTENDED_ADV */
  
#ifdef PERIODIC_ADV
  /* Configure periodic advertising */
  ret = aci_gap_set_periodic_advertising_configuration(1, 160, 160,  0 );                              
  if (ret != BLE_STATUS_SUCCESS)
  {
    PRINTF("Error in aci_gap_set_periodic_advertising_configuration() 0x%02x\r\n", ret);
    return;
  }    
      
  /* Set periodic advertising data: beacon payload */
  ret = aci_gap_set_periodic_advertising_data(1, 30, adv_data);
  if (ret != BLE_STATUS_SUCCESS)
  {
    PRINTF("Error in aci_gap_set_periodic_advertising_data() 0x%02x\r\n", ret);
    return;
  }
  
  /* Enable periodic advertising */
  ret = aci_gap_set_periodic_advertising_enable(ENABLE, 1);
  if (ret != BLE_STATUS_SUCCESS)
  {
    PRINTF("Error in aci_gap_set_periodic_advertising_enable() 0x%02x\r\n", ret);
    return;
  }
            
  printf("Periodic advertising configured\n");

#endif /* PERIODIC_ADV */  
  
   /* Enable advertising */
  ret = aci_gap_set_advertising_enable(ENABLE, adv_sets, Advertising_Set_Parameters);
  if (ret != BLE_STATUS_SUCCESS)
  {
    PRINTF ("Error in aci_gap_set_advertising_enable() 0x%02x\r\n", ret);
    return;
  }
  
  printf("Advertising started\n");

}

int main(void) 
{
  WakeupSourceConfig_TypeDef wakeupIO;
  PowerSaveLevels stopLevel;

  /* System initialization function */
  if (SystemInit(SYSCLK_64M, BLE_SYSCLK_32M) != SUCCESS) 
  {
    /* Error during system clock configuration take appropriate action */
    while(1);
  }
  /* Configure IOs for power save modes */
  BSP_IO_Init();
  /* Init the UART peripheral */
  BSP_COM_Init(NULL);
  /* Init BLE stack, HAL virtual timer and NVM modules */
  ModulesInit(); 
  /* Init the Bluetooth LE stack layesrs */
  Device_Init();
#if CONFIG_OTA_USE_SERVICE_MANAGER
  /* Initialize the button */
  BSP_PB_Init(USER_BUTTON, BUTTON_MODE_GPIO); 
#endif /* CONFIG_OTA_USE_SERVICE_MANAGER */
  
  printf("BlueNRG-LP BLE Beacon Application (version: %s)\r\n", BLE_BEACON_VERSION_STRING); 
 
  /* Start beaconing */
  Start_Beaconing();
  
  /* No Wakeup Source needed */
  wakeupIO.IO_Mask_High_polarity = 0;
  wakeupIO.IO_Mask_Low_polarity = 0;
  wakeupIO.RTC_enable = 0;          
  wakeupIO.LPU_enable = 0;
 
  while(1) 
  {
    ModulesTick();
    
    /* Power Save Request */
    //HAL_PWR_MNGR_Request(POWER_SAVE_LEVEL_STOP_NOTIMER, wakeupIO, &stopLevel); 
     
#if CONFIG_OTA_USE_SERVICE_MANAGER
    if (BSP_PB_GetState(USER_BUTTON) == SET)
    {
      OTA_Jump_To_Service_Manager_Application();
    }
#endif /* CONFIG_OTA_USE_SERVICE_MANAGER */
  }
}

/****************** BlueNRG-LP Power Management Callback ********************************/

PowerSaveLevels App_PowerSaveLevel_Check(PowerSaveLevels level)
{
  if(BSP_COM_TxFifoNotEmpty() || BSP_COM_UARTBusy())
    return POWER_SAVE_LEVEL_RUNNING;
  
  return POWER_SAVE_LEVEL_STOP_NOTIMER;
}

/* Hardware Error event. 
   This event is used to notify the Host that a hardware failure has occurred in the Controller. 
   Hardware_Code Values:
   - 0x01: Radio state error
   - 0x02: Timer overrun error
   - 0x03: Internal queue overflow error
   - 0x04: Late Radio ISR
   After this event with error code 0x01, 0x02 or 0x03, it is recommended to force a device reset. */

void hci_hardware_error_event(uint8_t Hardware_Code)
{
  if (Hardware_Code <= 0x03)
  {
    NVIC_SystemReset();
  }
}


#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
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

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
/** \endcond
 */
