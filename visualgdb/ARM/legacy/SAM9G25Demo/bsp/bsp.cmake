set(_core_sources E:/SDKs/atmel-software-package/drivers/serial/usartd.c E:/SDKs/atmel-software-package/drivers/serial/usart.c E:/SDKs/atmel-software-package/drivers/serial/uart.c E:/SDKs/atmel-software-package/drivers/serial/dbgu.c E:/SDKs/atmel-software-package/drivers/serial/seriald.c E:/SDKs/atmel-software-package/drivers/serial/console.c E:/SDKs/atmel-software-package/drivers/peripherals/wdt.c E:/SDKs/atmel-software-package/drivers/peripherals/tcd.c E:/SDKs/atmel-software-package/drivers/peripherals/tc.c E:/SDKs/atmel-software-package/drivers/peripherals/slowclock_sckc.c E:/SDKs/atmel-software-package/drivers/peripherals/shdwc.c E:/SDKs/atmel-software-package/drivers/peripherals/rtc.c E:/SDKs/atmel-software-package/drivers/peripherals/rstc.c E:/SDKs/atmel-software-package/drivers/peripherals/pwmc.c E:/SDKs/atmel-software-package/drivers/peripherals/pmc.c E:/SDKs/atmel-software-package/drivers/peripherals/pit.c E:/SDKs/atmel-software-package/drivers/peripherals/matrix.c E:/SDKs/atmel-software-package/drivers/peripherals/bus.c E:/SDKs/atmel-software-package/drivers/mm/cache.c E:/SDKs/atmel-software-package/drivers/irq/irq.c E:/SDKs/atmel-software-package/drivers/irq/aic2.c E:/SDKs/atmel-software-package/drivers/gpio/pio3.c E:/SDKs/atmel-software-package/drivers/extram/smc.c E:/SDKs/atmel-software-package/drivers/extram/mpddrc.c E:/SDKs/atmel-software-package/drivers/extram/ddram.c E:/SDKs/atmel-software-package/drivers/dma/dmac.c E:/SDKs/atmel-software-package/drivers/dma/dma_dmac.c E:/SDKs/atmel-software-package/drivers/dma/dma.c E:/SDKs/atmel-software-package/target/sam9xx5/board_sam9xx5-ek.c E:/SDKs/atmel-software-package/target/sam9xx5/board_support.c E:/SDKs/atmel-software-package/target/sam9xx5/chip.c E:/SDKs/atmel-software-package/target/common/chip_common.c E:/SDKs/atmel-software-package/target/common/board_timer.c E:/SDKs/atmel-software-package/target/common/board_console.c E:/SDKs/atmel-software-package/utils/timer.c E:/SDKs/atmel-software-package/utils/syscalls.c E:/SDKs/atmel-software-package/utils/trace.c E:/SDKs/atmel-software-package/utils/rand.c E:/SDKs/atmel-software-package/utils/intmath.c E:/SDKs/atmel-software-package/utils/callback.c E:/SDKs/atmel-software-package/arch/arm/mmu_cp15.c E:/SDKs/atmel-software-package/arch/arm/l1cache_cp15.c E:/SDKs/atmel-software-package/arch/arm/fault_handlers.c E:/SDKs/atmel-software-package/arch/arm/mutex.c E:/SDKs/atmel-software-package/arch/arm/mutex.c E:/SDKs/atmel-software-package/arch/arm/fault_handlers.c E:/SDKs/atmel-software-package/arch/arm/l1cache_cp15.c E:/SDKs/atmel-software-package/arch/arm/mmu_cp15.c E:/SDKs/atmel-software-package/utils/callback.c E:/SDKs/atmel-software-package/utils/intmath.c E:/SDKs/atmel-software-package/utils/rand.c E:/SDKs/atmel-software-package/utils/trace.c E:/SDKs/atmel-software-package/utils/syscalls.c E:/SDKs/atmel-software-package/utils/timer.c E:/SDKs/atmel-software-package/target/common/board_console.c E:/SDKs/atmel-software-package/target/common/board_timer.c E:/SDKs/atmel-software-package/target/common/chip_common.c E:/SDKs/atmel-software-package/target/sam9xx5/chip.c E:/SDKs/atmel-software-package/target/sam9xx5/board_support.c E:/SDKs/atmel-software-package/target/sam9xx5/board_sam9xx5-ek.c E:/SDKs/atmel-software-package/drivers/dma/dma.c E:/SDKs/atmel-software-package/drivers/dma/dma_dmac.c E:/SDKs/atmel-software-package/drivers/dma/dmac.c E:/SDKs/atmel-software-package/drivers/extram/ddram.c E:/SDKs/atmel-software-package/drivers/extram/mpddrc.c E:/SDKs/atmel-software-package/drivers/extram/smc.c E:/SDKs/atmel-software-package/drivers/gpio/pio3.c E:/SDKs/atmel-software-package/drivers/irq/aic2.c E:/SDKs/atmel-software-package/drivers/irq/irq.c E:/SDKs/atmel-software-package/drivers/mm/cache.c E:/SDKs/atmel-software-package/drivers/peripherals/bus.c E:/SDKs/atmel-software-package/drivers/peripherals/matrix.c E:/SDKs/atmel-software-package/drivers/peripherals/pit.c E:/SDKs/atmel-software-package/drivers/peripherals/pmc.c E:/SDKs/atmel-software-package/drivers/peripherals/pwmc.c E:/SDKs/atmel-software-package/drivers/peripherals/rstc.c E:/SDKs/atmel-software-package/drivers/peripherals/rtc.c E:/SDKs/atmel-software-package/drivers/peripherals/shdwc.c E:/SDKs/atmel-software-package/drivers/peripherals/slowclock_sckc.c E:/SDKs/atmel-software-package/drivers/peripherals/tc.c E:/SDKs/atmel-software-package/drivers/peripherals/tcd.c E:/SDKs/atmel-software-package/drivers/peripherals/wdt.c E:/SDKs/atmel-software-package/drivers/serial/console.c E:/SDKs/atmel-software-package/drivers/serial/seriald.c E:/SDKs/atmel-software-package/drivers/serial/dbgu.c E:/SDKs/atmel-software-package/drivers/serial/uart.c E:/SDKs/atmel-software-package/drivers/serial/usart.c E:/SDKs/atmel-software-package/drivers/serial/usartd.c "")
set(_core_linker_script
	E:/SDKs/atmel-software-package/target/sam9xx5/toolchain/gnu/ddram.ld)

set(_core_includes
	E:/SDKs/atmel-software-package/arch
	E:/SDKs/atmel-software-package/utils
	E:/SDKs/atmel-software-package/target/common
	E:/SDKs/atmel-software-package/target/sam9xx5
	E:/SDKs/atmel-software-package/drivers
	E:/SDKs/atmel-software-package/lib
	"")

set(_core_defines
	printf=iprintf
	TRACE_LEVEL=5
	VARIANT_DDRAM
	CONFIG_ARCH_ARMV5TE
	""
	CONFIG_ARCH_ARM
	CONFIG_SOC_SAM9XX5
	CONFIG_CHIP_SAM9G25
	CONFIG_BOARD_SAM9G25_EK
	CONFIG_HAVE_AIC2
	CONFIG_HAVE_PIO3
	CONFIG_HAVE_PIT
	CONFIG_HAVE_SMC
	CONFIG_HAVE_MPDDRC
	CONFIG_HAVE_MPDDRC_SDRAM
	CONFIG_HAVE_MPDDRC_DDR2
	CONFIG_HAVE_ADC_LOW_RES
	CONFIG_HAVE_PMC_PLLADIV2
	CONFIG_HAVE_PMC_UPLL_BIAS
	CONFIG_HAVE_PMC_PERIPH_DIV
	CONFIG_HAVE_SCKC
	CONFIG_HAVE_DMAC
	CONFIG_HAVE_SMD
	CONFIG_HAVE_PWMC
	CONFIG_HAVE_DDR2_MT47H64M16
	CONFIG_HAVE_RSTC_EXTERNAL_RESET
	CONFIG_HAVE_RSTC_INDEPENDENT_RESET
	CONFIG_HAVE_SHDWC
	CONFIG_HAVE_MMU
	CONFIG_HAVE_L1CACHE
	CONFIG_HAVE_DBGU
	CONFIG_HAVE_SERIALD_DBGU
	CONFIG_HAVE_UART
	CONFIG_HAVE_USART
	CONFIG_HAVE_USART_SPI_MODE
	"SOFTPACK_VERSION=\"2.17_v2.17-175-ge0428c7c\"")

set(_core_commonflags
	-mcpu=arm926ej-s
	-mfloat-abi=soft)

set(_core_ID)

