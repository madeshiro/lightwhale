/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "board.h"

#include <stm32wlxx_hal.h>
#include <stm32wlxx_nucleo.h>

#include "stm-nucleo-wl55jc-defs.h"

struct nucleo_board_register Register = {0};

static struct nucleo_board_logs
{
    char    entries[WL55JC1_NUCLEO_SERIAL_LOG_LINE_LENGTH][WL55JC1_BOARD_LOGS_ENTRY_SIZE];
    uint8_t entries_count;

    // cursor
    uint8_t cur, next;
} Logs = {0};

// HMI update method
static void (*Nucleo_UpdateInterfaceFn)(void);

/**
 * Init uart
 */
void Nucleo_InitUart();

void Nucleo_InitBoard()
{
    Logs.entries_count = 0;
    Logs.cur = 0;
    Logs.next = 0;

    Register = (struct nucleo_board_register) {
        .id         = WL55JC1_NUCLEO_SERIAL_ID,
        .version    = 1,

        // lp address
        .lp = (lp_address_t) {.bytes = {65, 1, 1, 1}},

        // FSM state
        .state = k_wl55jc1_lrhtcp_state_idle,
        .logs_queue = 0,

        // current lrhtcp socket, default invalid socket
        //.socket = {0}
    };

    Nucleo_UpdateInterfaceFn = Nucleo_Interface_UpdateUART;

    // Board ready to receive instruction
    Nucleo_InitUart();
}

bool board_log_has_entry(void)
{
    return (Logs.next+1) % WL55JC1_BOARD_LOGS_ENTRY_SIZE != Logs.cur;
}

char* board_log_new_entry(void)
{
    char* entry =  Logs.entries[Logs.next];
    Logs.next = (Logs.next+1) % WL55JC1_BOARD_LOGS_ENTRY_SIZE;

    if (!board_log_has_entry())
    {
        Logs.cur = (Logs.cur+1) % WL55JC1_BOARD_LOGS_ENTRY_SIZE;
    }

    return entry;
}

char* board_log_pop(void)
{
    if (Logs.cur == Logs.next)
    {
        return NULL;
    }

    char* entry = Logs.entries[Logs.cur];
    Logs.cur = (Logs.cur+1) % WL55JC1_BOARD_LOGS_ENTRY_SIZE;
    return entry;
}

//
// Board
//
void Nucleo_Interface_UpdateSync()
{
    static uint32_t tick = 0;

    if (HAL_GetTick() - tick > 1000)
    {
        BSP_LED_Toggle(LED_BLUE);
        tick = HAL_GetTick();
    }
}

void Nucleo_Interface_UpdateLoRa(){/*TODO*/}

void Nucleo_SetInterfaceMode(uint8_t mode)
{
    Nucleo_Interface_Reset();
    //
    switch (mode)
    {
        case NUCLEO_INTERFACE_MODE_SYNC:
            Nucleo_UpdateInterfaceFn = Nucleo_Interface_UpdateSync;
            break;
        case NUCLEO_INTERFACE_MODE_UART:
            Nucleo_UpdateInterfaceFn = Nucleo_Interface_UpdateUART;
            break;
        case NUCLEO_INTERFACE_MODE_LORA:
            Nucleo_UpdateInterfaceFn = Nucleo_Interface_UpdateLoRa;
            break;
        default:
            break;
    }
}

void Nucleo_Interface_Reset()
{
    BSP_LED_Off(LED_BLUE);
    BSP_LED_Off(LED_GREEN);
    BSP_LED_Off(LED_RED);
}

void Nucleo_UpdateInterface()
{
    Nucleo_UpdateInterfaceFn();
}
