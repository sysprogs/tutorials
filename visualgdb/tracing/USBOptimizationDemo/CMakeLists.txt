cmake_minimum_required(VERSION 3.15)

project(USBOptimizationDemo LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32
	VERSION 2023.07
	MCU STM32F746NG
	FRAMEWORKS com.sysprogs.arm.stm32.usbdev com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll
	HWREGISTER_LIST_FILE STM32F7xxxx/DeviceDefinitions/stm32f746xx.xml
	DISABLE_GNU_EXTENSIONS
	FWCONFIGURATION com.sysprogs.bspoptions.stm32.usb.speed=HS)

add_bsp_based_executable(
	NAME USBOptimizationDemo
	SOURCES
		USBOptimizationDemo.cpp
		usbd_cdc_if.c
		usbd_cdc_if.h
		usbd_conf.c
		usbd_conf.h
		usbd_desc.c
		usbd_desc.h
		system_stm32f7xx.c
		stm32f7xx_hal_conf.h
	GENERATE_BIN
	GENERATE_MAP
	OUTPUT_RELOCATION_RECORDS)
