cmake_minimum_required(VERSION 3.15)

project(Concentrator LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32wl
	VERSION 2021.11
	MCU STM32WL55JC
	FRAMEWORKS
		com.sysprogs.arm.stm32.utilities.trace
		com.sysprogs.arm.stm32.utilities.lpm
		com.sysprogs.arm.stm32.utilities.misc
		com.sysprogs.arm.stm32.utilities
		com.sysprogs.arm.stm32.utilities.timer
		com.sysprogs.arm.stm32.hal
		com.sysprogs.arm.stm32.ll
		com.sysprogs.arm.stm32.stm32bsp
		com.sysprogs.arm.stm32.subghz_phy
	HWREGISTER_LIST_FILE DeviceDefinitions/stm32wl55xx.xml
	DISABLE_GNU_EXTENSIONS
	CONFIGURATION com.sysprogs.toolchainoptions.arm.libctype=)

bsp_include_directories(Core/Inc
	SubGHz_Phy/App
	SubGHz_Phy/Target
	Common/Inc)

bsp_compile_definitions(CORE_CM4
	STM32WL55xx)

add_bsp_based_executable(
	NAME Concentrator
	LINKER_SCRIPT STM32WL55XX_FLASH_CM4.ld
	SOURCES
		SubGHz_Phy/App/demo_at.c
		SubGHz_Phy/App/demo_at.h
		SubGHz_Phy/App/demo_command.c
		SubGHz_Phy/App/demo_command.h
		SubGHz_Phy/App/demo_concentrator.c
		SubGHz_Phy/App/demo_concentrator.h
		SubGHz_Phy/App/demo_report.c
		SubGHz_Phy/App/demo_report.h
		Common/Src/demo_radio_utils.c
		Common/Src/demo_regions.c
		Common/Src/demo_utils.c
		Core/Src/adc.c
		Core/Src/adc_if.c
		Core/Src/dma.c
		Core/Src/main.c
		Core/Src/rtc.c
		Core/Src/stm32_lpm_if.c
		Core/Src/stm32wlxx_hal_msp.c
		Core/Src/stm32wlxx_it.c
		Core/Src/subghz.c
		Core/Src/sys_app.c
		Core/Src/sys_debug.c
		Core/Src/timer_if.c
		Core/Src/usart.c
		Core/Src/usart_if.c
		SubGHz_Phy/Target/radio_board_if.c
		Core/Src/system_stm32wlxx.c
		Common/Inc/demo_data_format.h
		Common/Inc/demo_packet_format.h
		Common/Inc/demo_prnd.h
		Common/Inc/demo_radio_utils.h
		Common/Inc/demo_regions.h
		Common/Inc/demo_utils.h
		Common/Inc/version.h
		Core/Inc/adc.h
		Core/Inc/adc_if.h
		Core/Inc/dma.h
		Core/Inc/main.h
		Core/Inc/platform.h
		Core/Inc/rtc.h
		Core/Inc/stm32wlxx_hal_conf.h
		Core/Inc/stm32wlxx_it.h
		Core/Inc/stm32wlxx_nucleo_conf.h
		Core/Inc/stm32_lpm_if.h
		Core/Inc/subghz.h
		Core/Inc/sys_app.h
		Core/Inc/sys_conf.h
		Core/Inc/sys_debug.h
		Core/Inc/timer_if.h
		Core/Inc/usart.h
		Core/Inc/usart_if.h
		Core/Inc/utilities_conf.h
		Core/Inc/utilities_def.h
	GENERATE_BIN
	GENERATE_MAP)
