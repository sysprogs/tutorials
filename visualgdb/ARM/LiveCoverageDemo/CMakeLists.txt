cmake_minimum_required(VERSION 3.15)

project(LiveCoverageDemo LANGUAGES C CXX ASM)

find_bsp(ID com.sysprogs.arm.stm32
	VERSION 2020.10
	MCU STM32F407VG
	FRAMEWORKS com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll com.sysprogs.arm.stm32.usbdev
	HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f407xx.xml)

bsp_configure_code_coverage(ENABLED 1
	EXCLUDE_TARGETS BSP_com.sysprogs.arm.stm32.hal
	EXCLUDE_FILES ${BSP_ROOT}/STM32F4xxxx/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c)

add_bsp_based_executable(NAME LiveCoverageDemo
	SOURCES
		LiveCoverageDemo.cpp
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
