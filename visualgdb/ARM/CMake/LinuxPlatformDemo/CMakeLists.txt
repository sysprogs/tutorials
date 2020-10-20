cmake_minimum_required(VERSION 3.15)

project(LinuxPlatformDemo LANGUAGES C CXX ASM)

find_bsp(ID com.sysprogs.arm.stm32
	VERSION 2020.10
	MCU STM32H747XI
	FRAMEWORKS com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll com.sysprogs.embedded.semihosting_and_profiler
	HWREGISTER_LIST_FILE STM32H7xxxx/DeviceDefinitions/stm32h747xx.xml)

add_bsp_based_executable(NAME LinuxPlatformDemo
	SOURCES LinuxPlatformDemo.cpp system_stm32h7xx.c stm32h7xx_hal_conf.h
	GENERATE_BIN
	GENERATE_MAP)
