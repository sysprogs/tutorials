
add_bsp_based_library(NAME DemoLibrary
	SOURCES DemoLibrary.cpp)

if (NOT SIMULATION)
set(MY_MACROS MYMACRO)
else()
set(MY_MACROS)
endif()

target_compile_definitions(DemoLibrary PRIVATE ${MY_MACROS})


target_link_libraries(DemoLibrary PRIVATE OSAL)
