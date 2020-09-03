cmake_minimum_required(VERSION 3.15)

project(MultiTargetDemo LANGUAGES C CXX ASM)

find_bsp(ID com.sysprogs.arm.stm32
	VERSION 2020.06
	MCU STM32F407VG
	FRAMEWORKS com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll
	HWREGISTER_LIST_FILE STM32F4xxxx/DeviceDefinitions/stm32f407xx.xml)

add_bsp_based_executable(NAME MultiTargetDemo
	SOURCES MultiTargetDemo.cpp
	GENERATE_BIN
	GENERATE_MAP)

target_link_libraries(MultiTargetDemo PRIVATE OSAL)

if(SIMULATION)
add_subdirectory(OSAL/Win32)
else()
add_subdirectory(OSAL/STM32)
endif()