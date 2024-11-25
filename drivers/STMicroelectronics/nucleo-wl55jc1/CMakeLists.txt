################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build the nucleo-wl55jc1 driver and third-parties #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Nov 2024                                                               #
#                                                                              #
################################################################################
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(nucleo-wl55jc1 LANGUAGES ASM C)

if ("$ENV{LIGHTWHALE_PROJECT_PATH}" STREQUAL "" AND "${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    message(FATAL_ERROR "Environment variable 'LIGHTWHALE_PROJECT_PATH' not set...")
elseif("${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    set(LIGHTWHALE_PROJECT_ROOT "$ENV{LIGHTWHALE_PROJECT_PATH}")
endif()

if ("$ENV{STM32_CUBE_WL}" STREQUAL "")
    message(FATAL_ERROR "Missing STM32CubeWL location (ENV:STM32_CUBE_WL)")
endif()

########################################
#####             ######################
# // SETTING-UP \\ #####################
#####             ######################
########################################

# generic set-up for every cmake file
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/setup.cmake)   # setup

# Setup MCU
set(DEVICE_FAMILY   STM32WL  CACHE STRING "Specify the STM32 device family")
set(DEVICE_VERSION  55xx     CACHE STRING "Specify the STM32 device version")
set(DEVICE_ARCH     armv7e-m CACHE STRING "Specify the MCU/MPU architecture")

# Setup LD
set(DEVICE_LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/STM32WL55XX_FLASH_CM4.ld PARENT_SCOPE)

# Project's name UPPERCASE
string(TOUPPER "${PROJECT_NAME}" PROJECT_UNAME)
string(TOLOWER "${PROJECT_NAME}" PROJECT_LNAME)

set(${PROJECT_UNAME}_THIRDPARTY_PATH  $ENV{STM32_CUBE_WL}/Middlewares/Third_Party)
set(${PROJECT_UNAME}_UTILITIES_PATH $ENV{STM32_CUBE_WL}/Utilities)

# Path to stm32 radio driver
set(${PROJECT_UNAME}_SUBGHZ_PATH ${${PROJECT_UNAME}_THIRDPARTY_PATH}/SubGHz_Phy)
set(${PROJECT_UNAME}_DRIVER_PATH ${${PROJECT_UNAME}_SUBGHZ_PATH}/stm32_radio_driver)

# utilities
set(${PROJECT_UNAME}_UTILS_TRACE_PATH ${${PROJECT_UNAME}_UTILITIES_PATH}/trace/adv_trace)
set(${PROJECT_UNAME}_UTILS_MISC_PATH ${${PROJECT_UNAME}_UTILITIES_PATH}/misc)
set(${PROJECT_UNAME}_UTILS_TIMER_PATH ${${PROJECT_UNAME}_UTILITIES_PATH}/timer)
set(${PROJECT_UNAME}_UTILS_LPM_PATH ${${PROJECT_UNAME}_UTILITIES_PATH}/lpm/tiny_lpm)

########################################
#####                   ################
# // PROJECT DEPENCENIES \\ ############
#####                    ###############
########################################

add_subdirectory(${LIGHTWHALE_PROJECT_ROOT}/drivers/STMicroelectronics/stm32wlxx_hal stm32wlxx_hal)
add_subdirectory(${LIGHTWHALE_PROJECT_ROOT}/drivers/STMicroelectronics/stm32wlxx_bsp stm32wlxx_bsp)
add_subdirectory(${LIGHTWHALE_PROJECT_ROOT}/drivers/STMicroelectronics/stm32wlxx_middlewares stm32wlxx_middlesware)
add_subdirectory(${LIGHTWHALE_PROJECT_ROOT}/drivers/STMicroelectronics/stm32wlxx_utilitaries stm32wlxx_utilitaries)

########################################
#####                ###################
# // PROJECT FILES \\ ##################
#####                ###################
########################################

# Sources
set(${PROJECT_UNAME}_SOURCES
    # Core
    Core/Src/aes.c
    Core/Src/dma.c
    Core/Src/gpio.c
    Core/Src/main.c
    Core/Src/rng.c
    Core/Src/stm32_adv_trace_if.c
    Core/Src/stm32_lpm_if.c
    Core/Src/stm32wlxx_hal_msp.c
    Core/Src/stm32wlxx_it.c
    Core/Src/subghz.c
    Core/Src/sys_app.c
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    Core/Src/system_stm32wlxx.c
    Core/Src/timer_if.c
    Core/Src/usart.c

    # SubGHz_Phy
    SubGHz_Phy/App/app_subghz_phy.c
    SubGHz_Phy/App/subghz_phy_app.c
    SubGHz_Phy/Target/radio_board_if.c

    # Entry-point
    startup_stm32wl55xx_cm4.s # ARM Cortex M4
)

########################################
#####                ###################
# // SET-UP TARGET \\ ##################
#####                ###################
########################################

# create library (static '.a' file)
add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} INTERFACE ${${PROJECT_UNAME}_SOURCES})
# include stm32 radio
target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/SubGHz_Phy/App
    ${CMAKE_CURRENT_SOURCE_DIR}/SubGHz_Phy/Target
)
target_link_libraries(${PROJECT_NAME}
    INTERFACE stm32wlxx_hal
    INTERFACE stm32wlxx_bsp
    INTERFACE stm32wlxx_middlewares
    INTERFACE stm32wlxx_utilitaries
)
