
add_bsp_based_library(NAME OSAL
	SOURCES OSAL.cpp OSAL.h system_stm32f4xx.c stm32f4xx_hal_conf.h)

target_include_directories(OSAL PUBLIC .)
