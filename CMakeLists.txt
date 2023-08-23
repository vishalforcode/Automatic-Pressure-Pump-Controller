cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

include($ENV{PICO_EXTRAS_PATH}/external/pico_extras_import.cmake)

project(automatic_water_pressure_pump C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

add_executable(pwm_automatic_water_pressure_pump_controller
        main.c
        )

# pull in common dependencies and additional pwm hardware support
target_link_libraries(pwm_automatic_water_pressure_pump_controller pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(pwm_automatic_water_pressure_pump_controller)

# enable usb output, disable uart output
pico_enable_stdio_uart(pwm_automatic_water_pressure_pump_controller 0)
pico_enable_stdio_usb(pwm_automatic_water_pressure_pump_controller 1)
