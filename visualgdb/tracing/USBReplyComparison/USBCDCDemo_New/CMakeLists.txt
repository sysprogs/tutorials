cmake_minimum_required(VERSION 3.15)

project(USBCDCDemo_New LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32
	VERSION 2023.07
	MCU STM32F407VG
	FRAMEWORKS com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll com.sysprogs.arm.stm32.usbdev
	HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f407xx.xml
	DISABLE_GNU_EXTENSIONS)

add_bsp_based_executable(
	NAME USBCDCDemo_New
	SOURCES
		USBCDCDemo_New.cpp
		usbd_cdc_if.c
		usbd_cdc_if.h
		usbd_conf.c
		usbd_conf.h
		usbd_desc.c
		usbd_desc.h
		system_stm32f4xx.c
		stm32f4xx_hal_conf.h
	GENERATE_BIN
	GENERATE_MAP
	OUTPUT_RELOCATION_RECORDS)
