################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build utilitaries                                 #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Nov 2024                                                               #
#                                                                              #
################################################################################
cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(stm32wlxx_utilitaries LANGUAGES ASM C)

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

set(STM32_UTILITARIES_PATH $ENV{STM32_CUBE_WL}/Utilities)
set(${PROJECT_UNAME}_UTILS
)


########################################
#####                    ###############
# // UTILITARIES: Misc \\ ##############
#####                    ###############
########################################

option(STM32_UTIL_MISC_ENABLE "Enable STM32WLxx Utils misc" ON)
set(Misc STM32WLxx_utils_misc)
set(Misc_PATH ${STM32_UTILITARIES_PATH}/misc)

set (Misc_SOURCES
    ${Misc_PATH}/stm32_mem.c
    ${Misc_PATH}/stm32_systime.c
    ${Misc_PATH}/stm32_tiny_sscanf.c
    ${Misc_PATH}/stm32_tiny_vsnprintf.c
)

add_library (${Misc} INTERFACE)
target_include_directories(${Misc} INTERFACE ${Misc_PATH})
target_sources(${Misc} INTERFACE ${Misc_SOURCES})
##
if ( ${STM32_UTIL_MISC_ENABLE} )
    list(APPEND ${PROJECT_UNAME}_UTILS ${Misc})
    message(STATUS "${BClrG}✓ ${ClrRst}Enable STM32wlxx misc utilitary")
endif()

########################################
#####                        ###########
# // UTILITARIES: tiny-lpm \\ ##########
#####                        ###########
########################################

option(STM32_UTIL_LPM_ENABLE "Enable STM32WLxx Utils tiny-lpm" ON)
set(TinyLPM STM32WLxx_utils_lpm)
set(TinyLPM_PATH ${STM32_UTILITARIES_PATH}/lpm/tiny_lpm)

set (TinyLPM_SOURCES
    ${TinyLPM_PATH}/stm32_lpm.c
)

add_library (${TinyLPM} INTERFACE)
target_include_directories(${TinyLPM} INTERFACE ${TinyLPM_PATH})
target_sources(${TinyLPM} INTERFACE ${TinyLPM_SOURCES})
##
if ( ${STM32_UTIL_LPM_ENABLE} )
    list(APPEND ${PROJECT_UNAME}_UTILS ${TinyLPM})
    message(STATUS "${BClrG}✓ ${ClrRst}Enable STM32wlxx tiny-lpm utilitary")
endif()

########################################
#####                         ##########
# // UTILITARIES: Sequencer \\ #########
#####                         ##########
########################################

option(STM32_UTIL_SEQ_ENABLE "${BClrG}✓ ${ClrRst}Enable STM32WLxx Utils sequencer" ON)
set(Seq STM32WLxx_utils_seq)
set(Seq_PATH ${STM32_UTILITARIES_PATH}/sequencer)

set (Seq_SOURCES
    ${Seq_PATH}/stm32_seq.c
)

add_library (${Seq} INTERFACE)
target_include_directories(${Seq} INTERFACE ${Seq_PATH})
target_sources(${Seq} INTERFACE ${Seq_SOURCES})
##
if ( ${STM32_UTIL_SEQ_ENABLE} )
    list(APPEND ${PROJECT_UNAME}_UTILS ${Seq})
    message(STATUS "${BClrG}✓ ${ClrRst}Enable STM32wlxx sequencer utilitary")
endif()

########################################
#####                     ##############
# // UTILITARIES: Timer \\ #############
#####                     ##############
########################################

option(STM32_UTIL_TIMER_ENABLE "Enable STM32WLxx Utils timer" ON)
set(Timer STM32WLxx_utils_timer)
set(Timer_PATH ${STM32_UTILITARIES_PATH}/timer)

set (Timer_SOURCES
    ${Timer_PATH}/stm32_timer.c
)

add_library (${Timer} INTERFACE)
target_include_directories(${Timer} INTERFACE ${Timer_PATH})
target_sources(${Timer} INTERFACE ${Timer_SOURCES})
##
if ( ${STM32_UTIL_TIMER_ENABLE} )
    list(APPEND ${PROJECT_UNAME}_UTILS ${Timer})
    message(STATUS "${BClrG}✓ ${ClrRst}Enable STM32wlxx timer utilitary")
endif()

########################################
#####                      #############
# // UTILITARIES: Tracer \\ ############
#####                      #############
########################################

option(STM32_UTIL_TRACER_ENABLE "Enable STM32WLxx Utils tracer" ON)
set(Tracer STM32WLxx_utils_tracer)
set(Tracer_PATH ${STM32_UTILITARIES_PATH}/trace/adv_trace)

set (Tracer_SOURCES
    ${Tracer_PATH}/stm32_adv_trace.c
)

add_library (${Tracer} INTERFACE)
target_include_directories(${Tracer} INTERFACE ${Tracer_PATH})
target_sources(${Tracer} INTERFACE ${Tracer_SOURCES})
##
if ( ${STM32_UTIL_TRACER_ENABLE} )
    list(APPEND ${PROJECT_UNAME}_UTILS ${Tracer})
    message(STATUS "${BClrG}✓ ${ClrRst}Enable STM32wlxx adv-tracer utilitary")
endif()

########################################
#####                ###################
# // SET-UP TARGET \\ ##################
#####                ###################
########################################

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE ${${PROJECT_UNAME}_UTILS})
