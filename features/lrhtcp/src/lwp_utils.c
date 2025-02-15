/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "lrhtcp/lwp_utils.h"

uint8_t lwp_get_endianness(void)
{
    uint16_t endianness = 0x1;
    return ((uint8_t*)&endianness)[0];
}

uint16_t lwp_ntohs(uint16_t netword)
{
    if (LWP_ENDIANNESS_LITTLE == lwp_get_endianness())
    {
        netword = (netword << 8) | ((netword >> 8) & 0xFF);
    }

    return netword;
}

uint32_t lwp_ntohl(uint32_t netdword)
{
    if (LWP_ENDIANNESS_LITTLE == lwp_get_endianness())
    {
        netdword = (netdword&0xFF << 24)
            | (netdword&0xFF00 << 8)
            | (netdword&0xFF0000 >> 8)
            | (netdword&0xFF000000 >> 24);
    }

    return netdword;
}
