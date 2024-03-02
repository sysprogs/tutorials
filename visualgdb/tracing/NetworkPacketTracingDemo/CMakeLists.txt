cmake_minimum_required(VERSION 3.15)

project(NetworkPacketTracingDemo LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.arm.stm32
	VERSION 2023.07
	MCU STM32F746NG
	CONFIGURATION com.sysprogs.bspoptions.arm.floatmode=-mfloat-abi=hard
	HWREGISTER_LIST_FILE STM32F7xxxx/DeviceDefinitions/stm32f746xx.xml
	DISABLE_GNU_EXTENSIONS)

bsp_include_directories(Inc
	Src
	${BSP_ROOT}/STM32F7xxxx/CMSIS_HAL/Device/ST/STM32F7xx/Include
	${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Inc
	${BSP_ROOT}/STM32F7xxxx/BSP/STM32746G-Discovery
	${BSP_ROOT}/STM32F7xxxx/BSP/Components/Common
	${BSP_ROOT}/STM32F7xxxx/BSP/Components
	${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/include
	${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/system
	${BSP_ROOT}/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
	${BSP_ROOT}/FreeRTOS/Source
	${BSP_ROOT}/FreeRTOS/Source/CMSIS_RTOS
	${BSP_ROOT}/FreeRTOS/Source/include
	${BSP_ROOT}/VendorSamples/F7/Utilities
	${BSP_ROOT}/Utilities/Log
	${BSP_ROOT}/STM32F7xxxx/CMSIS_HAL/Include
	${BSP_ROOT}/STM32F7xxxx/CMSIS_HAL/Include)

bsp_compile_definitions(USE_HAL_DRIVER
	USE_STM32746G_DISCOVERY
	STM32F746xx)

add_bsp_based_executable(
	NAME NetworkPacketTracingDemo
	LINKER_SCRIPT STM32F746NGHx_FLASH.ld
	SOURCES
		readme.txt
		syscalls.c
		Src/app_ethernet.c
		Src/ethernetif.c
		Src/httpserver-netconn.c
		Src/main.c
		Src/stm32f7xx_hal_timebase_tim.c
		Src/stm32f7xx_it.c
		Src/system_stm32f7xx.c
		Inc/app_ethernet.h
		Inc/ethernetif.h
		Inc/FreeRTOSConfig.h
		Inc/httpserver-netconn.h
		Inc/lcd_log_conf.h
		Inc/lwipopts.h
		Inc/main.h
		Inc/stm32f7xx_hal_conf.h
		Inc/stm32f7xx_it.h
		${BSP_ROOT}/Utilities/Log/lcd_log.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma2d.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_eth.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_ltdc.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sdram.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_uart.c
		${BSP_ROOT}/STM32F7xxxx/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_fmc.c
		${BSP_ROOT}/FreeRTOS/Source/croutine.c
		${BSP_ROOT}/FreeRTOS/Source/list.c
		${BSP_ROOT}/FreeRTOS/Source/queue.c
		${BSP_ROOT}/FreeRTOS/Source/tasks.c
		${BSP_ROOT}/FreeRTOS/Source/timers.c
		${BSP_ROOT}/STM32F7xxxx/BSP/Components/lan8742/lan8742.c
		${BSP_ROOT}/STM32F7xxxx/BSP/STM32746G-Discovery/stm32746g_discovery.c
		${BSP_ROOT}/STM32F7xxxx/BSP/STM32746G-Discovery/stm32746g_discovery_lcd.c
		${BSP_ROOT}/STM32F7xxxx/BSP/STM32746G-Discovery/stm32746g_discovery_sdram.c
		${BSP_ROOT}/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
		${BSP_ROOT}/FreeRTOS/Source/portable/MemMang/heap_4.c
		${BSP_ROOT}/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/api/api_lib.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/api/api_msg.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/api/err.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/api/netbuf.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/api/netdb.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/api/netifapi.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/api/sockets.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/api/tcpip.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/def.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/dns.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/inet_chksum.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/init.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/ip.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/mem.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/memp.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/netif.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/pbuf.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/raw.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/stats.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/sys.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/tcp.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/tcp_in.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/tcp_out.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/timeouts.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/udp.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/netif/ethernet.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/system/OS/sys_arch.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/apps/http/fs.c
		${BSP_ROOT}/VendorSamples/F7/Middlewares/Third_Party/LwIP/src/apps/http/httpd.c
	GENERATE_BIN
	GENERATE_MAP
	OUTPUT_RELOCATION_RECORDS)
