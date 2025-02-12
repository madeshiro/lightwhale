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
// Local
#include "stm-nucleo-wl55jc-defs.h"
#include "board.h"

// Driver
#include "main.h"
#include "subghz_phy_app.h"

// lrhtcp
#include <lrhtcp/lrhtcp.h>

// libstdc
#include <stdbool.h>
#include <usart.h>

//
// HAL Handlers
//

void Error_Handler(void)
{
    BSP_LED_Toggle(LED_BLUE);
    BSP_LED_Toggle(LED_GREEN);
    BSP_LED_Toggle(LED_RED);

    __disable_irq();

    while (1) /*NOLINT*/;
}

//
// BSP Callback
//

void BSP_PB_Callback(Button_TypeDef Button)
{
    switch (Button)
    {
        case BUTTON_SW1: // Enable HMI mode 1 (Board connection synchronization witness)
            Nucleo_SetInterfaceMode(NUCLEO_INTERFACE_MODE_SYNC);
            break;
        case BUTTON_SW2: // Enable HMI mode 2 (LoRa COM)
            Nucleo_SetInterfaceMode(NUCLEO_INTERFACE_MODE_LORA);
            break;
        case BUTTON_SW3: // Enable HMI mode 3 (UART COM)
            Nucleo_SetInterfaceMode(NUCLEO_INTERFACE_MODE_UART);
            break;
        default:
            break;
    }
}

//
// Program
//

void st_main(void)
{
    printf("========================================\n");
    printf("Welcome to the Nucleo-64 wl55jc(1)!\n"
        "\t_ board version %d\n"
        "\t_ made by Rin Baudelet\n", WL55JC1_NUCLEO_SERIAL_VERSION
        );
    printf("========================================\n\n");

    Nucleo_InitBoard();

    uint32_t tick = HAL_GetTick();

    while (1) // NOLINT
    {
        uint32_t now = HAL_GetTick();

        if (now - tick > 10) /* 100Hz HMI update */
        {
            Nucleo_UpdateInterface();

            // update tick
            tick = HAL_GetTick();
        }

        // Call radio
    }

    Error_Handler(); // NOLINT: if program reach this point, an error occured
}