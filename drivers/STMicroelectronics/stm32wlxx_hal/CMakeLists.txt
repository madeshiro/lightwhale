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
project(stm32wlxx_hal LANGUAGES ASM C)

if ("$ENV{LIGHTWHALE_PROJECT_PATH}" STREQUAL "" AND "${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    message(FATAL_ERROR "Environment variable 'LIGHTWHALE_PROJECT_PATH' not set...")
elseif("${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    set(LIGHTWHALE_PROJECT_ROOT "$ENV{LIGHTWHALE_PROJECT_PATH}")
endif()

if ("$ENV{STM32_CUBE_WL}" STREQUAL "")
    message(FATAL_ERROR "Missing STM32CubeWL location (ENV:STM32_CUBE_WL)")
endif()

# generic set-up for every cmake file
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/setup.cmake)   # setup

# Setup MCU
set(DEVICE_FAMILY   STM32WL  CACHE STRING "Specify the STM32 device family")
set(DEVICE_VERSION  55xx     CACHE STRING "Specify the STM32 device version")
set(DEVICE_ARCH     armv7e-m CACHE STRING "Specify the MCU/MPU architecture")

message(STATUS "Targetted device is '${DEVICE_FAMILY}${DEVICE_VERSION}' with march='${DEVICE_ARCH}'")

# Project's name UPPERCASE
string(TOUPPER "${PROJECT_NAME}" PROJECT_UNAME)
string(TOLOWER "${PROJECT_NAME}" PROJECT_LNAME)

# Path to stm32wl hal driver
set(${PROJECT_UNAME}_DRIVER_PATH $ENV{STM32_CUBE_WL}/Drivers/STM32WLxx_HAL_Driver)
set(${PROJECT_UNAME}_DEVICE_PATH $ENV{STM32_CUBE_WL}/Drivers/CMSIS/Device/ST/STM32WLxx)

# Path to arm cortex core
set(${PROJECT_UNAME}_CORTEX_PATH $ENV{STM32_CUBE_WL}/Drivers/CMSIS/Core)

########################################
#####                  #################
# // PROJECT OPTIONS \\ ################
#####                  #################
########################################

########################################
#####                ###################
# // PROJECT FILES \\ ##################
#####                ###################
########################################

# Get all C files from STM32WLxx_HAL_Driver/Src
file(GLOB ${PROJECT_UNAME}_SOURCES
    ${${PROJECT_UNAME}_DRIVER_PATH}/Src/*.c
)
list(FILTER ${PROJECT_UNAME}_SOURCES EXCLUDE REGEX "(template)")
message(DEBUG "${PROJECT_NAME}'s sources are\n${${PROJECT_UNAME}_SOURCES}")

########################################
#####                ###################
# // SET-UP TARGET \\ ##################
#####                ###################
########################################

# create library (static '.a' file)
add_library(${PROJECT_NAME} INTERFACE
)
target_sources(${PROJECT_NAME} INTERFACE
    ${${PROJECT_UNAME}_SOURCES}
)
# include stm32 hal and drivers
target_include_directories(${PROJECT_NAME}
    # stm32 drivers
    INTERFACE ${${PROJECT_UNAME}_DRIVER_PATH}/Inc
    INTERFACE ${${PROJECT_UNAME}_DRIVER_PATH}/Inc/Legacy
    INTERFACE ${${PROJECT_UNAME}_DEVICE_PATH}/Include

    # arm core drivers
    INTERFACE ${${PROJECT_UNAME}_CORTEX_PATH}/Include
)

# define stm32 device
target_compile_definitions(${PROJECT_NAME}
    INTERFACE ${DEVICE_FAMILY}${DEVICE_VERSION}
    INTERFACE USE_FULL_LL_DRIVER # Enable Low Layer
)
