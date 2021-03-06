/*
	This file contains the entry point (Reset_Handler) of your firmware project.
	The reset handled initializes the RAM and calls system library initializers as well as
	the platform-specific initializer and the main() function.
*/

#include <stddef.h>
extern void *_estack;

void Reset_Handler();
void Default_Handler();

#ifndef DEBUG_DEFAULT_INTERRUPT_HANDLERS

#ifdef DEBUG
#define DEBUG_DEFAULT_INTERRUPT_HANDLERS 1
#else
#define DEBUG_DEFAULT_INTERRUPT_HANDLERS 0
#endif

#endif

#if DEBUG_DEFAULT_INTERRUPT_HANDLERS
void __attribute__ ((weak, naked)) NMI_Handler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void NMI_Handler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) HardFault_Handler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void HardFault_Handler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) MemManage_Handler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void MemManage_Handler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) BusFault_Handler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void BusFault_Handler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) UsageFault_Handler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void UsageFault_Handler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) SVC_Handler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void SVC_Handler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DebugMon_Handler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DebugMon_Handler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) PendSV_Handler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void PendSV_Handler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) SysTick_Handler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void SysTick_Handler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) WWDG_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void WWDG_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) PVD_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void PVD_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TAMP_STAMP_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TAMP_STAMP_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) RTC_WKUP_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void RTC_WKUP_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) FLASH_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void FLASH_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) RCC_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void RCC_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) EXTI0_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI0_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) EXTI1_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI1_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) EXTI2_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI2_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) EXTI3_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI3_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) EXTI4_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI4_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA1_Stream0_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Stream0_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA1_Stream1_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Stream1_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA1_Stream2_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Stream2_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA1_Stream3_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Stream3_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA1_Stream4_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Stream4_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA1_Stream5_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Stream5_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA1_Stream6_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Stream6_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) ADC_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void ADC_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) CAN1_TX_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void CAN1_TX_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) CAN1_RX0_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void CAN1_RX0_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) CAN1_RX1_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void CAN1_RX1_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) CAN1_SCE_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void CAN1_SCE_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) EXTI9_5_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI9_5_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM1_BRK_TIM9_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM1_BRK_TIM9_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM1_UP_TIM10_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM1_UP_TIM10_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM1_TRG_COM_TIM11_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM1_TRG_COM_TIM11_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM1_CC_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM1_CC_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM2_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM2_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM3_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM3_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM4_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM4_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) I2C1_EV_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void I2C1_EV_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) I2C1_ER_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void I2C1_ER_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) I2C2_EV_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void I2C2_EV_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) I2C2_ER_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void I2C2_ER_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) SPI1_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void SPI1_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) SPI2_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void SPI2_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) USART1_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void USART1_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) USART2_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void USART2_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) USART3_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void USART3_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) EXTI15_10_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI15_10_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) RTC_Alarm_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void RTC_Alarm_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) OTG_FS_WKUP_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void OTG_FS_WKUP_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM8_BRK_TIM12_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM8_BRK_TIM12_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM8_UP_TIM13_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM8_UP_TIM13_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM8_TRG_COM_TIM14_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM8_TRG_COM_TIM14_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM8_CC_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM8_CC_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA1_Stream7_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Stream7_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) FSMC_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void FSMC_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) SDIO_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void SDIO_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM5_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM5_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) SPI3_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void SPI3_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) UART4_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void UART4_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) UART5_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void UART5_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM6_DAC_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM6_DAC_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) TIM7_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void TIM7_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA2_Stream0_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA2_Stream0_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA2_Stream1_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA2_Stream1_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA2_Stream2_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA2_Stream2_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA2_Stream3_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA2_Stream3_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA2_Stream4_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA2_Stream4_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) ETH_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void ETH_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) ETH_WKUP_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void ETH_WKUP_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) CAN2_TX_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void CAN2_TX_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) CAN2_RX0_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void CAN2_RX0_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) CAN2_RX1_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void CAN2_RX1_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) CAN2_SCE_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void CAN2_SCE_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) OTG_FS_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void OTG_FS_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA2_Stream5_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA2_Stream5_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA2_Stream6_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA2_Stream6_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DMA2_Stream7_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DMA2_Stream7_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) USART6_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void USART6_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) I2C3_EV_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void I2C3_EV_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) I2C3_ER_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void I2C3_ER_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) OTG_HS_EP1_OUT_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void OTG_HS_EP1_OUT_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) OTG_HS_EP1_IN_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void OTG_HS_EP1_IN_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) OTG_HS_WKUP_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void OTG_HS_WKUP_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) OTG_HS_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void OTG_HS_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) DCMI_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void DCMI_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) HASH_RNG_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void HASH_RNG_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}
void __attribute__ ((weak, naked)) FPU_IRQHandler() 
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code. 
	//Define the following function in your code to handle it:
	//	extern "C" void FPU_IRQHandler();
	__asm("bkpt 255");
	__asm("bx lr");
}

#else
void NMI_Handler()                    __attribute__ ((weak, alias ("Default_Handler")));
void HardFault_Handler()              __attribute__ ((weak, alias ("Default_Handler")));
void MemManage_Handler()              __attribute__ ((weak, alias ("Default_Handler")));
void BusFault_Handler()               __attribute__ ((weak, alias ("Default_Handler")));
void UsageFault_Handler()             __attribute__ ((weak, alias ("Default_Handler")));
void SVC_Handler()                    __attribute__ ((weak, alias ("Default_Handler")));
void DebugMon_Handler()               __attribute__ ((weak, alias ("Default_Handler")));
void PendSV_Handler()                 __attribute__ ((weak, alias ("Default_Handler")));
void SysTick_Handler()                __attribute__ ((weak, alias ("Default_Handler")));
void WWDG_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void PVD_IRQHandler()                 __attribute__ ((weak, alias ("Default_Handler")));
void TAMP_STAMP_IRQHandler()          __attribute__ ((weak, alias ("Default_Handler")));
void RTC_WKUP_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void FLASH_IRQHandler()               __attribute__ ((weak, alias ("Default_Handler")));
void RCC_IRQHandler()                 __attribute__ ((weak, alias ("Default_Handler")));
void EXTI0_IRQHandler()               __attribute__ ((weak, alias ("Default_Handler")));
void EXTI1_IRQHandler()               __attribute__ ((weak, alias ("Default_Handler")));
void EXTI2_IRQHandler()               __attribute__ ((weak, alias ("Default_Handler")));
void EXTI3_IRQHandler()               __attribute__ ((weak, alias ("Default_Handler")));
void EXTI4_IRQHandler()               __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream0_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream1_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream2_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream3_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream4_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream5_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream6_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void ADC_IRQHandler()                 __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_TX_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_RX0_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_RX1_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_SCE_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void EXTI9_5_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_BRK_TIM9_IRQHandler()       __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_UP_TIM10_IRQHandler()       __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_TRG_COM_TIM11_IRQHandler()  __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_CC_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void TIM2_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void TIM3_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void TIM4_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void I2C1_EV_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void I2C1_ER_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void I2C2_EV_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void I2C2_ER_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void SPI1_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void SPI2_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void USART1_IRQHandler()              __attribute__ ((weak, alias ("Default_Handler")));
void USART2_IRQHandler()              __attribute__ ((weak, alias ("Default_Handler")));
void USART3_IRQHandler()              __attribute__ ((weak, alias ("Default_Handler")));
void EXTI15_10_IRQHandler()           __attribute__ ((weak, alias ("Default_Handler")));
void RTC_Alarm_IRQHandler()           __attribute__ ((weak, alias ("Default_Handler")));
void OTG_FS_WKUP_IRQHandler()         __attribute__ ((weak, alias ("Default_Handler")));
void TIM8_BRK_TIM12_IRQHandler()      __attribute__ ((weak, alias ("Default_Handler")));
void TIM8_UP_TIM13_IRQHandler()       __attribute__ ((weak, alias ("Default_Handler")));
void TIM8_TRG_COM_TIM14_IRQHandler()  __attribute__ ((weak, alias ("Default_Handler")));
void TIM8_CC_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream7_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void FSMC_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void SDIO_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void TIM5_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void SPI3_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void UART4_IRQHandler()               __attribute__ ((weak, alias ("Default_Handler")));
void UART5_IRQHandler()               __attribute__ ((weak, alias ("Default_Handler")));
void TIM6_DAC_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void TIM7_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream0_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream1_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream2_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream3_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream4_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void ETH_IRQHandler()                 __attribute__ ((weak, alias ("Default_Handler")));
void ETH_WKUP_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void CAN2_TX_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void CAN2_RX0_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void CAN2_RX1_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void CAN2_SCE_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void OTG_FS_IRQHandler()              __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream5_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream6_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream7_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void USART6_IRQHandler()              __attribute__ ((weak, alias ("Default_Handler")));
void I2C3_EV_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void I2C3_ER_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void OTG_HS_EP1_OUT_IRQHandler()      __attribute__ ((weak, alias ("Default_Handler")));
void OTG_HS_EP1_IN_IRQHandler()       __attribute__ ((weak, alias ("Default_Handler")));
void OTG_HS_WKUP_IRQHandler()         __attribute__ ((weak, alias ("Default_Handler")));
void OTG_HS_IRQHandler()              __attribute__ ((weak, alias ("Default_Handler")));
void DCMI_IRQHandler()                __attribute__ ((weak, alias ("Default_Handler")));
void HASH_RNG_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void FPU_IRQHandler()                 __attribute__ ((weak, alias ("Default_Handler")));
#endif

void * g_pfnVectors[0x62] __attribute__ ((section (".isr_vector"), used)) = 
{
	&_estack,
	&Reset_Handler,
	&NMI_Handler,
	&HardFault_Handler,
	&MemManage_Handler,
	&BusFault_Handler,
	&UsageFault_Handler,
	NULL,
	NULL,
	NULL,
	NULL,
	&SVC_Handler,
	&DebugMon_Handler,
	NULL,
	&PendSV_Handler,
	&SysTick_Handler,
	&WWDG_IRQHandler,
	&PVD_IRQHandler,
	&TAMP_STAMP_IRQHandler,
	&RTC_WKUP_IRQHandler,
	&FLASH_IRQHandler,
	&RCC_IRQHandler,
	&EXTI0_IRQHandler,
	&EXTI1_IRQHandler,
	&EXTI2_IRQHandler,
	&EXTI3_IRQHandler,
	&EXTI4_IRQHandler,
	&DMA1_Stream0_IRQHandler,
	&DMA1_Stream1_IRQHandler,
	&DMA1_Stream2_IRQHandler,
	&DMA1_Stream3_IRQHandler,
	&DMA1_Stream4_IRQHandler,
	&DMA1_Stream5_IRQHandler,
	&DMA1_Stream6_IRQHandler,
	&ADC_IRQHandler,
	&CAN1_TX_IRQHandler,
	&CAN1_RX0_IRQHandler,
	&CAN1_RX1_IRQHandler,
	&CAN1_SCE_IRQHandler,
	&EXTI9_5_IRQHandler,
	&TIM1_BRK_TIM9_IRQHandler,
	&TIM1_UP_TIM10_IRQHandler,
	&TIM1_TRG_COM_TIM11_IRQHandler,
	&TIM1_CC_IRQHandler,
	&TIM2_IRQHandler,
	&TIM3_IRQHandler,
	&TIM4_IRQHandler,
	&I2C1_EV_IRQHandler,
	&I2C1_ER_IRQHandler,
	&I2C2_EV_IRQHandler,
	&I2C2_ER_IRQHandler,
	&SPI1_IRQHandler,
	&SPI2_IRQHandler,
	&USART1_IRQHandler,
	&USART2_IRQHandler,
	&USART3_IRQHandler,
	&EXTI15_10_IRQHandler,
	&RTC_Alarm_IRQHandler,
	&OTG_FS_WKUP_IRQHandler,
	&TIM8_BRK_TIM12_IRQHandler,
	&TIM8_UP_TIM13_IRQHandler,
	&TIM8_TRG_COM_TIM14_IRQHandler,
	&TIM8_CC_IRQHandler,
	&DMA1_Stream7_IRQHandler,
	&FSMC_IRQHandler,
	&SDIO_IRQHandler,
	&TIM5_IRQHandler,
	&SPI3_IRQHandler,
	&UART4_IRQHandler,
	&UART5_IRQHandler,
	&TIM6_DAC_IRQHandler,
	&TIM7_IRQHandler,
	&DMA2_Stream0_IRQHandler,
	&DMA2_Stream1_IRQHandler,
	&DMA2_Stream2_IRQHandler,
	&DMA2_Stream3_IRQHandler,
	&DMA2_Stream4_IRQHandler,
	&ETH_IRQHandler,
	&ETH_WKUP_IRQHandler,
	&CAN2_TX_IRQHandler,
	&CAN2_RX0_IRQHandler,
	&CAN2_RX1_IRQHandler,
	&CAN2_SCE_IRQHandler,
	&OTG_FS_IRQHandler,
	&DMA2_Stream5_IRQHandler,
	&DMA2_Stream6_IRQHandler,
	&DMA2_Stream7_IRQHandler,
	&USART6_IRQHandler,
	&I2C3_EV_IRQHandler,
	&I2C3_ER_IRQHandler,
	&OTG_HS_EP1_OUT_IRQHandler,
	&OTG_HS_EP1_IN_IRQHandler,
	&OTG_HS_WKUP_IRQHandler,
	&OTG_HS_IRQHandler,
	&DCMI_IRQHandler,
	NULL,
	&HASH_RNG_IRQHandler,
	&FPU_IRQHandler,
};

void SystemInit();
void __libc_init_array();
int main();

extern void *_sidata, *_sdata, *_edata;
extern void *_sbss, *_ebss;

void __attribute__((naked, noreturn)) Reset_Handler()
{
	//Normally the CPU should will setup the based on the value from the first entry in the vector table.
	//If you encounter problems with accessing stack variables during initialization, ensure the line below is enabled.
	#if defined(sram_layout) || defined(INITIALIZE_SP_AT_RESET)
	__asm ("ldr sp, =_estack");
	#endif

	void **pSource, **pDest;
	for (pSource = &_sidata, pDest = &_sdata; pDest != &_edata; pSource++, pDest++)
		*pDest = *pSource;

	for (pDest = &_sbss; pDest != &_ebss; pDest++)
		*pDest = 0;

	SystemInit();
	__libc_init_array();
	(void)main();
	for (;;) ;
}

void __attribute__((naked, noreturn)) Default_Handler()
{
	//If you get stuck here, your code is missing a handler for some interrupt.
	//Define a 'DEBUG_DEFAULT_INTERRUPT_HANDLERS' macro via VisualGDB Project Properties and rebuild your project.
	//This will pinpoint a specific missing vector.
	for (;;) ;
}
