//------------------------------------------------------------------------------
//
// File:        main.c
// Description: Main of stm-nucleo-wl55jc
//
//------------------------------------------------------------------------------
//
// File generated on Dec 2024 by Rin Baudelet
//------------------------------------------------------------------------------

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
// Board
#include "stm-nucleo-wl55jc-defs.h"
// Driver
#include "main.h"
// libstdc
#include <stdlib.h>
#include <stdbool.h>

typedef struct lw_session_st lw_session;
struct lw_session_st
{
    lw_session_state state;

};

volatile lw_session* session = NULL;

//
// HAL Handlers
//

void Error_Handler(void)
{
    BSP_LED_Toggle(LED_BLUE);
    BSP_LED_Toggle(LED_GREEN);
    BSP_LED_Toggle(LED_RED);

    __disable_irq();

    while (true) /*NOLINT*/;
}

//
// BSP Callback
//

void BSP_BP_Callback(Button_TypeDef Button)
{
    switch (Button)
    {
        case BUTTON_SW1:

            break;
        case BUTTON_SW2:

            break;
        case BUTTON_SW3:

            break;
        default:
            break;
    }
}

//
// BSP HMI      
//

void update_hmi(void)
{
}

//
// Program
//

void init()
{
    session = (lw_session*)malloc(sizeof(lw_session));
    session->state = k_lw_session_state_standalone;
}

void st_main(void)
{
    init();

    while (1) // NOLINT
    {

        update_hmi();
    }

    Error_Handler(); // NOLINT: if program reach this point, an error occured
}