//------------------------------------------------------------------------------
//
// File:        lwp_utils.h
// Description: (LightWhale Protocol) definitions of util methods
//
//------------------------------------------------------------------------------
//
// File generated on jan 2025 by Rin Baudelet
//------------------------------------------------------------------------------

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _lwp_utils_h_
#define _lwp_utils_h_

// Local
#include "lrhtcp/lrhtcp-defs.h"

// C standard lib
#include "stdint.h"

/**
 *
 * @return If the current host system is in big-endian or little-endian.
 */
uint8_t lwp_get_endianness(void);

//
// Integer universal Big Endian (network side) <> local int encoding
//

/**
 *
 * @param netword
 * @return
 */
uint16_t lwp_ntohs(uint16_t netword);
/**
 *
 * @param hostword
 * @return
 */
uint16_t lwp_htons(uint16_t hostword) __asm__("lwp_ntohs");

/**
 *
 * @param netdword
 * @return
 */
uint32_t lwp_ntohl(uint32_t netdword);
/**
 *
 * @param hostdword
 * @return
 */
uint32_t lwp_htonl(uint32_t hostdword) __asm__("lwp_ntohl");


#endif // _lwp_utils_h_