//------------------------------------------------------------------------------
//
// File:        fsm.h
// Description: Definitions of the nucleo's machine state.
//              Core feature of the nucleo's behavior
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
#ifndef _fsm_h_
#define _fsm_h_
#include <cstdint>

enum class NucleoState : uint8_t
{
    k_none = 0, // undefined state, lead to Error_Handler

    k_hmiSelection = 0x01, // select current HMI behavior (LED)
};


#endif // _fsm_h_
