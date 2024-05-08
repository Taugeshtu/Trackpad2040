cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(proto_mt)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# enable usb output, disable uart output

add_executable(proto_mt hello_tau.c)

pico_enable_stdio_usb(proto_mt 1)
pico_enable_stdio_uart(proto_mt 0)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(proto_mt pico_stdlib hardware_pwm hardware_adc)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(proto_mt)