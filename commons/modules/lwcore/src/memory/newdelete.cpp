/*
* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "lwcore/memory.h"

//
// operator new :: PAGING
//

void* operator new(size_t iSize, lw::Session* ipSession __lwcore_monitoring__) noexcept
{
    __lw_assert_return(nullptr != ipSession, nullptr);
    return ipSession->memoryManager().allocate(iSize __lwcore_trace__);
}

void* operator new(size_t iSize, lw::Session* ipSession, const char* _pChunk __lwcore_monitoring__) noexcept
{
    __lw_assert_return(nullptr != ipSession, nullptr);

    const auto& chunk = ipSession->memoryManager().pool().getChunk(_pChunk);
    __lw_assert_return(chunk.type == lw::memory::Chunk::Type::paging, nullptr);

    return chunk.asPaging().allocate(iSize __lwcore_trace__);
}

//
// operator new :: STACK
//

void* operator new(size_t iSize, lw::Session* ipSession, size_t __lwcore_monitoring__) noexcept
{
    __lw_assert_return(nullptr != ipSession, nullptr);
    return ipSession->memoryManager().push(iSize __lwcore_trace__);
}

void* operator new(size_t iSize, lw::Session* ipSession, size_t, const char* _pChunk
    __lwcore_monitoring__) noexcept
{
    __lw_assert_return(nullptr != ipSession, nullptr);

    const auto& chunk = ipSession->memoryManager().pool().getChunk(_pChunk);
    __lw_assert_return(chunk.type == lw::memory::Chunk::Type::stack, nullptr);

    return chunk.asStack().push(iSize __lwcore_trace__);
}

//
// operator new :: BUFFER
//

void* operator new[](size_t iSize, lw::Session* ipSession __lwcore_monitoring__) noexcept
{
    __lw_assert_return(nullptr != ipSession, nullptr);
    return ipSession->memoryManager().allocateBuffer(iSize __lwcore_trace__);
}

void* operator new[](size_t iSize, lw::Session* ipSession, const char* _pChunk
    __lwcore_monitoring__) noexcept
{
    __lw_assert_return(nullptr != ipSession, nullptr);

    const auto& chunk = ipSession->memoryManager().pool().getChunk(_pChunk);
    __lw_assert_return(chunk.type == lw::memory::Chunk::Type::buffer, nullptr);

    memsize_t size = iSize;
    return chunk.asBuffer().allocate(size __lwcore_trace__);
}
