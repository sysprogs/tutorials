cmake_minimum_required(VERSION 3.15)

project(EmbeddedSimulationDemo LANGUAGES C CXX ASM)

find_bsp(ID com.sysprogs.arm.stm32
	VERSION 2020.06
	MCU STM32F407VG
	FRAMEWORKS com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll com.sysprogs.embedded.semihosting_and_profiler
	HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f407xx.xml)

find_test_framework(ID com.sysprogs.unittest.tinyembtest)

add_bsp_based_executable(NAME EmbeddedSimulationDemo
	SOURCES
		EmbeddedSimulationDemoTests.cpp
		EmbeddedSimulationDemo.cpp
		system_stm32f4xx.c
		stm32f4xx_hal_conf.h
	GENERATE_BIN
	GENERATE_MAP
	BUILD_UNIT_TESTS)

if(SIMULATION)
	set_source_files_properties(system_stm32f4xx.c PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

add_bsp_based_executable(NAME AnotherTestExecutable
	SOURCES AnotherTestExecutable.cpp
	GENERATE_BIN
	GENERATE_MAP
	BUILD_UNIT_TESTS
	EXCLUDED_PLATFORMS Hardware)
