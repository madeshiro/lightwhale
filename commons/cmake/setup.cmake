################################################################################
#                                                                              #
# File: setup.cmake                                                            #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Oct 2024                                                               #
#                                                                              #
################################################################################
if("${LIGHTWHALE_SETUP_CMAKE}" STREQUAL "")

    # Avoid double inclusion
    set(LIGHTWHALE_SETUP_CMAKE ON)

    # Global options
    include(${LIGHTWHALE_PROJECT_ROOT}/commons/cmake/options.cmake)

    if (${GLOBAL_ENABLE_COVERAGE})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")

        message(STATUS "Enable code coverage profiler")
    endif ()
endif()
