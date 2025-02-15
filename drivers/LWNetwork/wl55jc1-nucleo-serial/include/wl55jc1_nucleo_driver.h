//------------------------------------------------------------------------------
//
// File:        wl55jc1_nucleo_driver.h
// Description: Definition of wl55jc1 (stm32 nucleo) driver.
//
//------------------------------------------------------------------------------
//
// File generated on feb 2025 by Rin Baudelet
//------------------------------------------------------------------------------

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef _driver_wl55jc1_nucleo_serial_h_
#define _driver_wl55jc1_nucleo_serial_h_

// Local
#include "wl55jc1_nucleo_driver-defs.h"

// LR/HT Control Protocol
#include "lrhtcp/driver.h"

//
//> device properties
//

#define WL55JC1_NUCLEO_SERIAL_ID                0x8d     //< device id
#define WL55JC1_NUCLEO_SERIAL_VERSION           1        //< 2025/02 version 1
#define WL55JC1_NUCLEO_SERIAL_BAUDRATE          B57600   //< device serial baudrate
#define WL55JC1_NUCLEO_SERIAL_LOG_LINE_LENGTH   256      //< max line's length for logs

//
//> device actions
//

#define WL55JC1_ACTION_REGISTER_READ   0x00 //< read value from register
#define WL55JC1_ACTION_REGISTER_WRITE  0x01 //< write value to register

#define WL55JC1_ACTION_PING         0x02 //< ping frequency         (NIC)
#define WL55JC1_ACTION_BIND         0x03 //< bind to host           (NIC)
#define WL55JC1_ACTION_LISTEN       0x04 //< listen to frequency    (NIC)
#define WL55JC1_ACTION_ACCEPT       0x05 //< accept incoming user   (NIC)
#define WL55JC1_ACTION_SEND         0x06 //< send data              (NIC)
#define WL55JC1_ACTION_RECEIVE      0x07 //< receive data           (NIC)
#define WL55JC1_ACTION_CLOSE        0x08 //< close communication    (NIC)

#define WL55JC1_ACTION_INTERRUPT    0x09 //< interrupt action
#define WL55JC1_ACTION_CLEARFLAG    0x0a //< clear error flag
#define WL55JC1_ACTION_READLOG      0x0b //< read debug log (if available)

//
//> list of registers
//

typedef enum wl55jc1_nucleo_register
{
    // read-only device info
    k_wl55jc1_reg_id                    = 0x00, //< device id, (expected 0x8d)
    k_wl55jc1_reg_version               = 0x01, //< firmware version, (expected >= 1)

    // r/w#
    k_wl55jc1_reg_lrhtcp_lpaddr0        = 0x02, //< LP host addr, (default=0x41)  65
    k_wl55jc1_reg_lrhtcp_lpaddr1        = 0x03, //< LP host addr, (default=0x01) .1
    k_wl55jc1_reg_lrhtcp_lpaddr2        = 0x04, //< LP host addr, (default=0x01) .1
    k_wl55jc1_reg_lrhtcp_lpaddr3        = 0x05, //< LP host addr, (default=0x01) .1

    // ro
    k_wl55jc1_reg_lrhtcp_state          = 0x06, //< COM state,            (default=idle)
    k_wl55jc1_reg_logs_queue            = 0x07, //< logs entry available, (default=0)

    // r/w#
    k_wl55jc1_reg_lrhtcp_socket         = 0x08, //< socket, (default=0)
} wl55jc1_nucleo_register_t;

//
//> device fsm states
//

typedef enum wl55jc1_nucleo_state
{
    k_wl55jc1_lrhtcp_state_idle     = 0x00,
    k_wl55jc1_lrhtcp_state_tx       = 0x01,
    k_wl55jc1_lrhtcp_state_rx       = 0x02,
    k_wl55jc1_lrhtcp_state_txdone   = 0x03,
    k_wl55jc1_lrhtcp_state_rxdone   = 0x04,
    k_wl55jc1_lrhtcp_state_txerror  = 0x05,
    k_wl55jc1_lrhtcp_state_rxerror  = 0x06,
    k_wl55jc1_lrhtcp_state_busy     = 0x07,
} wl55jc1_nucleo_state_t;

/**
 * Device descriptor
 */
typedef struct wl55jc1_nucleo_serial_driver wl55jc1_nucleo_serial_driver_t, *pWL55JC1_Serial;

//
//> device HAL
//

/**
 *
 * @param serial_port
 * @return
 */
bool WL55JC1_Nucleo_Serial_Detect(const char* serial_port);

/**
 *
 * @param serial_port
 * @return
 */
pWL55JC1_Serial WL55JC1_Nucleo_Serial_Init(const char* serial_port);

/**
 *
 * @param serial
 * @return Previously used device handler. If not null, manage it by closing it or
 *         something else.
 */
pWL55JC1_Serial WL55JC1_Nucleo_SetCurrentDevice(pWL55JC1_Serial serial);

/**
 *
 * @return
 */
pWL55JC1_Serial WL55JC1_Nucleo_GetCurrentDevice(void);

/**
 *
 * @param fd
 */
void WL55JC1_Nucleo_Serial_Close(pWL55JC1_Serial fd);

/**
 *
 * @param fd
 * @param reg
 * @param value
 * @param len
 */
size_t WL55JC1_Nucleo_Serial_WriteRegister(pWL55JC1_Serial fd,
            wl55jc1_nucleo_register_t reg, const void* value, size_t len);

/**
 *
 * @param fd
 * @param reg
 * @param inValue
 * @param len
 * @return
 */
size_t WL55JC1_Nucleo_Serial_ReadRegister(pWL55JC1_Serial fd,
            wl55jc1_nucleo_register_t reg, void* inValue, size_t len);

/**
 *
 * @param fd
 * @param value
 * @param len
 * @return
 */
size_t WL55JC1_Nucleo_Serial_Read(pWL55JC1_Serial fd, void* value, size_t len);

/**
 *
 * @param fd
 * @param value
 * @param len
 * @return
 */
size_t WL55JC1_Nucleo_Serial_Write(pWL55JC1_Serial fd, const void* value, size_t len);

/**
 *
 * @param fd
 * @param socket
 * @return
 */
bool WL55JC1_Nucleo_Serial_SetSocket(pWL55JC1_Serial fd, pLwpSocket socket);

/**
 *
 * @param fd
 * @param socket
 * @return
 */
bool WL55JC1_Nucleo_Serial_GetSocket(pWL55JC1_Serial fd, pLwpSocket socket);

/**
 *
 * @param fd
 * @param inBuffer
 * @param logRemainingLines
 * @return Read line's length, or 0 if EOF reached. Return -1 if an error occurred.
 */
size_t WL55JC1_Nucleo_Serial_ReadLog(pWL55JC1_Serial fd, void* inBuffer, uint8_t* logRemainingLines);

#endif // _driver_wl55jc1_nucleo_serial_h_
