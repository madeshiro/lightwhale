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
#include "board.h"

// Driver
#include <main.h>
#include <usart.h>
#include <wl55jc1_nucleo_driver.h>

//
// Local variables
//

static uint8_t rxBuffer[1024];
// static char txBuffer[1024];

//
// Local UART methods :: actions
//

static void Nucleo_UART_ReadRegister(UART_HandleTypeDef* huart);
static void Nucleo_UART_WriteRegister(UART_HandleTypeDef* huart);

static void Nucleo_UART_ReadLog(UART_HandleTypeDef* huart);

//
// Typedefs
//

// Allow switching quickly between DMA, IT or Polling for test purposes
#define UART_Receive(_huart, _rxBuffer, _size) HAL_UART_Receive_DMA(_huart, _rxBuffer, _size)
#define UART_Transmit(_huart, _txBuffer, _size) HAL_UART_Transmit_DMA(_huart, _txBuffer, _size)

#define Nucleo_UART_RelaunchRx(hndl) \
    UartDevice.actionId = 0xff;\
    UartDevice.opState = k_uart_idle; \
    if (HAL_OK != UART_Receive(hndl, rxBuffer, sizeof(uint8_t))) Error_Handler()
#define Nucleo_UART_Rx(hndl, size) \
    if (HAL_OK != UART_Receive(hndl, rxBuffer, size)) Error_Handler()
#define Nucleo_UART_Tx(hndl, buffer, size)\
    if (HAL_OK != UART_Transmit(hndl, buffer, size)) Error_Handler()

/**
 * uart rx fsm state
 */
typedef enum uart_rx_state
{
    k_uart_idle,

    k_uart_register_read,
    k_uart_register_write,

    k_uart_read_log,
} uart_state_t;

struct uart_device
{
    // UART handler
    UART_HandleTypeDef* huart;

    // Current action performed
    uint8_t actionId;

    // Last requested read size
    uint32_t readCnt;
    uint8_t* rxCursor;

    // UART operation state
    enum uart_rx_state opState;
} UartDevice;

void Nucleo_Interface_UpdateUART()
{
    static uint32_t tick = 0;

    if (UartDevice.opState != k_uart_idle)
    {
        if (HAL_GetTick() - tick > 100)
        {
            BSP_LED_Toggle(LED_GREEN);
            tick = HAL_GetTick();
        }
    }
    else
    {
        BSP_LED_Off(LED_GREEN);
    }
}

void Nucleo_InitUart(void)
{
    UartDevice = (struct uart_device)
    {
        .huart = &huart1,
        .actionId = 0xff,
        .opState = k_uart_idle,
        .readCnt = 0,
        .rxCursor = 0,
    };

    // Prepare to receive instruction using DMA
    if (HAL_OK != UART_Receive(UartDevice.huart, rxBuffer, 1))
    {
        Error_Handler();
    }

    board_log("UART initialized");
}

void Nucleo_ResetUart(void)
{
    UartDevice.opState = k_uart_idle;
    UartDevice.actionId = 0xff;
    Nucleo_UART_RelaunchRx(UartDevice.huart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (UartDevice.actionId == 0xff)
    {
        UartDevice.actionId = rxBuffer[0];
        board_log("Incoming uart instruction (actionId=0x%02x)", UartDevice.actionId);
    }
    else if (UartDevice.readCnt != 0)
    {
        UartDevice.readCnt--;
        UART_Receive(UartDevice.huart, UartDevice.rxCursor, 1);
        UartDevice.rxCursor++;
    }

    UartDevice.rxCursor = 0;

    switch (UartDevice.actionId)
    {
        case WL55JC1_ACTION_REGISTER_READ:
            Nucleo_UART_ReadRegister(huart);
            break;

        case WL55JC1_ACTION_REGISTER_WRITE:
            Nucleo_UART_WriteRegister(huart);
            break;


        case WL55JC1_ACTION_READLOG:
            Nucleo_UART_ReadLog(huart);
            break;

        default:
            // Invalid state, force reset
            board_log("Unknown actionId 0x%02x, force reset on UART", UartDevice.actionId);
            Nucleo_ResetUart();
            break;
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}

//
// Implementations of UART actions
//

static void Nucleo_UART_ReadRegister(UART_HandleTypeDef* huart)
{
    // Convert register struct to buffer
    uint8_t* ptr = (uint8_t*)&Register;

    switch (UartDevice.opState)
    {
        case k_uart_idle:
            UartDevice.opState = k_uart_register_read;
            stlink_log("Start reading register...");

            // Read next bytes containing register id and read size
            Nucleo_UART_Rx(huart, 2);
            break;
        case k_uart_register_read:
            UartDevice.opState = k_uart_idle;
        {
            uint8_t registerId = rxBuffer[0];
            uint8_t readLen    = rxBuffer[1];

            stlink_log("Read %d byte(s) at offset 0x%02x", readLen, registerId);

            // Check if registerId is valid
            if (registerId <= k_wl55jc1_reg_lrhtcp_socket)
            {
                Nucleo_UART_Tx(huart, ptr + registerId, readLen);
            }
        }
            Nucleo_UART_RelaunchRx(huart);
            break;
        default:
            Nucleo_UART_RelaunchRx(huart);
            break;
    }
}

static void Nucleo_UART_WriteRegister(UART_HandleTypeDef* huart)
{
    // Convert register struct to buffer
    uint8_t* ptr = (uint8_t*)&Register;

    switch (UartDevice.opState)
    {
        case k_uart_idle:
            UartDevice.opState = k_uart_register_write;

            // Read next bytes containing register id and write size
            Nucleo_UART_Rx(huart, 2);
            break;
        case k_uart_register_write:
        {
            uint8_t registerId = rxBuffer[0];
            uint8_t writeLen   = rxBuffer[1];
            if (registerId <= k_wl55jc1_reg_lrhtcp_socket
                && writeLen <= sizeof(Register) - registerId)
            {
                HAL_UART_Receive_DMA(huart, ptr+registerId, writeLen);
            }
            else
            {
                // Receive dummy data
                HAL_UART_Receive_DMA(huart, rxBuffer, writeLen);
            }
        }
            Nucleo_UART_RelaunchRx(huart);
            break;
        default:
            Nucleo_UART_RelaunchRx(huart);
            break;
    }
}

static void Nucleo_UART_ReadLog(UART_HandleTypeDef*huart)
{
    UartDevice.opState = k_uart_read_log;
    if (board_log_has_entry())
    {
        const uint8_t* logEntry = (uint8_t*) board_log_pop();
        if (logEntry != NULL)
        {
            Nucleo_UART_Tx(huart, logEntry, WL55JC1_NUCLEO_SERIAL_LOG_LINE_LENGTH);
        }
    }

    Nucleo_UART_RelaunchRx(huart);
}
