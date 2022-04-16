cmake_minimum_required(VERSION 3.15)

project(RenesasARMDemo LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.renesas
	VERSION 3.3.0
	MCU R7FA2E1A92DFM
	FRAMEWORKS
		com.renesas.arm.Renesas.HAL_Drivers.r_adc
		com.renesas.arm.Arm.CMSIS.CMSIS5.CoreM
		com.renesas.arm.Renesas.Common.fsp_common
		com.renesas.arm.Renesas.BSP.device.mcu
		com.renesas.arm.Renesas.BSP.device.family
		com.renesas.arm.Renesas.BSP.fsp.family
		com.renesas.arm.Renesas.HAL_Drivers.r_ioport
		com.renesas.arm.Renesas.BSP.Board.default_board
	HWREGISTER_LIST_FILE DeviceDefinitions/R7FA2E1A9.xml
	DISABLE_GNU_EXTENSIONS
	FWCONFIGURATION "com.renesas.ra.device.pins.mode.p000=(uint32_t) IOPORT_CFG_PORT_DIRECTION_OUTPUT | (uint32_t) IOPORT_CFG_PORT_OUTPUT_HIGH")

add_bsp_based_executable(
	NAME RenesasARMDemo
	SOURCES
		hal_entry.cpp
		ra_cfg/fsp_cfg/bsp/bsp_cfg.h
		ra_cfg/fsp_cfg/bsp/bsp_pin_cfg.h
		ra_gen/pin_data.c
		ra_gen/common_data.h
		ra_gen/common_data.c
		ra_gen/hal_data.h
		ra_gen/hal_data.c
		ra_cfg/fsp_cfg/bsp/board_cfg.h
		ra_cfg/fsp_cfg/r_ioport_cfg.h
		ra_cfg/fsp_cfg/bsp/board_cfg.h
		ra_cfg/fsp_cfg/bsp/bsp_mcu_device_pn_cfg.h
		ra_cfg/fsp_cfg/bsp/bsp_mcu_device_cfg.h
		ra_cfg/fsp_cfg/bsp/bsp_mcu_family_cfg.h
		ra_gen/bsp_clock_cfg.h
		ra_gen/main.cpp
		ra_gen/vector_data.cpp
		ra_gen/vector_data.h
		ra_cfg/fsp_cfg/r_adc_cfg.h
	GENERATE_BIN
	GENERATE_MAP)
