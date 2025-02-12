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
#ifndef _nucleo_board_h_
#define _nucleo_board_h_

// Local
#include "board.h"

// Driver (API)
#include "wl55jc1_nucleo_driver.h"

// C standard lib
#include <stdbool.h>

struct nucleo_board_register
{
    // read-only device info
    uint8_t id;
    uint8_t version;

    // r/w#
    lp_address_t lp;

    // ro
    uint8_t state;
    uint8_t logs_queue;

    // r/w#
    LwpSocket socket;
} __attribute__ ((packed, aligned(4)));

/**
 * List of board's registers
 */
extern struct nucleo_board_register Register;

#define stlink_log(msg, ...) \
    printf("[device:wl55jc1:%s:%03d] " msg "\n", __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#define board_log(msg, ...) \
    snprintf(board_log_new_entry(), WL55JC1_NUCLEO_SERIAL_LOG_LINE_LENGTH-1, \
             "[device:wl55jc1:%s:%03d] " msg, __FILE_NAME__, __LINE__, ##__VA_ARGS__); \
    stlink_log(msg, ##__VA_ARGS__)

void Nucleo_InitBoard();

//
// LOGS
//

bool board_log_has_entry(void);
char* board_log_new_entry(void);
char* board_log_pop(void);

//
// HMI
//

void Nucleo_Interface_Reset();
void Nucleo_Interface_UpdateSync();
void Nucleo_Interface_UpdateLoRa();
void Nucleo_Interface_UpdateUART();

//
// Board
//

void Nucleo_SetInterfaceMode(uint8_t mode);
void Nucleo_UpdateInterface();

#endif // _nucleo_board_h_
