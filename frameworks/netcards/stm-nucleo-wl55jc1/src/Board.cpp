/*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include "Board.h"

//
// Static
//

void Board::Init()
{
    if (_Instance)
    {
        delete _Instance;
    }

    _Instance = new Board();
}

void Board::DeInit()
{
    delete _Instance;
    _Instance = nullptr;
}

Board *Board::GetInstance()
{
    return _Instance;
}

HMI* Board::GetHMI()
{
    return _Instance ? &_Instance->_hmi : nullptr;
}

UART *Board::GetCOM1()
{
    return _Instance ? &_Instance->_uartCom1 : nullptr;
}

Board * Board::_Instance = nullptr;

//
// Board class's functions
//

Board::Board()
    : _uartCom1(&hcom_uart[COM1])
{
}

//
// HAL UART CALLBACK
//

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *ipHuart)
{
    if (auto* pCom1 = Board::GetCOM1();
        pCom1 && pCom1->inAvailable() >= 8u
        )
    {
        // Receive 8 bytes in the Rx's buffer
        HAL_UART_Receive_DMA(ipHuart, pCom1->_pRxBufferPtr, 8u);

        // Increase Rx buffer's pointer
        pCom1->_pRxBufferPtr += 8u;

        // TODO com1 red led flash !
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}

void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}

void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}
