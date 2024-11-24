################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build BSP                                         #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Nov 2024                                                               #
#                                                                              #
################################################################################
cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(stm32wlxx_bsp LANGUAGES ASM C)

if ("$ENV{LIGHTWHALE_PROJECT_PATH}" STREQUAL "" AND "${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    message(FATAL_ERROR "Environment variable 'LIGHTWHALE_PROJECT_PATH' not set...")
elseif("${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    set(LIGHTWHALE_PROJECT_ROOT "$ENV{LIGHTWHALE_PROJECT_PATH}")
endif()

if ("$ENV{STM32_CUBE_WL}" STREQUAL "")
    message(FATAL_ERROR "Missing STM32CubeWL location (ENV:STM32_CUBE_WL)")
endif()

# generic set-up for every cmake file
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/setup.cmake)

# Project's name UPPERCASE
string(TOUPPER "${PROJECT_NAME}" PROJECT_UNAME)
string(TOLOWER "${PROJECT_NAME}" PROJECT_LNAME)

set(STM32_BSP_PATH $ENV{STM32_CUBE_WL}/Drivers/BSP)


########################################
#####                        ###########
# // BSP: STM32WLxx Nucleo \\ ##########
#####                        ###########
########################################

option(STM32_BSP_NUCLEO_ENABLED "Enable STM32WLxx Nucleo BSP" ON)
set(NUCLEO STM32WLxx_Nucleo)
set(NUCLEO_PATH ${STM32_BSP_PATH}/STM32WLxx_Nucleo)

set (BSP_NUCLEO_SOURCES
    ${NUCLEO_PATH}/stm32wlxx_nucleo.c
    ${NUCLEO_PATH}/stm32wlxx_nucleo_radio.c
)

add_library (${NUCLEO} INTERFACE)
target_include_directories(${NUCLEO} INTERFACE ${NUCLEO_PATH})
target_sources(${NUCLEO} INTERFACE ${BSP_NUCLEO_SOURCES})
target_compile_definitions(${NUCLEO} INTERFACE USE_BSP_DRIVER)

########################################
#####                ###################
# // SET-UP TARGET \\ ##################
#####                ###################
########################################

set(${PROJECT_UNAME}_BSP
)

if ( ${STM32_BSP_NUCLEO_ENABLED} )
    list(APPEND ${PROJECT_UNAME}_BSP ${NUCLEO})
    message(STATUS "${BClrG}✓ ${ClrRst}Enable STM32wlxx Nucleo BSP")
endif()
##

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE  ${${PROJECT_UNAME}_BSP})
