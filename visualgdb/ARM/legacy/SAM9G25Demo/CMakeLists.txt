cmake_minimum_required(VERSION 3.15)

project(SAM9G25Demo LANGUAGES C CXX ASM)

find_bsp(
	SOURCE bsp
	DISABLE_GNU_EXTENSIONS)

add_bsp_based_executable(
	NAME SAM9G25Demo
	GENERATE_BIN
	GENERATE_MAP
	OUTPUT_RELOCATION_RECORDS
	SOURCES ../../../../../SDKs/atmel-software-package/examples/usart/main.c ../../../../../SDKs/atmel-software-package/target/sam9xx5/toolchain/gnu/cstartup.S)
