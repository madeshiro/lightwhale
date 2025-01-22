//------------------------------------------------------------------------------
//
// File:        stm_nucleo_wl55jc_defs.h
// Description: Definitions of macros and typedefs for stm-nucleo-wl55jc
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
#ifndef _stm_nucleo_wl55jc_defs_h_
#define _stm_nucleo_wl55jc_defs_h_
#include "inttypes.h"

typedef enum
{
    k_lw_session_state_undefined = 0,
    k_lw_session_state_connected = 1,
    k_lw_session_state_standalone = 2,
    k_lw_session_state_error = 3,
} lw_session_state;

typedef uint8_t lw_byte;

#endif // _stm_nucleo_wl55jc_defs_h_