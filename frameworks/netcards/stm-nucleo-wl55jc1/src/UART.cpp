/*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include "UART.h"
#include "Board.h"

UART::~UART()
{
    delete[] _pRxBuffer;
    delete[] _pTxBuffer;
}

uint32_t UART::getBaudRate() const
{
    return _pHuart->Init.BaudRate;
}

uint32_t UART::getParityMode() const
{
    return _pHuart->Init.Parity;
}

uint32_t UART::getStopBitMode() const
{
    return _pHuart->Init.StopBits;
}

//
// Tx
//

uint16_t UART::transmit(const char *data, uint16_t size) const
{
    HAL_UART_Transmit_DMA()
}


//
// Rx
//