//------------------------------------------------------------------------------
//
// File:        UART.h
// Description: Definitions of UART communication module
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
#ifndef UART_H
#define UART_H
// Local
#include "stm-nucleo-wl55jc-defs.h"
// HAL
#include <stm32wlxx_hal_uart.h>


/**
 * UART Communication module is a second abstraction layer with the HAL and LL,
 * allowing DMA Receive/Transmission.
 *
 * In Rx mode, UART module can continuisly receive data without being explicitly read by
 * the main embedded software. It allows communication to continue without interruption while
 * the main program does something else. However, when Rx buffer reached the FIFO end,
 * an overflow packet will be sent to stop communication until the FIFO had some space
 * available (read).
 *
 * In Tx mode, UART module uses DMA with the Tx buffer to send data without interrupting the
 * main program. The Tx buffer size indicates how many bytes may be sent without having to
 * recall the UART transmit method.
 */
class UART
{
public:
    //
    // Rule-of-Five:
    //  - disallow copy
    //  - disallow move
    //  - destructor free buffers using PoolMemory
    //
    UART(const UART&) = delete;
    UART(UART&&) noexcept = delete;

    UART& operator=(const UART&) = delete;
    UART& operator=(UART&&) noexcept = delete;

    ~UART();

    //
    // Traits
    //

    /**
     *
     * @return
     */
    [[nodiscard]] uint32_t getBaudRate() const;

    /**
     *
     * @return
     */
    [[nodiscard]] uint32_t getParityMode() const;

    /**
     *
     * @return
     */
    [[nodiscard]] uint32_t getStopBitMode() const;

    //
    // Tx
    //

    uint16_t transmit(const uint8_t* data, uint16_t size) const;
    uint16_t transmit(const char* data, uint16_t size) const;

    //
    // Rx
    //

    uint16_t receive(uint8_t* data, uint16_t size) const;
    uint16_t receive(char* data, uint16_t size) const;

private:

    //
    // Constructors
    //

    /**
     * Create a new UART COM module from its STM32_HAL handle.
     *
     * @param ipHuart the UART HAL Handle
     * @param iRxBufferSize the DMA Rx buffer (max Rx size before interrupt Rx)
     * @param iTxBufferSize the DMA Tx buffer (max Tx size per transmission)
     */
    explicit UART(UART_HandleTypeDef* ipHuart,
        uint16_t iRxBufferSize = 1024, // 1 KiB
        uint8_t iTxBufferSize = 256    // 256 B
        );

    //
    // Private fields
    //
    UART_HandleTypeDef* _pHuart;

    // elw = embedded lightwhale (cf. lwcore)
    // elw::uniq_ptr _pRxBuffer;
    uint8_t* _pRxBuffer;

    // elw::uniq_ptr _pTxBuffer;
    uint8_t* _pTxBuffer;

    friend class Board;
};

#endif // UART_H
