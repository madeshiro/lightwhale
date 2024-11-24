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

# force arm-none-eabi-gcc compiler
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/arm-none-eabi-gcc.cmake)

##
## Setting-up the project
##
project(stm-nucleo-wl55jc VERSION ${CURRENT_PROJECT_VERSION} LANGUAGES ASM C CXX)

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
set(GLOBAL_ENABLE_COVERAGE OFF CACHE BOOL "Enable coverage options during compilation" FORCE)

# Setup MCU
set(DEVICE_FAMILY   STM32WL  CACHE STRING "Specify the STM32 device family"  FORCE)
set(DEVICE_VERSION  55xx     CACHE STRING "Specify the STM32 device version" FORCE)
set(DEVICE_ARCH     armv7e-m CACHE STRING "Specify the MCU/MPU architecture" FORCE)

# Setup heap memory
set(HEAP_SIZE   "32"     # Size of the heap memory, unit specified by ${HEAP_UNIT}
    CACHE STRING "Specify the heap's size (cf. unit with HEAP_UNIT)" FORCE
)
set(HEAP_UNIT   "KB"     # KiloByte => ${HEAP_SIZE}*2^10. {B/KB/MB/GB}
    CACHE STRING "Heap size's unit (B, KB, MB or GB)" FORCE
)
set(HEAP_ALLOC  "STATIC" # STATIC/DYNAMIC => static = .bss section
    CACHE STRING "Heap allocation style (dynamic or static)" FORCE
)

########################################
#####                       ############
# // PROJECT DEPENDENCIES \\ ###########
#####                       ############
########################################

# generic set-up for every cmake file
include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/setup.cmake)   # setup

# Depend on nucleo-board WL55JC1
add_subdirectory(${LIGHTWHALE_PROJECT_ROOT}/drivers/STMicroelectronics/nucleo-wl55jc1 board)

# commons/lwcore
# add_subdirectory(${LIGHTWHALE_PROJECT_ROOT}/commons/modules/lwcore corelib)

########################################
#####                 ##################
# // COMPILER FLAGS \\ #################
#####                 ##################
########################################

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -fdata-sections -ffunction-sections")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g3")
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -g0")
endif()

set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=${DEVICE_ARCH}")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_C_LINK_FLAGS "${TARGET_FLAGS}")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T \"${DEVICE_LINKER_FILE}\"")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --specs=nano.specs")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lc -lm -Wl,--end-group")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--print-memory-usage")

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lstdc++ -lsupc++ -Wl,--end-group")

if (CMAKE_C_STANDARD LESS 11)
    message(ERROR "Required minimum standard c11")
endif()

########################################
#####                ###################
# // PROJECT FILES \\ ##################
#####                ###################
########################################

# headers directory
include_directories(include)

# headers (h/hpp/hxx)
set(${PROJECT_UNAME}_HEADERS
    include/${PROJECT_LNAME}-defs.h
    include/fsm.h
    include/HMI.h
    include/Board.h
)

# sources (c/cpp)
set(${PROJECT_UNAME}_SOURCES
    src/main.cpp
    src/HMI.cpp
    src/Board.cpp
    src/Board.cpp
)

########################################
#####                ###################
# // SET-UP TARGET \\ ##################
#####                ###################
########################################

add_executable(${PROJECT_NAME} ${${PROJECT_UNAME}_SOURCES} ${${PROJECT_UNAME}_HEADERS})
target_link_libraries(${PROJECT_NAME}
    PRIVATE nucleo-wl55jc1
    #PRIVATE lwcore
)

add_custom_target(${PROJECT_NAME}_bin
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)