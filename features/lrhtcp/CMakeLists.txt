################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build LR/HT control protocol                      #
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

project(lrhtcp VERSION ${CURRENT_PROJECT_VERSION} LANGUAGES C)

# Add colors to message
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/colorful.cmake)

message(STATUS "${BClrA}[${BClrR}feature${BClrA}] "
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

########################################
#####                ###################
# // PROJECT FILES \\ ##################
#####                ###################
########################################

set(${PROJECT_UNAME}_HEADERS
    include/lrhtcp/${PROJECT_LNAME}-defs.h
    include/lrhtcp/lwp_utils.h
    include/lrhtcp/lrhtcp.h
    include/lrhtcp/driver.h
    include/lrhtcp/com.h
)

set(${PROJECT_UNAME}_SOURCES
    src/lwp_packet.c
    src/lwp_utils.c
    src/lrhtcp.c

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

add_library(${PROJECT_NAME} INTERFACE ${${PROJECT_UNAME}_HEADERS}
)
target_sources(${PROJECT_NAME} INTERFACE
     ${${PROJECT_UNAME}_SOURCES}
)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
