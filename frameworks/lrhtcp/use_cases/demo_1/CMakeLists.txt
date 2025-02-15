################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build nucleo-wl55jc board's embedded software     #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Jan 2025                                                               #
#                                                                              #
################################################################################
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

##
## Prepare project
##

# get current project version
FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/version.txt CURRENT_PROJECT_VERSION)

if ("$ENV{LIGHTWHALE_PROJECT_PATH}" STREQUAL "" AND "${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    message(FATAL_ERROR "Environment variable 'LIGHTWHALE_PROJECT_PATH' not set...")
elseif("${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    set(LIGHTWHALE_PROJECT_ROOT "$ENV{LIGHTWHALE_PROJECT_PATH}")
endif()

##
## Setting-up the project
##
project(lrhtcp_demo_1 VERSION ${CURRENT_PROJECT_VERSION} LANGUAGES ASM C)

# Add colors to message
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/colorful.cmake)

message(STATUS "${BClrA}[${BClrR}framework${BClrA}] "
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

if (SERVER_SIDE)
    add_definitions(-DDEMO_SERVER_SIDE)
endif ()

########################################
#####                ###################
# // PROJECT FILES \\ ##################
#####                ###################
########################################

include_directories(include)

set(${PROJECT_UNAME}_HEADERS

)

set(${PROJECT_UNAME}_SOURCES
    src/main.c
)

########################################
#####                       ############
# // PROJECT DEPENDENCIES \\ ###########
#####                       ############
########################################

# generic set-up for every cmake file
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/setup.cmake)   # setup

# drivers/LWNetwork/wl55jc1-nucleo (serial) with lrhtcp
add_subdirectory(${LIGHTWHALE_PROJECT_ROOT}/drivers/LWNetwork/wl55jc1-nucleo-serial wl55jc1-nucleo-driver)

########################################
#####                ###################
# // SET-UP TARGET \\ ##################
#####                ###################
########################################

add_executable(${PROJECT_NAME} ${${PROJECT_UNAME}_SOURCES} ${${PROJECT_UNAME}_HEADERS})
target_link_libraries(${PROJECT_NAME}
    PRIVATE wl55jc1-nucleo-driver
    PRIVATE lrhtcp
)
