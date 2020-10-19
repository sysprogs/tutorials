cmake_minimum_required(VERSION 3.15)

project(StandaloneFrameworkDemo LANGUAGES C CXX ASM)

find_bsp(ID com.sysprogs.arm.stm32
	VERSION 2020.10
	MCU STM32F407VG
	FRAMEWORKS com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll
	HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f407xx.xml)
import_framework(NAME USBFramework PATH USBFramework)

add_bsp_based_executable(NAME StandaloneFrameworkDemo
	SOURCES
		StandaloneFrameworkDemo.cpp
		usbd_cdc_if.c
		usbd_cdc_if.h
		usbd_conf.c
		usbd_conf.h
		usbd_desc.c
		usbd_desc.h
		system_stm32f4xx.c
		stm32f4xx_hal_conf.h
	GENERATE_BIN
	GENERATE_MAP)
target_link_libraries(StandaloneFrameworkDemo PRIVATE USBFramework)
