cmake_minimum_required(VERSION 3.12)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(PicoSDKProfilerDemo C CXX ASM)
pico_sdk_init()

if(DEFINED SYSPROGS_FRAMEWORKS_FILE)
include(${SYSPROGS_FRAMEWORKS_FILE})
endif()
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
if (TARGET tinyusb_device)
    add_executable(hello_usb
            hello_usb.c
            )

    # pull in common dependencies
    target_link_libraries(hello_usb pico_stdlib Profiler)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(hello_usb 1)
    pico_enable_stdio_uart(hello_usb 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(hello_usb)

    # add url via pico_set_program_url
#     example_auto_set_url(hello_usb)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif()
