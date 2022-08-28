cmake_minimum_required(VERSION 3.12)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(FreeRTOSDemo C CXX ASM)
add_subdirectory("${PICO_SDK_PATH}/FreeRTOS/portable/ThirdParty/GCC/RP2040" FREERTOS_KERNEL)
pico_sdk_init()

SET(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS 1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(tcp_demo
        tcp_demo.c)

target_compile_definitions(tcp_demo PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        NO_SYS=0)
target_include_directories(tcp_demo PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../..)

target_link_libraries(tcp_demo
        pico_cyw43_arch_lwip_sys_freertos
        pico_stdlib
        pico_lwip_iperf
        FreeRTOS-Kernel-Heap4)

pico_add_extra_outputs(tcp_demo)
