/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "lrhtcp/lrhtcp.h"

#include <stdlib.h>

#include "lrhtcp/driver.h"

#define LP_CHANNEL_INVALID (lwp_channel_t) { .bandwidth=0, .channel=0 }
#define LP_ADDRESS_INVALID (lp_address_t) { .dword=0 }
#define LP_ADDRESS_BROADCAST (lp_address_t) { .bytes={255, 255, 255, 255} }

// --- //

uint8_t LpGetFrequencyPlan(pLwpSocket socket)
{
    return socket->frequencyPlan;
}

void LpSetFrequencyPlan(pLwpSocket socket, uint8_t frequencyPlan)
{
    switch (frequencyPlan)
    {
        case LP_FREQUENCY_PLAN_EU863_70:
            socket->syncChannel = (lwp_channel_t) {
                .bandwidth=0, // eq. 125 KHz // TODO RADIO DRIVER CHANGE
                .channel=869700000 // 869.7 MHz, 1% duty-cycle for 25mW e.r.p (h1.9)
            };
            break;
        case LP_FREQUENCY_PLAN_EU443:
            socket->syncChannel = (lwp_channel_t) {
                .bandwidth=0, // eq. 125 KHz // TODO RADIO DRIVER CHANGE
                .channel=443050000 // 433.05 MHz, 10% duty-cycle 10 mW e.r.p (g1)
            };
            break;
        default:
            socket->frequencyPlan = LP_FREQUENCY_PLAN_UNDEFINED;
            socket->syncChannel   = LP_CHANNEL_INVALID;
            return ; // NOT IMPLEMENTED
    }

    socket->frequencyPlan = frequencyPlan;
}

LwpSocket* LpCreateSocket(uint8_t frequencyPlan)
{
    LwpSocket* pSocket = malloc(sizeof(LwpSocket));

    if (NULL != pSocket)
    {
        LpSetFrequencyPlan(pSocket, frequencyPlan);
    }

    return pSocket;
}

//
// LRHTCP COM methods
//

bool LpListen(pLwpSocket socket)
{
    socket->bindAddress = LP_ADDRESS_BROADCAST;
    socket->protocolType = k_lwp_protocol_p2p;

    return LpDeviceDriver.Listen(socket);
}

bool LpGateway(pLwpSocket socket)
{
    return false;
}

void LpAccept(pLwpSocket socket)
{
    LpDeviceDriver.Accept(socket);
}

bool LpBind(pLwpSocket socket, lp_address_t lpdest)
{
    socket->protocolType = k_lwp_protocol_p2p;
    socket->bindAddress = lpdest;

    return LpDeviceDriver.Bind(socket);
}

bool LpBind2Net(pLwpSocket socket, lpIPV4 netaddr)
{
#if 0
    socket->protocolType = k_lwp_protocol_net;
    socket->bindAddress  = LP_ADDRESS_BROADCAST;
    // socket->netAddress = (lp_netaddress_t) { .ipv4 = netaddr };
#endif

    return false; // NOT IMPLEMENTED YET
}
// bool LpBind2Netv6(pLwpSocket socket, lpIPV6 netaddr); NOT IMPLEMENTED YET

size_t LpSend(pLwpSocket socket, const void* data, size_t length)
{
    return LpDeviceDriver.Send(socket, data, length);
}

size_t LpRecv(pLwpSocket socket, void* data, size_t length)
{
    return LpDeviceDriver.Receive(socket, data, length);
}

void LpClose(pLwpSocket socket)
{
    // Tell the NIC to close the communication
    LpDeviceDriver.Close(socket);

    // Free memory
    free(socket);
}