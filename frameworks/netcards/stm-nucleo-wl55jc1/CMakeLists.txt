################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build nucleo-wl55jc board's embedded software     #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Nov 2024                                                               #
#                                                                              #
################################################################################
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/version.txt CURRENT_PROJECT_VERSION)
project(stm-nucleo-wl55jc VERSION ${CURRENT_PROJECT_VERSION} LANGUAGES ASM C CXX)

if ("$ENV{LIGHTWHALE_PROJECT_PATH}" STREQUAL "" AND "${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    message(FATAL_ERROR "Environment variable 'LIGHTWHALE_PROJECT_PATH' not set...")
elseif("${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    set(LIGHTWHALE_PROJECT_ROOT "$ENV{LIGHTWHALE_PROJECT_PATH}")
endif()

# Project's name UPPERCASE
string(TOUPPER "${PROJECT_NAME}" PROJECT_UNAME)
string(TOLOWER "${PROJECT_NAME}" PROJECT_LNAME)

# Depend on nucleo-board WL55JC1
add_subdirectory(${LIGHTWHALE_PROJECT_ROOT}/drivers/STMicroelectronics/nucleo-wl55jc1 board)

include_directories(include)

# generic set-up for every cmake file
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/setup.cmake)   # setup
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/arm-none-eabi-gcc.cmake)

########################################
#####                  #################
# // PROJECT OPTIONS \\ ################
#####                  #################
########################################

option(${PROJECT_UNAME}_ENABLE_TESTS "Enable unit testing for ${PROJECT_NAME}" OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=${DEVICE_ARCH}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=${DEVICE_ARCH}")

########################################
#####                ###################
# // PROJECT FILES \\ ##################
#####                ###################
########################################

set(${PROJECT_UNAME}_HEADERS
    include/${PROJECT_LNAME}-defs.h
)

set(${PROJECT_UNAME}_SOURCES
    src/main.cpp
)

########################################
#####                        ###########
# // PROJECT TESTING FILES \\ ##########
#####                        ###########
########################################

set(${PROJECT_UNAME}_TESTS

)

########################################
#####                ###################
# // SET-UP TARGET \\ ##################
#####                ###################
########################################

add_executable(${PROJECT_NAME} ${${PROJECT_UNAME}_SOURCES} ${${PROJECT_UNAME}_HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE nucleo-wl55jc1)


add_custom_target(${PROJECT_NAME}_bin
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)