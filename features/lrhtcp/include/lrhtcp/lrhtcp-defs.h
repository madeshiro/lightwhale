//------------------------------------------------------------------------------
//
// File:        lrhtcp-defs.h
// Description: Definitions of macros and typedefs for the lrhtcp library
//
//------------------------------------------------------------------------------
//
// File generated on Nov 2024 by Rin Baudelet
//------------------------------------------------------------------------------

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _lrhtcp_defs_h_
#define _lrhtcp_defs_h_

#define LRHTCP          202501010 //< 2025/01 0.1.0
#define LRHTCP_VERSION  "0.1.0"
#define LRHTCP_VERSION_MAJOR 0
#define LRHTCP_VERSION_MINOR 1
#define LRHTCP_VERSION_PATCH 0

#define LWP_ENDIANNESS_BIG  0
#define LWP_ENDIANNESS_LITTLE 1

#define __lrhtcp_packet__ __attribute__((__packed__, aligned(4)))
#define __lrhtcp_packet_byte__ __attribute__((__packed__, aligned(1)))

#endif // _lrhtcp_defs_h_