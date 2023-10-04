cmake_minimum_required(VERSION 3.15)

project(TestTracingDemo LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32
	VERSION 2023.07
	MCU STM32F446RE
	FRAMEWORKS com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll com.sysprogs.embedded.semihosting_and_profiler
	HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f446xx.xml
	DISABLE_GNU_EXTENSIONS)

find_test_framework(
	ID com.sysprogs.unittest.tinyembtest)

add_bsp_based_executable(
	NAME TestTracingDemo
	SOURCES TestTracingDemoTests.cpp TestTracingDemo.cpp system_stm32f4xx.c stm32f4xx_hal_conf.h
	GENERATE_BIN
	GENERATE_MAP
	BUILD_UNIT_TESTS
	OUTPUT_RELOCATION_RECORDS)
