cmake_minimum_required(VERSION 3.15)

project(STM32H7S_App LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32.h7rs
	VERSION 1.1.0
	MCU STM32H7S3L8
	FRAMEWORKS com.sysprogs.arm.stm32.hal com.sysprogs.arm.stm32.ll
	FWCONFIGURATION com.sysprogs.bspoptions.stm32.ll_driver=USE_FULL_LL_DRIVER
	HWREGISTER_LIST_FILE STM32H7RSxx/DeviceDefinitions/stm32h7s3xx.xml
	DISABLE_GNU_EXTENSIONS)

bsp_include_directories(Appli/Inc
	${BSP_ROOT}/STM32H7RSxx/STM32H7RSxx_HAL_Driver/Inc
	${BSP_ROOT}/STM32H7RSxx/STM32H7RSxx_HAL_Driver/Inc/Legacy
	${BSP_ROOT}/STM32H7RSxx/STM32_USBPD_Library/Core/inc
	${BSP_ROOT}/STM32H7RSxx/STM32_USBPD_Library/Devices/STM32H7RSXX/inc
	${BSP_ROOT}/STM32H7RSxx/STM32_USB_Device_Library/Core/Inc
	${BSP_ROOT}/STM32H7RSxx/STM32_USB_Device_Library/Class/CDC/Inc
	${BSP_ROOT}/STM32H7RSxx/CMSIS_HAL/Device/ST/STM32H7RSxx/Include
	${BSP_ROOT}/STM32H7RSxx/CMSIS_HAL/Include
	${BSP_ROOT}/STM32H7RSxx/BSP/STM32H7RSxx_Nucleo
	${BSP_ROOT}/STM32H7RSxx/BSP/Components/tcpp0203
	${BSP_ROOT}/STM32H7RSxx/CMSIS_HAL/Include)

bsp_compile_definitions(USE_HAL_DRIVER
	USBPD_PORT_COUNT=1
	_SNK
	USBPDCORE_LIB_NO_PD
	TCPP0203_SUPPORT
	STM32H7S3xx)

add_bsp_based_executable(
	NAME STM32H7S_App
	LINKER_SCRIPT STM32CubeIDE/STM32H7S3L8HX_ROMxspi2_app.ld
	SOURCES
		README.md
		Appli/Src/main.c
		Appli/Src/stm32h7rsxx_hal_msp.c
		Appli/Src/stm32h7rsxx_it.c
		Appli/Src/usb_device.c
		Appli/Src/usbd_cdc_if.c
		Appli/Src/usbd_conf.c
		Appli/Src/usbd_desc.c
		Appli/Src/usbpd.c
		Appli/Src/usbpd_dpm_core.c
		Appli/Src/usbpd_dpm_user.c
		Appli/Src/usbpd_pwr_if.c
		Appli/Src/usbpd_pwr_user.c
		Appli/Src/usbpd_usb_if.c
		Appli/Src/system_stm32h7rsxx.c
		Appli/Inc/main.h
		Appli/Inc/stm32h7rsxx_hal_conf.h
		Appli/Inc/stm32h7rsxx_it.h
		Appli/Inc/stm32h7rsxx_nucleo_conf.h
		Appli/Inc/stm32_assert.h
		Appli/Inc/usbd_cdc_if.h
		Appli/Inc/usbd_conf.h
		Appli/Inc/usbd_desc.h
		Appli/Inc/usbpd.h
		Appli/Inc/usbpd_devices_conf.h
		Appli/Inc/usbpd_dpm_conf.h
		Appli/Inc/usbpd_dpm_core.h
		Appli/Inc/usbpd_dpm_user.h
		Appli/Inc/usbpd_os_port_mx.h
		Appli/Inc/usbpd_pdo_defs.h
		Appli/Inc/usbpd_pwr_if.h
		Appli/Inc/usbpd_pwr_user.h
		Appli/Inc/usbpd_usb_if.h
		Appli/Inc/usb_device.h
		${BSP_ROOT}/STM32H7RSxx/STM32_USBPD_Library/Devices/STM32H7RSXX/src/usbpd_cad_hw_if.c
		${BSP_ROOT}/STM32H7RSxx/STM32_USBPD_Library/Devices/STM32H7RSXX/src/usbpd_hw.c
		${BSP_ROOT}/STM32H7RSxx/STM32_USBPD_Library/Devices/STM32H7RSXX/src/usbpd_hw_if_it.c
		${BSP_ROOT}/STM32H7RSxx/STM32_USBPD_Library/Devices/STM32H7RSXX/src/usbpd_phy_hw_if.c
		${BSP_ROOT}/STM32H7RSxx/STM32_USBPD_Library/Devices/STM32H7RSXX/src/usbpd_pwr_hw_if.c
		${BSP_ROOT}/STM32H7RSxx/STM32_USBPD_Library/Core/src/usbpd_trace.c
		${BSP_ROOT}/STM32H7RSxx/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
		${BSP_ROOT}/STM32H7RSxx/STM32_USB_Device_Library/Core/Src/usbd_core.c
		${BSP_ROOT}/STM32H7RSxx/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
		${BSP_ROOT}/STM32H7RSxx/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
		${BSP_ROOT}/STM32H7RSxx/BSP/Components/tcpp0203/tcpp0203.c
		${BSP_ROOT}/STM32H7RSxx/BSP/Components/tcpp0203/tcpp0203_reg.c
		${BSP_ROOT}/STM32H7RSxx/BSP/STM32H7RSxx_Nucleo/stm32h7rsxx_nucleo.c
		${BSP_ROOT}/STM32H7RSxx/BSP/STM32H7RSxx_Nucleo/stm32h7rsxx_nucleo_bus.c
		${BSP_ROOT}/STM32H7RSxx/BSP/STM32H7RSxx_Nucleo/stm32h7rsxx_nucleo_usbpd_pwr.c
	GENERATE_BIN
	GENERATE_MAP
	OUTPUT_RELOCATION_RECORDS)
target_link_libraries(STM32H7S_App
	PRIVATE
	${BSP_ROOT}/STM32H7RSxx/STM32_USBPD_Library/Core/lib/USBPDCORE_NOPD_CM7_wc32.a)

set_target_properties(STM32H7S_App PROPERTIES SUFFIX ".elf")
