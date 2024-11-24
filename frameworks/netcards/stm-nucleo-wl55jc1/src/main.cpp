//------------------------------------------------------------------------------
//
// File:        main.cpp
// Description: Main of stm-nucleo-wl55jc
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
// Driver
#include "main.h"
// Local
#include "Board.h"

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
extern "C" void Error_Handler(void)
{
    __disable_irq();
}

/**
 * Entry point of the embedded software
 */
extern "C" void st_main(void)
{
    Board::Init();

    // ReSharper disable once CppDFAEndlessLoop
    while (true)
    {


        // Update HMI (10HZ = 100ms)
        if (HAL_GetTick() - Board::GetHMI()->getLastUpdateTime() >= 100)
        {
            Board::GetHMI()->update();
        }
    }

    // if reach here, an error occured!
}
