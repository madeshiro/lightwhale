################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build middlewares third-party                     #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Nov 2024                                                               #
#                                                                              #
################################################################################
cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(stm32wlxx_middlewares LANGUAGES ASM C)

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

# Project's name UPPERCASE
string(TOUPPER "${PROJECT_NAME}" PROJECT_UNAME)
string(TOLOWER "${PROJECT_NAME}" PROJECT_LNAME)

set(STM32_MIDDLEWARES_PATH $ENV{STM32_CUBE_WL}/Middlewares/Third_Party)


########################################
#####                     ##############
# // THIRD PARTY: FatFS \\ #############
#####                     ##############
########################################

option(STM32_3P_FATFS_ENABLED "Enable STM32WLxx Fat file-system third-party" OFF)
set(FatFS STM32WLxx_FatFS)
set(FatFS_PATH ${STM32_MIDDLEWARES_PATH}/FatFs/src)

set (FatFS_SOURCES
    ${FatFS_PATH}/ff.c
    ${FatFS_PATH}/ff_gen_driv.c
    ${FatFS_PATH}/diskio.c
    ${FatFS_PATH}/option/cc932.c
    ${FatFS_PATH}/option/cc936.c
    ${FatFS_PATH}/option/cc949.c
    ${FatFS_PATH}/option/cc950.c
    ${FatFS_PATH}/option/ccsbcs.c
    ${FatFS_PATH}/option/syscall.c
    ${FatFS_PATH}/option/unicode.c
)

add_library (${FatFS} INTERFACE)
target_include_directories(${FatFS} INTERFACE ${FatFS_PATH})
target_sources(${FatFS} INTERFACE ${FatFS_SOURCES})

########################################
#####                      #############
# // THIRD PARTY: SubGHz \\ ############
#####                      #############
########################################

option(STM32_3P_SUBGHZ_ENABLED "Enable STM32WLxx SubGHz_Phy (radio) third-party" ON)
set(SubGHz STM32WLxx_SubGHz)
set(SubGHz_PATH ${STM32_MIDDLEWARES_PATH}/SubGHz_Phy)

set (SubGHz_SOURCES
    ${SubGHz_PATH}/stm32_radio_driver/radio.c
    ${SubGHz_PATH}/stm32_radio_driver/radio_driver.c
    ${SubGHz_PATH}/stm32_radio_driver/radio_fw.c
    ${SubGHz_PATH}/stm32_radio_driver/wl_lr_fhss.c
    ${SubGHz_PATH}/stm32_radio_driver/lr_fhss_mac.c
)

add_library (${SubGHz} INTERFACE)
target_include_directories(${SubGHz} INTERFACE ${SubGHz_PATH})
target_sources(${SubGHz} INTERFACE ${SubGHz_SOURCES})

########################################
#####                ###################
# // SET-UP TARGET \\ ##################
#####                ###################
########################################

set(${PROJECT_UNAME}_LIBS
)

if ( ${STM32_3P_SUBGHZ_ENABLED} )
    list(APPEND ${PROJECT_UNAME}_LIBS ${SubGHz})
    message(STATUS "${BClrG}✓ ${ClrRst}Enable STM32wlxx SubGHz middleware")
endif()
##
if ( ${STM32_3P_FATFS_ENABLED} )
    list(APPEND ${PROJECT_UNAME}_LIBS ${FatFS})
    message(STATUS "${BClrG}✓ ${ClrRst}Enable STM32wlxx FatFs middleware")
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE ${${PROJECT_UNAME}_LIBS})
