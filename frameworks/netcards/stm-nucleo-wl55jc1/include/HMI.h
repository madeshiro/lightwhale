//------------------------------------------------------------------------------
//
// File:        hmi.h
// Description: Definitions Human/Machine Interface of the nucleo board
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
#ifndef HMI_H
#define HMI_H
// Board
#include "stm-nucleo-wl55jc-defs.h"
// STL/STDLIB
#include <cstdint>
#include <stm32wlxx_nucleo.h>

enum class HMIState : uint8_t
{
    k_none = 0x02,     // disable LEDs (blue='off')
    k_uartCom = 0x01,  // LEDs witness uart com (red='Rx', green='Tx', blue='blink 1Hz')
    k_loraCom = 0x00,  // LEDs witness lora com (red='Rx', green='Tx', blue='fixed')
};

/**
 * HMI defines the human-machine interface, as buttons and leds.
 * Buttons' events are connected to their features:
 * - PB1 => switch HMI state (leds display mode)
 * - PB2 =>
 *  - in @code k_none@endcode state, enable or disable serial debug mode (leds used as ON/OFF indicator)
 *  - in @code k_uartCom@endcode state, trigger a board/computer test message
 *  - in @code k_loraCom@endcode state, trigger a lora ping
 * - PB3 => special trigger <user-defined> for debug/demo purposes
 * @details Singleton
 */
class HMI
{
public:
    /**
     * Explicitly call the button's callback
     * @param iButton the triggered button.
     */
    void pbCallback(Button_TypeDef iButton);

    /**
     * Update interface.
     */
    void update();

    /**
     * Set the HMI display state.
     * @param iState the new HMI display state
     */
    void setState(HMIState iState);

    [[nodiscard]] constexpr HMIState getState() const
        { return _state; }

    [[nodiscard]] constexpr uint32_t getLastUpdateTime() const
    { return _lastUpdate; }
private:
    //
    // Private constructor(s)
    //

    /**
     * Creates a default HMI.
     */
    HMI() = default;

    //
    // Private methods
    //

    void pb1Callback();
    //
    void pb2NoneCallback();
    void pb2UartCallback();
    void pb2LoraCallback();
    //
    void pb3Callback();

    /**
     * Update LED according to COM (Lora or COM1[(LP)UART])
     */
    void comLedUpdate() const;

    //
    // Variables
    //

    HMIState _state = HMIState::k_none; /// the current HMI state, default to none
    uint32_t _lastUpdate = 0;
    uint32_t _lastUpdate1Hz = 0;

    /// Session class manage this singleton
    friend class Board;
};

#endif // HMI_H