cmake_minimum_required(VERSION 3.15)

project(BlueNRG_LP_Demo LANGUAGES C CXX ASM)

find_bsp(ID com.sysprogs.arm.stm32.bluenrg-lp
	VERSION 2020.05
	MCU BlueNRG-LP
	FRAMEWORKS
		com.sysprogs.arm.stm32.bluenrg.lp.bsp.steval-idb011v1
		com.sysprogs.arm.stm32.bluenrg.lp.rf_driver.hal_aux
		com.sysprogs.arm.stm32.bluenrg.lp.rf_driver.ll
		com.sysprogs.arm.stm32.bluenrg.lp.aesmgr
		com.sysprogs.arm.stm32.bluenrg.lp.ble.stack
		com.sysprogs.arm.stm32.bluenrg.lp.blecntr
		com.sysprogs.arm.stm32.bluenrg.lp.cryptolib
		com.sysprogs.arm.stm32.bluenrg.lp.hal_utils
		com.sysprogs.arm.stm32.bluenrg.lp.nvmdb
		com.sysprogs.arm.stm32.bluenrg.lp.pkamgr
		com.sysprogs.arm.stm32.bluenrg.lp.ble.app
		com.sysprogs.arm.stm32.bluenrg.lp.rngmgr
	FWCONFIGURATION
		com.sysprogs.bspoptions.stm32.bluenrg.core.speed=10uH
		com.sysprogs.bspoptions.stm32.bluenrg.core.ls=XTAL
		com.sysprogs.bspoptions.stm32.bluenrg.core.hse_tune=32
		com.sysprogs.bspoptions.stm32.bluenrg.ble.app.central=
		com.sysprogs.bspoptions.stm32.bluenrg.ble.app.queued_write=
	HWREGISTER_LIST_FILE DeviceDefinitions/bluenrg_lpx.xml)

bsp_include_directories(Inc)

bsp_compile_definitions(USER_BUTTON=BSP_PUSH1
	LEGACY_ADV)

add_bsp_based_executable(NAME BlueNRG_LP_Demo
	LINKER_SCRIPT WiSE-Studio/BlueNRG_LP.ld
	SOURCES
		Src/BLE_Beacon_main.c
		Src/bluenrg_lp_it.c
		Inc/Beacon_config.h
		Inc/bluenrg_lp_it.h
		Inc/custom_ble_stack_conf.h
	GENERATE_BIN
	GENERATE_MAP)
