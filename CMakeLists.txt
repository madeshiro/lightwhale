################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build lightwhale binaries (all)                   #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Oct 2024                                                               #
#                                                                              #
################################################################################
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project(lightwhale_lora)

if ("$ENV{LIGHTWHALE_PROJECT_PATH}" STREQUAL "")
    set(LIGHTWHALE_PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
else()
    set(LIGHTWHALE_PROJECT_ROOT "$ENV{LIGHTWHALE_PROJECT_PATH}")
endif()
message(STATUS "Current project path is ${LIGHTWHALE_PROJECT_ROOT}")

# generic set-up for every cmake file
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/setup.cmake)
