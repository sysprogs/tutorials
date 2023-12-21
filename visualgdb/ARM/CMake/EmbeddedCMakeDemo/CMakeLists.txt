cmake_minimum_required(VERSION 3.15)

project(EmbeddedCMakeDemo LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32
	VERSION 2023.07
	MCU STM32F407VG
	FRAMEWORKS com.sysprogs.arm.stm32.freertos com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll
	HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f407xx.xml
	FWCONFIGURATION com.sysprogs.bspoptions.stm32.freertos.heap=heap_1)

add_bsp_based_executable(NAME EmbeddedCMakeDemo
	SOURCES
		EmbeddedCMakeDemo.c
		FreeRTOSConfig.h
		system_stm32f4xx.c
		stm32f4xx_hal_conf.h
		heap_1.c
	GENERATE_BIN
	GENERATE_MAP)

target_link_libraries(EmbeddedCMakeDemo PRIVATE StaticLibrary)
add_bsp_based_library(NAME StaticLibrary
	SOURCES StaticLibrary.cpp)
set_source_files_properties(${BSP_ROOT}/FreeRTOS/Source/portable/MemMang/heap_1.c PROPERTIES HEADER_FILE_ONLY TRUE)
