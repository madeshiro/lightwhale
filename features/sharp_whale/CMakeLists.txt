################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build SharpWhale                                  #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Nov 2024                                                               #
#                                                                              #
################################################################################
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/version.txt CURRENT_PROJECT_VERSION)
project(sharp_whale VERSION ${CURRENT_PROJECT_VERSION} LANGUAGES C CXX)

if ("$ENV{LIGHTWHALE_PROJECT_PATH}" STREQUAL "" and "${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    message(FATAL_ERROR "Environment variable 'LIGHTWHALE_PROJECT_PATH' not set...")
elseif("${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
    set(LIGHTWHALE_PROJECT_ROOT "$ENV{LIGHTWHALE_PROJECT_PATH}")
endif()

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
    ${PROJECT_LNAME}-defs.h
)

set(${PROJECT_UNAME_}SOURCES

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

add_library(${PROJECT_NAME} ${${PROJECT_UNAME}_SOURCES} ${${PROJECT_UNAME}_HEADERS})
target_include_directories(${PROJECT_UNAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
