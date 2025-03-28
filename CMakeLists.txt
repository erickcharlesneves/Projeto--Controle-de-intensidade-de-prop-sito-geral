# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "pico_w")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Sistemas_Embarcados C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(
    Sistemas_Embarcados 

    display/src/ssd_1306_font.c
    display/src/ssd_1306.c
    
    joystick/src/joystick.c

    Sistemas_Embarcados.c 
)

pico_set_program_name(Sistemas_Embarcados "Sistemas_Embarcados")
pico_set_program_version(Sistemas_Embarcados "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Sistemas_Embarcados 0)
pico_enable_stdio_usb(Sistemas_Embarcados 0)

# Add the standard library to the build
target_link_libraries(Sistemas_Embarcados
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(
    Sistemas_Embarcados PRIVATE inc

    display/inc
    joystick/inc
)

# Add any user requested libraries
target_link_libraries(Sistemas_Embarcados 
        hardware_gpio
        hardware_adc
        hardware_pwm
        hardware_i2c
        pico_stdlib
        hardware_timer
)

pico_add_extra_outputs(Sistemas_Embarcados)

