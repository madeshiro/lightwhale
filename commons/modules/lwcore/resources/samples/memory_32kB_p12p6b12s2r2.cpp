/**
 * @file memory_32kB_p12p6b12s2r2.cpp
 *
 * @brief Example of a static declared heap memory for an embedded program on a 32 bits
 * architecture (compatible 64 bits).
 *
 * It contains two paging chunks of 12 KiB and 6 KiB which ones dedicated for
 * large objects (over 128 Bytes, up to 1 KiB); one buffer chunk of 12 KiB which
 * managed buffer with size between 32B up to 1 KiB and one stack chunk of 2 KiB memory.
 *
 * Reserved chunk used for chunks' headers is considered optimal at a size of 2 KiB.
 * Then, efficient size of this configuration is actually 30KiB.
 *
 * -----------------------------------------------------------------------------
 *
 * File generated on Dec 2024 by Rin Baudelet
 */
// -------------------------------------------------------------------------- //
/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <lwcore/memory.h>

// For a 32-bit system, defining sizeof(memsize_t) == 16 (max block size is 64 KiB bytes)
// --
// optimal header size estimated as with 4 pages cache
// - bytes per page: 2 + (512/(4*8)) = 18 Bytes
// - bytes for every pages: 130 * 24 = 432 Bytes
// - bytes for cache: ~12B*4 = ~48 Bytes
// - bytes for chunk: ~100 Bytes
// total: 580 Bytes == 4.72%
MEMORY_CHUNK_DEFINE_PAGING(objects, 12_KiB, 512, 4, 4);
// --
// optimal header size estimated as with 4 pages cache
// - bytes per page: 2 + (1024/(128*8)) = 3 Bytes
// - bytes for every pages: 3 * 6 = 18 Bytes
// - bytes for cache: ~12B*4 = ~48 Bytes
// - bytes for chunk: ~100 Bytes
// total: 166 Bytes == 2.70%
MEMORY_CHUNK_DEFINE_PAGING(big_objects, 6_KiB, 1_KiB, 128, 4);
// --
// optimal header size estimated as with minimum buffer size = 32 Bytes
// - buffer descriptor array's size: (1+384) * 2 = 770 Bytes
// - bytes for chunk: ~100 Bytes
// total: 870 Bytes == 6,26%
MEMORY_CHUNK_DEFINE_BUFFER(buffers, 12_KiB, 0x7E0);
// --
// optimal header estimated
// - bytes for chunk: ~60 bytes
// total: 60 bytes == 2,30%
MEMORY_CHUNK_DEFINE_STACK(stack, 2_KiB);

//
// optimal reserved chunk size estimated to 1,64 KiB == 5,11%
//

MEMORY_STATIC_POOL(2_KiB,
    MEMORY_CHUNK(objects),
    MEMORY_CHUNK(big_objects),
    MEMORY_CHUNK(buffers),
    MEMORY_CHUNK(stack)
);

