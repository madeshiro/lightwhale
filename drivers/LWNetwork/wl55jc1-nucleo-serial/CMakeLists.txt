################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build the nucleo board WL55JC1 serial driver      #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Nov 2024                                                               #
#                                                                              #
################################################################################
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/version.txt CURRENT_PROJECT_VERSION)

if ("$ENV{LIGHTWHALE_PROJECT_PATH}" STREQUAL "" AND "${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    message(FATAL_ERROR "Environment variable 'LIGHTWHALE_PROJECT_PATH' not set...")
elseif("${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    set(LIGHTWHALE_PROJECT_ROOT "$ENV{LIGHTWHALE_PROJECT_PATH}")
endif()

project(wl55jc1-nucleo-driver VERSION ${CURRENT_PROJECT_VERSION} LANGUAGES C)

# Add colors to message
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/colorful.cmake)

message(STATUS "${BClrA}[${BClrR}driver${BClrA}] "
    "${BClrB}Start generating cmake files for ${BClrC}${PROJECT_NAME}"
    " ${ClrA}(${ClrW}version${ClrA}=${BClrR}${CURRENT_PROJECT_VERSION}${ClrA})${ClrRst}"
)

# Project's name UPPERCASE
string(TOUPPER "${PROJECT_NAME}" PROJECT_UNAME)
string(TOLOWER "${PROJECT_NAME}" PROJECT_LNAME)

########################################
#####                  #################
# // PROJECT OPTIONS \\ ################
#####                  #################
########################################

option(${PROJECT_UNAME}_ENABLE_TESTS "Enable unit testing for ${PROJECT_NAME}" OFF)
set(GLOBAL_ENABLE_COVERAGE OFF CACHE BOOL "Enable coverage options during compilation" FORCE)

########################################
#####                ###################
# // PROJECT FILES \\ ##################
#####                ###################
########################################

set(${PROJECT_UNAME}_HEADERS
    include/wl55jc1_nucleo_driver.h
    include/wl55jc1_nucleo_driver-defs.h
)

set(${PROJECT_UNAME}_SOURCES
    src/linux-any64/driver.c
)

########################################
#####                        ###########
# // PROJECT TESTING FILES \\ ##########
#####                        ###########
########################################

set(${PROJECT_UNAME}_TESTS

)

########################################
#####                       ############
# // PROJECT DEPENDENCIES \\ ###########
#####                       ############
########################################

# generic set-up for every cmake file
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/setup.cmake)   # setup

# features/lrhtcp
add_subdirectory(${LIGHTWHALE_PROJECT_ROOT}/features/lrhtcp lrhtcp)

########################################
#####                ###################
# // SET-UP TARGET \\ ##################
#####                ###################
########################################

add_library(${PROJECT_NAME} STATIC ${${PROJECT_UNAME}_SOURCES} ${${PROJECT_UNAME}_HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME}
    PUBLIC lrhtcp
)