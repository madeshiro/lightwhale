//------------------------------------------------------------------------------
//
// File:        Board.h
// Description: Definitions of the nucleo board features
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
#ifndef BOARD_H
#define BOARD_H
// local
#include "stm-nucleo-wl55jc-defs.h"
#include "HMI.h"

//
// Forward declarations
//


/**
 * @details Singleton
 */
class Board
{
public:
    //
    // STATIC FUNCTIONS
    //

    static void Init();
    static void DeInit();

    /**
     *
     * @return the Board instance.
     */
    static Board* GetInstance();

    /**
     *
     * @return
     */
    static HMI* GetHMI();

    //
    // Constructor/Destructor
    //

    Board();
    ~Board();

private:
    static /*lw::core_ptr<Board>*/ Board* _Instance;

    HMI* _pHmi;
};

#endif // BOARD_H