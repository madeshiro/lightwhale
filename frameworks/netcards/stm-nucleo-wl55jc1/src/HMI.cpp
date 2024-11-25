// Local
#include "HMI.h"
#include "Board.h"
// STL
#include <usart.h>

void BSP_PB_Callback(Button_TypeDef Button)
{
    Board::GetHMI()->pbCallback(Button);
}

void HMI::pbCallback(Button_TypeDef iButton)
{
    switch (iButton)
    {
        case BUTTON_SW1:
            pb1Callback(); break;
        case BUTTON_SW2:
            switch (_state)
            {
                case HMIState::k_loraCom:
                    pb2LoraCallback(); break;
                case HMIState::k_uartCom:
                    pb2UartCallback(); break;
                case HMIState::k_none:
                    pb2NoneCallback(); break;
            }
            break;
        case BUTTON_SW3:
            pb3Callback(); break;
    }
}

void HMI::update()
{
    const auto tick = HAL_GetTick();

    if (HMIState::k_none == _state)
    {
        // Force LED off
        BSP_LED_Off(LED_GREEN);
        BSP_LED_Off(LED_RED);
        BSP_LED_Off(LED_BLUE);
    }
    else
    {
        //comLedUpdate();
    }

    // update HMI update time
    _lastUpdate = tick;
}

void HMI::comLedUpdate() const
{
    bool Tx = false;
    bool Rx = false;

    switch (_state)
    {
        case HMIState::k_loraCom:
            Tx = false; // TODO Radio COM Led
            Rx = false; // TODO Radio COM Led
            break;
        case HMIState::k_uartCom:
            Tx = GPIO_PIN_SET != HAL_GPIO_ReadPin(COM1_TX_GPIO_PORT, COM1_TX_PIN);
            Rx = GPIO_PIN_SET != HAL_GPIO_ReadPin(COM1_RX_GPIO_PORT, COM1_RX_PIN);
            break;
        default:
            return;
    }

    // Update Tx led (green)
    if (Tx)
        BSP_LED_On(LED_GREEN);
    else
        BSP_LED_Off(LED_GREEN);

    // Update Rx led (red)
    if (Rx)
        BSP_LED_On(LED_RED);
    else
        BSP_LED_Off(LED_RED);
}

void HMI::setState(HMIState iState)
{
    _state = iState;
}

//
// PUSH-BUTTON 1 CALLBACK
//

void HMI::pb1Callback()
{
    _state = static_cast<HMIState>((static_cast<uint8_t>(_state)+1)%3);

    // Display menu selection
    // - BLUE  = LORA
    // - GREEN = UART
    // - RED   = NONE
    BSP_LED_On(static_cast<Led_TypeDef>(_state));
    HAL_Delay(500);

    // Force update HMI
    BSP_LED_Off(LED_BLUE);
    BSP_LED_Off(LED_GREEN);
    BSP_LED_Off(LED_RED);
}

//
// PUSH-BUTTON 2 CALLBACKS
//

void HMI::pb2NoneCallback()
{
    // Not implemented
}

void HMI::pb2LoraCallback()
{
    // Not implemented
}

void HMI::pb2UartCallback()
{
    auto tick = HAL_GetTick() + 100;
    auto* hcom1 = &hcom_uart[COM1];

    const uint8_t msg[] = {0x88, 0x93, 0x13, 0x44, 0x52, 0x61, 0xff};
    size_t len = sizeof(msg);

    auto* msg16 = new uint16_t[len];
    for (size_t i = 0; i < len; i++)
    {
        msg16[i] = 0x100 | static_cast<uint16_t>(msg[i]);
    }

    for (size_t i = 0; i < 5*1000; i++)
    {
        HAL_UART_Transmit(
            hcom1,
            msg,
            static_cast<uint16_t>(len),
            1000
        );

        // 15Hz
        if (HAL_GetTick() - tick >= 66)
        {
            BSP_LED_Toggle(LED_GREEN);
            tick = HAL_GetTick();
        }
    }
    BSP_LED_Off(LED_GREEN);

    delete[] msg16;
}

//
// PUSH-BUTTON 3 CALLBACK
//

void HMI::pb3Callback()
{
    // Not implemented
}

