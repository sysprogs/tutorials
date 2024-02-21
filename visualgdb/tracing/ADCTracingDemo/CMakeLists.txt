cmake_minimum_required(VERSION 3.15)

project(ADCTracingDemo LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32
	VERSION 2023.07
	MCU STM32F407VG
	HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f407xx.xml
	DISABLE_GNU_EXTENSIONS)

bsp_include_directories(Inc
	${BSP_ROOT}/STM32F4xxxx/CMSIS_HAL/Device/ST/STM32F4xx/Include
	${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Inc
	${BSP_ROOT}/STM32F4xxxx/BSP/STM32F4-Discovery
	${BSP_ROOT}/STM32F4xxxx/CMSIS_HAL/Include
	${BSP_ROOT}/STM32F4xxxx/CMSIS_HAL/Include)

bsp_compile_definitions(USE_HAL_DRIVER
	USE_STM32F4_DISCO
	STM32F407xx)

add_bsp_based_executable(
	NAME ADCTracingDemo
	LINKER_SCRIPT STM32F407VGTx_FLASH.ld
	SOURCES
		Src/main.c
		readme.txt
		Src/system_stm32f4xx.c
		Src/stm32f4xx_hal_msp.c
		Src/stm32f4xx_it.c
		Inc/main.h
		Inc/stm32f4xx_hal_conf.h
		Inc/stm32f4xx_it.h
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
		${BSP_ROOT}/STM32F4xxxx/BSP/STM32F4-Discovery/stm32f4_discovery.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
		${BSP_ROOT}/STM32F4xxxx/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
	GENERATE_BIN
	GENERATE_MAP
	OUTPUT_RELOCATION_RECORDS)
