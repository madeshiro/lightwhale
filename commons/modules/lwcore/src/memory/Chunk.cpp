/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "lwcore/memory.h"
#include "lwcore/RuntimeException.h"

#define memsize_highbit (LWCORE_MEMSIZE_MAXUINT ^ LWCORE_MEMSIZE_MAXINT)

namespace lw::memory
{
    Chunk::Buffer& Chunk::asBuffer() const
    {
        __lw_assert(Type::stack == type);
        return *reinterpret_cast<Buffer*>(_pHeaderStart);
    }

    Chunk::Paging& Chunk::asPaging() const
    {
        __lw_assert(Type::stack == type);
        return *reinterpret_cast<Paging*>(_pHeaderStart);
    }

    Chunk::Stack& Chunk::asStack() const
    {
        __lw_assert(Type::stack == type);
        return *reinterpret_cast<Stack*>(_pHeaderStart);
    }

    //
    // Chunk::Paging
    //

    Chunk::Paging::Paging(Chunk& iParent)
        : _chunk(iParent)
    {
        ptr_t pHdrOffset     = _chunk._pHeaderStart + sizeof(*this);
        size_t headerMaxSize = _chunk._pHeaderEnd - pHdrOffset;

        //>> Prepare cache
        //@{

        // Assert if enough space available inside the header
        __lw_assert(headerMaxSize > sizeof(Page*)*detail()->_pageCache);

        // Init the cache and force memory to zero
        _pCache = new (pHdrOffset) Page*[detail()->_pageCache];
        zero(_pCache, sizeof(Page*)*detail()->_pageCache);

        // update header ptr's cursor
        pHdrOffset += detail()->_pageCache*sizeof(Page*);

        //@}
        //>> Prepare pages header
        //@{

        // Update max avail size inside the header
        headerMaxSize = _chunk._pHeaderEnd - pHdrOffset;

        _maskBytePerPage = detail()->_pageSize / detail()->_blockSizes[0];
        _maxAvailPages   = detail()->_chunkSize / detail()->_pageSize;

        size_t currentHeaderUsage = _maxAvailPages * (sizeof(memsize_t) + _maskBytePerPage);

        // Reduce the number of available pages to fit available places inside the header
        // Fine-tuning between page's size, and the max-available pages header can handle
        // to make sure the whole allocated memory will be used
        while (currentHeaderUsage > headerMaxSize)
        {
            _maxAvailPages /= 2;
            detail()->_pageSize <<= 1;

            _maskBytePerPage = detail()->_pageSize / detail()->_blockSizes[0];
            _maxAvailPages   = detail()->_chunkSize / detail()->_pageSize;

            currentHeaderUsage = _maxAvailPages * (sizeof(memsize_t) + _maskBytePerPage)
            > headerMaxSize;

            // if maxAvailPages equals 0, it means that the reserved chunk
            // used for chunks' headers is too small and can't handle
            // chunk features' memory
            __lw_assert(_maxAvailPages > 1U, "Reserved chunk too small!");
        }

        // Set-up pages
        _pPages = new (pHdrOffset) memsize_t[_maxAvailPages];
        pHdrOffset += _maxAvailPages*sizeof(memsize_t);

        // Set-up pages mask
        _pPagesMask = new (pHdrOffset) uint8_t[_maxAvailPages * _maskBytePerPage];
        pHdrOffset += _maxAvailPages * _maskBytePerPage * sizeof(uint8_t);

        //@}

        // Update header tail
        _chunk._pHeaderEnd = pHdrOffset;
    }

    Chunk::ptr_t Chunk::Paging::allocate(size_t iSize __lwcore_monitoring__)
    {
// TODO
    }

    bool Chunk::Paging::deallocate(ptr_t ipPtr __lwcore_monitoring__)
    {
// TODO
    }

    //
    // Chunk::Stack
    //

    Chunk::Stack::Stack(Chunk& iParent)
        : _pStack(iParent._pChunkStart), _chunk(iParent)
    {
        // Stack does not need a header except for its entity
        _chunk._pHeaderEnd = _chunk._pHeaderStart + sizeof(*this);
    }

    void* Chunk::Stack::push(size_t iSize __lwcore_monitoring__)
    {
        if (_pStack + iSize > _chunk._pHeaderEnd)
        {
            return nullptr;
        }

        void* ptr = _pStack;
        _pStack += iSize;

        return ptr;
    }

    void* Chunk::Stack::pop(size_t iSize __lwcore_monitoring__)
    {
        if (_pStack - iSize < _chunk._pHeaderStart)
        {
            return nullptr;
        }

        _pStack -= iSize;
        return _pStack;
    }

    //
    // Chunk::Buffer
    //

    Chunk::Buffer::Block Chunk::Buffer::Block::nextBlock() const
    {
        if (0U == size())
        {
            return *this;
        }

        auto pData = _pData + size();
        memsize_t* pHeader = _pHeader + size() / _parent._minBufferSize;

        return {_parent, pHeader, pData};
    }

    constexpr bool Chunk::Buffer::Block::isAvailable() const
    {
        return *_pHeader & memsize_highbit == 0;
    }

    void* Chunk::Buffer::Block::alloc(memsize_t& iSize) const
    {
        iSize = _parent.roundSize(iSize);
        if (!isAvailable() || iSize == 0 || size() < iSize)
        {
            return nullptr;
        }

        _pHeader[iSize/_parent._minBufferSize] = size() - iSize;
        return _pData;
    }

    void Chunk::Buffer::Block::free() const
    {
        // set higher-bit as '0' (indicate that it is free to use)
        *_pHeader &= LWCORE_MEMSIZE_MAXINT;

        // Try to merge every next available block
        Block next = nextBlock();
        while (next.size() > 0 && next.isAvailable())
        {
            // Increase the current block's size to merge blocks
            *_pHeader += next.size();
            *next._pHeader = 0U; // set the next block as totally unused

            const Block b = next.nextBlock();

            next._pData   = b._pData;
            next._pHeader = b._pHeader;
        }
    }

    memsize_t Chunk::Buffer::roundSize(memsize_t iSize) const
    {
        size_t roundSize = 1;
        size_t masks = detail()->_bufferSizesMask;
        while (iSize >= roundSize && masks & 1)
        {
            if (masks == 0)
            {
                // size cannot be rounded
                return 0U;
            }

            masks >>= 1;
            roundSize <<= 1;
        }

        return roundSize;
    }

    void *Chunk::Buffer::allocate(memsize_t &iSize __lwcore_monitoring__) const
    {
        Block block {*this, reinterpret_cast<memsize_t *>(_chunk._pHeaderStart),
            _chunk._pChunkStart};

        iSize = roundSize(iSize);

        while (!block.isAvailable() || block.size() < iSize)
        {
            Block b = block.nextBlock();

            if (0 == b.size())
            {
                return nullptr;
            }

            block._pHeader = b._pHeader;
            block._pData = b._pData;
        }

        return block.alloc(iSize);
    }

    void Chunk::Buffer::deallocate(void *p) const
    {
        if (p == nullptr
            || (p < _chunk._pChunkStart && p >= _chunk._pChunkEnd))
        {
            return;
        }

        const auto addr = reinterpret_cast<size_t>(p);
        const size_t offset = addr / _minBufferSize;

        auto* pHeader = reinterpret_cast<memsize_t*>(_chunk._pHeaderStart + offset);

        const auto block = Block {*this, pHeader, static_cast<uint8_t*>(p)};

        block.free();
    }

    Chunk::Buffer::Buffer(Chunk& iParent)
        : _chunk(iParent)
    {
        ptr_t pHdrOffset     = _chunk._pHeaderStart + sizeof(*this);
        const size_t headerMaxSize = _chunk._pHeaderEnd - pHdrOffset;

        //>> prepare blocks header
        // @{

        _minBufferSize = roundSize(1U);

        // Assert if buffer sizes' mask is valid and roundSize() return a non-zero value
        __lw_assert(_minBufferSize > 0, "Invalid buffer sizes' mask!");

        const size_t headerMaxBlocks = 1 + detail()->_chunkSize / _minBufferSize;

        // Assert if enough space available inside the header
        __lw_assert(headerMaxSize >= headerMaxBlocks * sizeof(memsize_t),
            "Reserved chunk too small");

        // Set-up blocks headers
        _pBlocks = new(pHdrOffset) memsize_t[headerMaxBlocks];
        pHdrOffset += headerMaxBlocks * sizeof(memsize_t);

        // @}

        // Update header tail
        _chunk._pHeaderEnd = pHdrOffset;
    }

    //
    // Chunk ::
    //

    Chunk::Chunk(detail::chunk::__header__ *ipDetailedHeader,
        ptr_t ipHeaderStart, ptr_t ipDataStart,
        ptr_t& iopHeaderEnd, ptr_t ipDataEnd)
        : _pHeader(ipDetailedHeader),
        _pChunkStart(ipDataStart), _pChunkEnd(ipDataEnd),
        _pHeaderStart(ipHeaderStart), _pHeaderEnd(iopHeaderEnd)
    {
        // compute allowed header's size to assert fitting of chunk features' data
        const size_t maxHeaderSize = iopHeaderEnd - ipHeaderStart;

        // Prepare placement for chunk entity's instance
        void* pEntityPlacement = _pHeaderStart;

        // Allocation of Chunk's entity according to its type
        switch (ipDetailedHeader->_type)
        {
            case Type::buffer:
                __lw_assert(maxHeaderSize > sizeof(Buffer));
                new (pEntityPlacement) Buffer(*this);
                break;
            case Type::paging:
                __lw_assert(maxHeaderSize > sizeof(Paging));
                new (pEntityPlacement) Paging(*this);
                break;
            case Type::stack:
                __lw_assert(maxHeaderSize > sizeof(Stack));
                new (pEntityPlacement) Stack(*this);
                break;
            case Type::reserved:
                // Nothing much to do here, reserved only contains data
                _pChunkStart = ipDataStart;
                _pChunkEnd = ipDataStart +
                             reinterpret_cast<const detail::chunk::reserved *>(ipDetailedHeader)
                                     ->_chunkSize;
                break;
        }

        // Actualize header's end according to current header memory usage
        iopHeaderEnd = _pHeaderEnd;
    }
}
