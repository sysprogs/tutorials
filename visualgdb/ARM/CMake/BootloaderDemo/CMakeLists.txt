cmake_minimum_required(VERSION 3.15)

project(BootloaderDemo LANGUAGES C CXX ASM)

find_bsp(ID com.sysprogs.arm.stm32
	VERSION 2020.10
	MCU STM32F407VG
	FRAMEWORKS com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll
	HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f407xx.xml)

add_bsp_based_executable(NAME BootloaderDemo
	SOURCES BootloaderDemo.cpp system_stm32f4xx.c stm32f4xx_hal_conf.h
	GENERATE_BIN
	GENERATE_MAP
	LINKER_SCRIPT bootloader.lds)

add_bsp_based_executable(NAME TargetApplication
	SOURCES TargetApplication.cpp system_stm32f4xx.c
	GENERATE_BIN
	GENERATE_MAP
	LINKER_SCRIPT application.lds)

embed_target_output(FROM_TARGET BootloaderDemo IN_TARGETS TargetApplication SECTION .bootldr)
