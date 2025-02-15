/*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include "UART.h"
#include "Board.h"
#include "main.h"
// STL
#include <cstring>

UART::~UART()
{
    delete[] _pRxBufferBase;

    _pRxBufferBase = nullptr;
    _pRxBufferPtr  = nullptr;
    _pRxBufferEnd  = nullptr;
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

HAL_StatusTypeDef UART::transmit(const char *ipData) const
{
    // do standard UART transmit using uint8_t*
    return transmit(
        reinterpret_cast<const uint8_t*>(ipData),
        strnlen(ipData, UART_BUFFER_MAX_SIZE)
        );
}

HAL_StatusTypeDef UART::transmit(const uint8_t *ipData, uint16_t size) const
{
    if (size > 0)
    {
        return HAL_UART_Transmit_DMA(_pHuart, ipData, size);
    }

    // Invalid size
    return HAL_ERROR;
}

UART& UART::operator<<(const char* ipData)
{
    (void)transmit(ipData);
    return *this;
}

UART& UART::operator<<(const std::tuple<const uint8_t*, uint16_t> &iData)
{
    (void)transmit(std::get<0>(iData), std::get<1>(iData));
    return *this;
}

//
// Rx
//

uint16_t UART::receive(char *iopData, uint16_t size, uint32_t iTimeout)
{
    return receive(
        reinterpret_cast<uint8_t*>(iopData),
        size,
        iTimeout
        );
}

uint16_t UART::receive(uint8_t* iopData, uint16_t size, uint32_t iTimeout)
{
    uint16_t received = 0;
    const uint32_t startTick = HAL_GetTick();

    auto* pRxBufferBase = _pRxBufferBase;

    while (size > 0)
    {
        if (_pRxBufferPtr == _pRxBufferBase)
        {
            if (HAL_GetTick() - startTick > iTimeout)
            {
                break;
            }

            // 1ms delay to avoid MCU saturation
            HAL_Delay(1);
            continue;
        }

        *iopData = *pRxBufferBase;
        iopData++;

        pRxBufferBase++;
        size--;

        received++;
    }

    {
        //const irq_lock_guard lock;
        for (auto *ptr = pRxBufferBase, *base = _pRxBufferBase;
            ptr < _pRxBufferPtr; ptr++, base++)
        {
            *base = *ptr;
        }
    }
    
    return received;
}

UART& UART::operator>>(std::tuple<uint8_t*, uint16_t> &ioData)
{
    receive(std::get<0>(ioData), std::get<1>(ioData));
    return *this;
}

UART& UART::operator>>(std::tuple<char*, uint16_t> &ioData)
{
    receive(std::get<0>(ioData), std::get<1>(ioData));
    return *this;
}

UART::UART(UART_HandleTypeDef *ipHuart, uint16_t iRxBufferSize)
    : _pHuart(ipHuart),
      _pRxBufferBase(nullptr), _pRxBufferPtr(nullptr), _pRxBufferEnd(nullptr),
      _rxBufferSize(iRxBufferSize)
{
    if (_pHuart == nullptr)
    {
        Error_Handler();
    }

    if (iRxBufferSize > 0)
    {
        _pRxBufferBase = new uint8_t[iRxBufferSize];
        _pRxBufferPtr = _pRxBufferBase;
        _pRxBufferEnd = _pRxBufferBase + iRxBufferSize;
    }
}

