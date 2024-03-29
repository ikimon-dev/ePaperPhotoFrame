cmake_minimum_required(VERSION 3.12)

set(ENV{PICO_SDK_PATH} "${CMAKE_SOURCE_DIR}/thirdparty/pico-sdk")
set(ENV{PICO_EXTRAS_PATH} "${CMAKE_SOURCE_DIR}/thirdparty/pico-extras")

message(STATUS "*** dump start cmake variables ***")
get_cmake_property(_variableNames VARIABLES)

foreach(_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

message(STATUS "*** dump end ***")

# Pull in SDK (must be before project)
include(./thirdparty/pico-sdk/external/pico_sdk_import.cmake)

# Pull in extras (must be before project)
include(./thirdparty/pico-extras/external/pico_extras_import.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# set(C)

set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/bin")

if(PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
        message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_compile_options(
        -O0
        -g
        -Wall
        -Wno-format # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
)

add_subdirectory(src/epaperdisplay_sample)
