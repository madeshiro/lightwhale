################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build STM32CubeWL (external dependencies)         #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Nov 2024                                                               #
#                                                                              #
################################################################################
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(stm32wlxx_hal LANGUAGES C)

if ("$ENV{LIGHTWHALE_PROJECT_PATH}" STREQUAL "" AND "${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    message(FATAL_ERROR "Environment variable 'LIGHTWHALE_PROJECT_PATH' not set...")
elseif("${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    set(LIGHTWHALE_PROJECT_ROOT "$ENV{LIGHTWHALE_PROJECT_PATH}")
endif()

if ("ENV{STM32_CUBE_WL}" STREQUAL "")
    message(FATAL_ERROR "Missing STM32CubeWL location (ENV:STM32_CUBE_WL)")
endif()

# generic set-up for every cmake file
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/setup.cmake)   # setup
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/use_arm.cmake) # force ARM-GCC compiler

# Setup MCU
set(DEVICE_FAMILY   STM32WL  CACHE STRING "Specify the STM32 device family")
set(DEVICE_VERSION  55xx     CACHE STRING "Specify the STM32 device version")
set(DEVICE_ARCH     armv7e-m CACHE STRING "Specify the MCU/MPU architecture")

message(STATUS "Targetted device is '${DEVICE_FAMILY}${DEVICE_VERSION}' with march='${DEVICE_ARCH}'")

# Project's name UPPERCASE
string(TOUPPER "${PROJECT_NAME}" PROJECT_UNAME)
string(TOLOWER "${PROJECT_NAME}" PROJECT_LNAME)

# Path to stm32wl hal driver
set(${PROJECT_LNAME}_DRIVER_PATH $ENV{STM32_CUBE_WL}/Drivers/STM32WLxx_HAL_Driver)
set(${PROJECT_LNAME}_DEVICE_PATH $ENV{STM32_CUBE_WL}/Drivers/CMSIS/Device/ST/STM32WLxx)

# Path to arm cortex core
set(${PROJECT_LNAME}_CORTEX_PATH $ENV{STM32_CUBE_WL}/Drivers/CMSIS/Core)

########################################
#####                  #################
# // PROJECT OPTIONS \\ ################
#####                  #################
########################################

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=${DEVICE_ARCH}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=${DEVICE_ARCH}")

########################################
#####                ###################
# // PROJECT FILES \\ ##################
#####                ###################
########################################

# Get all C files from STM32WLxx_HAL_Driver/Src
file(GLOB ${PROJECT_UNAME}_SOURCES
    ${${PROJECT_LNAME}_DRIVER_PATH}/Src/*.c
)
message(DEBUG "${PROJECT_NAME}'s sources are\n${${PROJECT_UNAME}_SOURCES}")

# Get all H files from STM32WLxx_HAL_Driver/Inc
file(GLOB ${PROJECT_UNAME}_HEADERS
    # STM32CubeWL files
    ${${PROJECT_LNAME}_DRIVER_PATH}/Inc/*.h
    ${${PROJECT_LNAME}_DEVICE_PATH}/Include/*.h
    ${${PROJECT_LNAME}_CORTEX_PATH}/Include/*.h

    # Config files
    ${CMAKE_CURRENT_SOURCE_DIR}/conf/*.h
)
message(DEBUG "${PROJECT_NAME}'s headers are\n${${PROJECT_UNAME}_HEADERS}")

########################################
#####                ###################
# // SET-UP TARGET \\ ##################
#####                ###################
########################################

# create library (static '.a' file)
add_library(${PROJECT_NAME} STATIC ${${PROJECT_UNAME}_SOURCES}
                                    ${${PROJECT_UNAME}_HEADERS})

# include stm32 hal and drivers
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/conf

                                           # stm32 drivers
                                           PUBLIC ${${PROJECT_LNAME}_DRIVER_PATH}/Inc
                                           PUBLIC ${${PROJECT_LNAME}_DEVICE_PATH}/Include

                                           # arm core drivers
                                           PUBLIC ${${PROJECT_LNAME}_CORTEX_PATH}/Include)

# define stm32 device
target_compile_definitions(${PROJECT_NAME} PUBLIC ${DEVICE_FAMILY}${DEVICE_VERSION})
