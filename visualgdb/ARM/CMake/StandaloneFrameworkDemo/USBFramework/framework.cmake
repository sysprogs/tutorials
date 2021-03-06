if(TRUE)
    if(NOT DEFINED BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass)
        set(BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass "CDC")
        set(BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass "${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass}" PARENT_SCOPE)
    endif()

    if(NOT DEFINED BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.speed)
        set(BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.speed "FS")
        set(BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.speed "${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.speed}" PARENT_SCOPE)
    endif()

    set(_sources
        "${FRAMEWORK_ROOT}/Core/Src/usbd_core.c"
        "${FRAMEWORK_ROOT}/Core/Src/usbd_ctlreq.c"
        "${FRAMEWORK_ROOT}/Core/Src/usbd_ioreq.c"
        "${FRAMEWORK_ROOT}/Core/Inc/usbd_core.h"
        "${FRAMEWORK_ROOT}/Core/Inc/usbd_ctlreq.h"
        "${FRAMEWORK_ROOT}/Core/Inc/usbd_def.h"
        "${FRAMEWORK_ROOT}/Core/Inc/usbd_ioreq.h")
    if("${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass}" STREQUAL "AUDIO")
        set(_sources ${_sources} "${FRAMEWORK_ROOT}/Class/AUDIO/Src/usbd_audio.c" "${FRAMEWORK_ROOT}/Class/AUDIO/Inc/usbd_audio.h")
    endif()


    if("${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass}" STREQUAL "BillBoard")
        set(_sources ${_sources} "${FRAMEWORK_ROOT}/Class/BillBoard/Src/usbd_billboard.c" "${FRAMEWORK_ROOT}/Class/BillBoard/Inc/usbd_billboard.h")
    endif()


    if("${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass}" STREQUAL "CDC")
        set(_sources ${_sources} "${FRAMEWORK_ROOT}/Class/CDC/Src/usbd_cdc.c" "${FRAMEWORK_ROOT}/Class/CDC/Inc/usbd_cdc.h")
    endif()


    if("${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass}" STREQUAL "CDC_ECM")
        set(_sources ${_sources} "${FRAMEWORK_ROOT}/Class/CDC_ECM/Src/usbd_cdc_ecm.c" "${FRAMEWORK_ROOT}/Class/CDC_ECM/Inc/usbd_cdc_ecm.h")
    endif()


    if("${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass}" STREQUAL "CDC_RNDIS")
        set(_sources ${_sources} "${FRAMEWORK_ROOT}/Class/CDC_RNDIS/Src/usbd_cdc_rndis.c" "${FRAMEWORK_ROOT}/Class/CDC_RNDIS/Inc/usbd_cdc_rndis.h")
    endif()


    if("${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass}" STREQUAL "CustomHID")
        set(_sources ${_sources} "${FRAMEWORK_ROOT}/Class/CustomHID/Src/usbd_customhid.c" "${FRAMEWORK_ROOT}/Class/CustomHID/Inc/usbd_customhid.h")
    endif()


    if("${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass}" STREQUAL "DFU")
        set(_sources ${_sources} "${FRAMEWORK_ROOT}/Class/DFU/Src/usbd_dfu.c" "${FRAMEWORK_ROOT}/Class/DFU/Inc/usbd_dfu.h")
    endif()


    if("${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass}" STREQUAL "HID")
        set(_sources ${_sources} "${FRAMEWORK_ROOT}/Class/HID/Src/usbd_hid.c" "${FRAMEWORK_ROOT}/Class/HID/Inc/usbd_hid.h")
    endif()


    if("${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass}" STREQUAL "MSC")
        set(_sources ${_sources}
            "${FRAMEWORK_ROOT}/Class/MSC/Src/usbd_msc.c"
            "${FRAMEWORK_ROOT}/Class/MSC/Src/usbd_msc_bot.c"
            "${FRAMEWORK_ROOT}/Class/MSC/Src/usbd_msc_data.c"
            "${FRAMEWORK_ROOT}/Class/MSC/Src/usbd_msc_scsi.c"
            "${FRAMEWORK_ROOT}/Class/MSC/Inc/usbd_msc.h"
            "${FRAMEWORK_ROOT}/Class/MSC/Inc/usbd_msc_bot.h"
            "${FRAMEWORK_ROOT}/Class/MSC/Inc/usbd_msc_data.h"
            "${FRAMEWORK_ROOT}/Class/MSC/Inc/usbd_msc_scsi.h")
    endif()


    set(_includes "${FRAMEWORK_ROOT}/Core/Inc" "${FRAMEWORK_ROOT}/Class/${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.devclass}/Inc")
    set(_defines "USE_USB_${BSP_CONFIGURATION_com.sysprogs.bspoptions.stm32.usb.speed}")
    set(_forced_includes)
    create_bsp_framework_library_from_vars(StandaloneEFP "com.sysprogs.arm.stm32.usbdev.STM32F4" StandaloneEFP_STM32F4 "STM32 USB Device Library" "USB Device")
    set(_framework_libraries ${_framework_libraries} "${_created_library}")
endif()

set(_core_includes "${_core_includes}" PARENT_SCOPE)
set(_core_defines "${_core_defines}" PARENT_SCOPE)
set(_core_forced_includes "${_core_forced_includes}" PARENT_SCOPE)
set(_framework_libraries "${_framework_libraries}" PARENT_SCOPE)
