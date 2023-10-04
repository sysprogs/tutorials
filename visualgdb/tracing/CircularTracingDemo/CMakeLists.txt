cmake_minimum_required(VERSION 3.15)

project(CircularTracingDemo LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32
	VERSION 2023.07
	MCU STM32F446RE
	FRAMEWORKS com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll com.sysprogs.arm.stm32.freertos
	HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f446xx.xml
	DISABLE_GNU_EXTENSIONS)

add_bsp_based_executable(
	NAME CircularTracingDemo
	SOURCES CircularTracingDemo.c FreeRTOSConfig.h system_stm32f4xx.c stm32f4xx_hal_conf.h
	GENERATE_BIN
	GENERATE_MAP
	OUTPUT_RELOCATION_RECORDS)
