cmake_minimum_required(VERSION 3.12)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(PicoWDemo C CXX ASM)
include(PicoSDKConfig.cmake)
pico_sdk_init()

if(DEFINED SYSPROGS_FRAMEWORKS_FILE)
include(${SYSPROGS_FRAMEWORKS_FILE})
endif()
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_executable(picow_tcpip_server_background
        picow_tcp_server.c
        )
target_compile_definitions(picow_tcpip_server_background PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )
target_include_directories(picow_tcpip_server_background PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
        )
target_link_libraries(picow_tcpip_server_background
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        Profiler)

pico_add_extra_outputs(picow_tcpip_server_background)

add_executable(picow_tcpip_server_poll
        picow_tcp_server.c
        )
target_compile_definitions(picow_tcpip_server_poll PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )
target_include_directories(picow_tcpip_server_poll PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
        )
target_link_libraries(picow_tcpip_server_poll
        pico_cyw43_arch_lwip_poll
        pico_stdlib
        Profiler)
pico_add_extra_outputs(picow_tcpip_server_poll)
