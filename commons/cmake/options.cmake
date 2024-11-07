################################################################################
#                                                                              #
# File: options.cmake                                                          #
# Include this file to add global options ####                                 #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Oct 2024                                                               #
#                                                                              #
################################################################################

option(GLOBAL_ENABLE_COVERAGE "Enable coverage options during compilation" ON)
option(GLOBAL_ENABLE_TESTING  "Enable unit testing with CTest"             ON)
option(GLOBAL_ENABLE_PACKING  "Enable packing manager CPack"               OFF)

if (WIN32)
    set(GLOBAL_REPORTS_PATH "%localappdata%\\tmp\\lightwhale" CACHE PATH "Specify a directory to store any test(s) report(s)")
else()
    set(GLOBAL_REPORTS_PATH "/tmp/lightwhale" CACHE PATH "Specify a directory to store any test(s) report(s)")
endif()
