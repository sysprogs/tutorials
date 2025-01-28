cmake_minimum_required(VERSION 3.15)

project(STM32H7S_CDC LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32.h7rs
	VERSION 1.1.0
	MCU STM32H7S3L8
	FRAMEWORKS com.sysprogs.arm.stm32.extmem.manager com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll
	HWREGISTER_LIST_FILE STM32H7RSxx/DeviceDefinitions/stm32h7s3xx.xml
	DISABLE_GNU_EXTENSIONS)

bsp_include_directories(Boot/Inc
	${BSP_ROOT}/STM32H7RSxx/STM32H7RSxx_HAL_Driver/Inc
	${BSP_ROOT}/STM32H7RSxx/STM32H7RSxx_HAL_Driver/Inc/Legacy
	${BSP_ROOT}/STM32_ExtMem_Manager
	${BSP_ROOT}/STM32H7RSxx/CMSIS_HAL/Device/ST/STM32H7RSxx/Include
	${BSP_ROOT}/STM32H7RSxx/CMSIS_HAL/Include
	${BSP_ROOT}/STM32H7RSxx/CMSIS_HAL/Include)

bsp_compile_definitions(USE_HAL_DRIVER
	STM32H7S3xx)

add_bsp_based_executable(
	NAME STM32H7S_CDC
	LINKER_SCRIPT STM32CubeIDE/STM32H7S3L8HX_FLASH.ld
	SOURCES
		README.md
		Boot/Src/extmem_manager.c
		Boot/Src/main.c
		Boot/Src/stm32h7rsxx_hal_msp.c
		Boot/Src/stm32h7rsxx_it.c
		Boot/Src/system_stm32h7rsxx.c
		Boot/Inc/extmem_manager.h
		Boot/Inc/main.h
		Boot/Inc/stm32h7rsxx_hal_conf.h
		Boot/Inc/stm32h7rsxx_it.h
		Boot/Inc/stm32_extmem_conf.h
	GENERATE_BIN
	GENERATE_MAP
	OUTPUT_RELOCATION_RECORDS)
