cmake_minimum_required(VERSION 3.15)

project(ThreadXDemo LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32
	VERSION 2022.01
	MCU STM32F407VG
	FRAMEWORKS com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll com.sysprogs.arm.stm32.threadx
	HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f407xx.xml
	DISABLE_GNU_EXTENSIONS)

add_bsp_based_executable(
	NAME ThreadXDemo
	SOURCES
		ThreadXDemo.c
		app_azure_rtos_config.h
		app_threadx.h
		tx_initialize_low_level.S
		tx_user.h
		system_stm32f4xx.c
		stm32f4xx_hal_conf.h
	GENERATE_BIN
	GENERATE_MAP)
