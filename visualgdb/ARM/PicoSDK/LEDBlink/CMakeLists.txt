cmake_minimum_required(VERSION 3.12)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(LEDBlink C CXX ASM)
include(PicoSDKConfig.cmake)
pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_executable(blink
        blink.c
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(blink pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(blink)

# add url via pico_set_program_url
# example_auto_set_url(blink)
