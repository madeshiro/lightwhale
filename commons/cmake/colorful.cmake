################################################################################
#                                                                              #
# File: colorful.cmake                                                         #
# Add color to your messages in CMake                                          #
#                                                                              #
# Author: Rin "madeshiro" Baudelet (https://madeshiro.dev)                     #
# Date: Nov 2024                                                               #
#                                                                              #
################################################################################

# Work only for non-windows cmake
if (NOT WIN32)
    string(ASCII 27 esc)

    # Standard modifiers
    set(ClrRst  "${esc}[m")  # Reset color
    set(BoldClr "${esc}[1m") # Bold text

    set(ClrK    "${esc}[30m") # blacK
    set(ClrR    "${esc}[31m") # Red
    set(ClrG    "${esc}[32m") # Green
    set(ClrY    "${esc}[33m") # Yellow
    set(ClrB    "${esc}[34m") # Blue
    set(ClrM    "${esc}[35m") # Magenta
    set(ClrC    "${esc}[36m") # Cyan
    set(ClrA    "${esc}[37m") # grAy
    set(ClrW    "${esc}[38m") # White

    set(BClrK   "${esc}[1;30m") # Bold blacK
    set(BClrR   "${esc}[1;31m") # Bold Red
    set(BClrG   "${esc}[1;32m") # Bold Green
    set(BClrY   "${esc}[1;33m") # Bold Yellow
    set(BClrB   "${esc}[1;34m") # Bold Blue
    set(BClrM   "${esc}[1;35m") # Bold Magenta
    set(BClrC   "${esc}[1;36m") # Bold Cyan
    set(BClrA   "${esc}[1;37m") # Bold grAy
    set(BClrW   "${esc}[1;38m") # Bold White
endif()

# Special chars

set(ChCheckOk   "${BClrG}✔${ClrRst}")
set(ChCheckFail "${BClrR}✗${ClrRst}")