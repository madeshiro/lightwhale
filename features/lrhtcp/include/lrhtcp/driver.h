//------------------------------------------------------------------------------
//
// File:        driver.h
// Description: (LightWhale Protocol) definition of driver structure.
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
#ifndef _lwp_device_driver_h_
#define _lwp_device_driver_h_

// Local
#include "lrhtcp/lrhtcp.h"

// C standard lib
#include <stdint.h>

/**
 *
 */
typedef struct lwp_device_driver_s
{
    /**
     *
     */
    size_t (*Ping)(pLwpSocket socket, lp_ping_result_t* addrBuffer, size_t maxSize);
    /**
     * Both net and p2p
     */
    bool (*Bind)(pLwpSocket socket);
    /**
     *
     */
    bool (*Listen)(pLwpSocket socket);
    /**
     *
     */
    bool (*Accept)(pLwpSocket socket);
    /**
     *
     */
    size_t (*Send)(pLwpSocket socket, const void* data, size_t size);
    /**
     *
     */
    size_t (*Receive)(pLwpSocket socket, void* data, size_t size);
    /**
     *
     */
    void (*Close)(pLwpSocket socket);
} LpDeviceDriver_s;

/**
 * Define by driver implementation in the OS.
 */
extern LpDeviceDriver_s LpDeviceDriver;

#endif // _lwp_device_driver_h_
