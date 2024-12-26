################################################################################
#                                                                              #
# File: CMakeLists.txt                                                         #
# CMakeLists to generate and build lwcore                                      #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Nov 2024                                                               #
#                                                                              #
################################################################################
cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/version.txt CURRENT_PROJECT_VERSION)
project(lwcore VERSION ${CURRENT_PROJECT_VERSION} LANGUAGES C CXX)

if ("$ENV{LIGHTWHALE_PROJECT_PATH}" STREQUAL "" AND "${LIGHTWHALE_PROJECT_ROOT}" STREQUAL "")
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
option(${PROJECT_UNAME}_USE_MEMORY_MANAGER "Enable usage of a pool memory" ON)
option(${PROJECT_UNAME}_EMBEDDED_ENVIRONMENT "Enable embedded environment for lightweight features" OFF)

option(MEMORY_TRACE_MEMORYLEAKS "For debug purpose only! Trace every alloc/dealloc to
    monitore memory and easily detect memory leaks" OFF)

set(MEMORY_ALLOCATION_SIZE_T 32 CACHE STRING "Define memory allocation type's width")

########################################
#####                ###################
# // PROJECT FILES \\ ##################
#####                ###################
########################################

set(${PROJECT_UNAME}_HEADERS
    include/lwcore/${PROJECT_LNAME}-defs.h
    include/lwcore/Session.h
    include/lwcore/memory.h
    include/lwcore/architecture.h
    include/lwcore/RuntimeException.h
)

set(${PROJECT_UNAME}_SOURCES
    src/Session.cpp
    src/memory/MemoryManager.cpp
    src/memory/Chunk.cpp
    src/memory/newdelete.cpp
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
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (${${PROJECT_UNAME}_EMBEDDED_ENVIRONMENT})
    target_compile_definitions(${PROJECT_NAME} PUBLIC "LWCORE_EMBEDDED")
endif()

if (${${PROJECT_UNAME}_USE_MEMORY_MANAGER})
    # Setting up the memory manager traits

    target_compile_definitions(${PROJECT_NAME} PUBLIC
        "LWCORE_USE_MEMORY_MANAGER"
        "LWCORE_MEMORY_SIZE_T=${MEMORY_ALLOCATION_SIZE_T}"
    )

    if (MEMORY_TRACE_MEMORYLEAKS)
        target_compile_definitions(${PROJECT_NAME} PUBLIC
            LWCORE_MEMORY_TRACE_LEAKS
        )
    endif()
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC
        LWCORE_DEBUG
    )
endif ()