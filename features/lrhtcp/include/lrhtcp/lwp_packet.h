
#ifndef _lwp_packet_h_
#define _lwp_packet_h_

// Local
#include "lrhtcp-defs.h"

// C standard lib
#include "stdint.h"

//
// Macro values
//

#define LP_HEADER_PROTOCOL_VERSION 1 // Version of 2025/01

// --- //
// HEADER FLAGS

#define LP_HEADER_FLAG_DONOTFRAGMENT 0x01

// --- //

#define LP

//
// Enumerations
//

typedef enum lwp_service_type
{
    // Ping device
    k_lwp_service_ping          = 0x0,

    // Synchronize p2p communication
    k_lwp_service_sync_request  = 0x1, //< require channel synch for COM
    k_lwp_service_sync_reply    = 0x2, //< reply channel synch for COM

    // Control
    k_lwp_service_comm_transmit = 0x3,
    k_lwp_service_comm_ack      = 0x4,
    k_lwp_service_comm_nack     = 0x5,

} lwp_service_t;

typedef enum lwp_protocol_type
{
    k_lwp_protocol_lrhtcp_p2p = 0x0, // peer to peer communication using LRHTCP
    k_lwp_protocol_lrhtcp_net = 0x1, // gateway to internet using lRHTCP (TCP)

    k_lwp_protocol_unknown    = 0xf
} lwp_protocol_t;

//
// Union and other types
//

#define LP_ADDRESS_TYPE_LPV1        0x0 // use address assigned by the network provider (48 bits)
#define LP_ADDRESS_TYPE_MAC         0x1 // use material info (48 bits)
#define LP_ADDRESS_TYPE_RESERVED    0x2 // for future purpose
#define LP_ADDRESS_TYPE_LPV2        0x3 // not used yet (2032 bits)

typedef union lp_address
{
    uint8_t     bytes[4];
    uint16_t    words[2];
    uint32_t    qword;

    struct detail_definition
    {
        /**
         *
         */
        uint8_t  lp_type : 2;
        /**
         *
         */
        uint16_t dev_id  : 14;
        /**
         *
         */
        uint16_t suffix  : 16;
    } __lrhtcp_packet__ detail;
} lp_address_t;

//
// Packet structures
//

/**
 * \brief
 *
 */
typedef struct lwp_lp_header
{
    /**
     *
     */
    uint8_t lpVersion : 4;
    /**
     *
     */
    uint8_t lpHeaderLength : 4;

    /**
     *
     */
    lwp_service_t service : 4;

    /**
     *
     */
    uint8_t flags : 4;

    /**
     *
     */
    uint16_t totalLength;

    /**
     *
     */
    uint16_t identifier;

    /**
     *
     */
    uint16_t fragmentOffset;

    /**
     * (Required) lightwhale (lora) protocol host's address.
     *
     * \note The device can be identified using its MAC address. Report to the
     *       LightWhale Protocol Documentation to check how to build this packet.
     */
    lp_address_t lpHostAddress;
    /**
     * (Required for p2p) lightwhale (lora) protocol destination's address.
     */
    lp_address_t lpDestinationAddress;

} __lrhtcp_packet__ lwp_lp_header_t;

/**
 * Packet represen
 */
typedef struct lwp_lp_synch_frame
{
    struct lwp_lp_header header;

} __lrhtcp_packet__ lwp_lp_sync_frame_t;

// --- //


#endif // _lwp_packet_h_
