#ifdef __cplusplus
extern "C"
{
#endif
	#include <usbd_core.h>
	#include <usbd_cdc.h>
	#include "usbd_cdc_if.h"
	#include <usbd_desc.h>

	USBD_HandleTypeDef USBD_Device;
	void SysTick_Handler(void);
	void OTG_FS_IRQHandler(void);
	void OTG_HS_IRQHandler(void);
	extern PCD_HandleTypeDef hpcd;
	
	int VCP_read(void *pBuffer, int size);
	int VCP_write(const void *pBuffer, int size);
	extern char g_VCPInitialized;
	
#ifdef __cplusplus
}
#endif

static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	__PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK |
	                               RCC_CLOCKTYPE_HCLK |
	                               RCC_CLOCKTYPE_PCLK1 |
	                               RCC_CLOCKTYPE_PCLK2);

	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
	
}


void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd);
}
#elif defined(USE_USB_HS)
void OTG_HS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd);
}
#else
#error USB peripheral type not defined
#endif

/*
	This is a basic USB CDC device sample based on the ST USB library.
	To test it out:
		1. Install the ST VCP drivers (http://www.st.com/web/en/catalog/tools/PF257938)
		2. Connect to the virtual COM port using SmarTTY or any other terminal program
		3. Type some characters and observe the output
	Read more about the sample: http://visualgdb.com/tutorials/arm/stm32/usb/
*/

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	USBD_Init(&USBD_Device, &VCP_Desc, 0);

	USBD_RegisterClass(&USBD_Device, &USBD_CDC);
	USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_USBCDCDemo_New_fops);
	USBD_Start(&USBD_Device);

	char byte;
	for (;;)
	{
		if (VCP_read(&byte, 1) != 1)
			continue;
		VCP_write("\r\nYou typed ", 12);
		VCP_write(&byte, 1);
		VCP_write("\r\n", 2);
	}
}