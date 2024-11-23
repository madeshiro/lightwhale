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
#include "main.h"

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
extern "C" void Error_Handler(void)
{
    __disable_irq();
}

void BSP_PB_Callback(Button_TypeDef Button)
{
    if (BUTTON_SW3 == Button)
    {
        BSP_LED_Toggle(LED_BLUE);
    }
}

/**
 * Entry point of the embedded software
 */
extern "C" void st_main(void)
{
    // ReSharper disable once CppDFAEndlessLoop
    while (true)
    {
        BSP_LED_Toggle(LED_RED);
        HAL_Delay(1000);
    }

    // if reach here, an error occured!
}
