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
#include "UART.h"

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

    /**
     *
     * @return
     */
    static UART* GetCOM1();

    //
    // Rule of zero
    //
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    Board(Board&&) = delete;
    Board& operator=(Board&&) = delete;

    ~Board() = default;

    //
    // Constructor(s)
    //
    Board();

private:
    static /*lw::core_ptr<Board>*/ Board* _Instance;

    HMI     _hmi;
    UART    _uartCom1;
};

#endif // BOARD_H