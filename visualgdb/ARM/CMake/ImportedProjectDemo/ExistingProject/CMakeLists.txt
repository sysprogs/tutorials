cmake_minimum_required(VERSION 3.15)

SET(TOOLCHAIN_ROOT c:/SysGCC/arm-eabi)

SET(CMAKE_C_COMPILER "${TOOLCHAIN_ROOT}/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_ROOT}/bin/arm-none-eabi-g++.exe")
SET(CMAKE_ASM_COMPILER "${TOOLCHAIN_ROOT}/bin/arm-none-eabi-g++.exe")
SET(CMAKE_AR "${TOOLCHAIN_ROOT}/bin/arm-none-eabi-ar.exe")
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
SET(CMAKE_SYSTEM_NAME Generic)

project(StandaloneSTM32Test LANGUAGES C CXX ASM)
	
add_executable( StandaloneSTM32Test StandaloneSTM32Test.cpp startup_stm32f407xx.c)

target_compile_options(StandaloneSTM32Test PRIVATE --specs=nano.specs --specs=nosys.specs -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
target_link_libraries(StandaloneSTM32Test PRIVATE --specs=nano.specs --specs=nosys.specs -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F407VG_flash.lds)
target_compile_definitions(StandaloneSTM32Test PRIVATE ARM_MATH_CM4 flash_layout STM32F407VG)