//------------------------------------------------------------------------------
//
// File:        wl55jc1_nucleo_driver-defs.h
// Description: Definitions of wl55jc1 (stm32 nucleo) driver structures.
//
//------------------------------------------------------------------------------
//
// File generated on feb 2025 by Rin Baudelet
//------------------------------------------------------------------------------

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _driver_wl55jc1_nucleo_serial_defs_h_
#define _driver_wl55jc1_nucleo_serial_defs_h_

// C standard lib
#include <stdint.h>

#define driver_log(msg, ...) printf("[driver:wl55jc1.l%03d] " msg "\n", __LINE__, \
    ##__VA_ARGS__)

#define __serial_packet__ __attribute__((__packed__, aligned(1)))

struct wl55jc1_nucleo_serial_header
{
    uint8_t actionId;
} __serial_packet__;

struct wl55jc1_nucleo_serial_register_header
{
    struct wl55jc1_nucleo_serial_header actionId;
    uint8_t registerId;
    uint8_t length;
} __serial_packet__;

#endif // _driver_wl55jc1_nucleo_serial_defs_h_
