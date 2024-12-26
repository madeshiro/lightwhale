//------------------------------------------------------------------------------
//
// File:        memory.h (C++)
// Description:
//
//------------------------------------------------------------------------------
//
// File generated on Dec 2024 by Rin Baudelet
//------------------------------------------------------------------------------

/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#ifndef LW_CORE_MEMORY_H
#define LW_CORE_MEMORY_H
#include "Session.h"
#ifdef LWCORE_USE_MEMORY_MANAGER
// LWCORE
#include "lwcore/lwcore-defs.h"
// STL
#include <cstdint>
#include <algorithm>

//#//
//#// >> memory macros and traits <<
//#//

#define LWCORE_UNIT_KiB 1024
#define LWCORE_UNIT_MiB (1024 * 1024)
#define LWCORE_UNIT_GiB (1024 * 1024 * 1024)

constexpr unsigned long long operator ""_KiB(unsigned long long n)
{
    return n * LWCORE_UNIT_KiB;
}

constexpr unsigned long long operator ""_MiB(unsigned long long n)
{
    return n * LWCORE_UNIT_MiB;
}

constexpr unsigned long long operator ""_GiB(unsigned long long n)
{
    return n * LWCORE_UNIT_GiB;
}

#define MEMORY_CHUNK_DEFINE_PAGING(name, size, min_block, max_block, page_size) \
    extern const char __chunk_name_##name[] = #name; \
    using _chunk_t_##name = lw::memory::detail::chunks::paging< \
        __chunk_name_##name, size, min_block, max_block, page_size \
    >;

#define MEMORY_CHUNK_DEFINE_STACK(name, size) \
    extern const char __chunk_name_##name[] = #name; \
    using _chunk_t_##name = lw::memory::detail::chunks::stack< \
        __chunk_name_##name, size \
    >;

#define MEMORY_CHUNK_DEFINE_BUFFER(name, size, bufferSizeMask) \
    extern const char __chunk_name_##name[] = #name; \
    using _chunk_t_##name = lw::memory::detail::chunks::buffer< \
        __chunk_name_##name, size, bufferSizeMask \
    >;

#define MEMORY_CHUNK(name) _chunk_t_##name

#define MEMORY_STATIC_POOL(iReservedSize, iChunks...) \
    using _LwCore_Memory = lw::memory::detail::StaticMemory< \
        lw::memory::detail::Pool< \
            lw::memory::detail::chunks::reserved<iReservedSize>, \
            iChunks \
        > \
    >; \
    uint8_t* __lwcore_memory_heap      = (uint8_t*) _LwCore_Memory::AllocateHeap(); \
    size_t   __lwcore_memory_heap_size = _LwCore_Memory::_heap_size; \
    const size_t* const* __lwcore_memory_pool_chunks   = _LwCore_Memory::pool_t::_chunks; \
    size_t               __lwcore_memory_pool_chunks_N = _LwCore_Memory::pool_t::_chunks_N

#define MEMORY_DYNAMIC_POOL(iReservedSize, iChunks...) \
    using _LwCore_Memory = lw::memory::detail::DynamicMemory< \
        lw::memory::detail::Pool< \
            lw::memory::detail::chunks::reserved<iReservedSize>, \
            iChunks \
        > \
    >; \
    uint8_t* __lwcore_memory_heap      = (uint8_t*) _LwCore_Memory::AllocateHeap(); \
    size_t   __lwcore_memory_heap_size = _LwCore_Memory::_heap_size; \
    const size_t* const* __lwcore_memory_pool_chunks   = _LwCore_Memory::pool_t::_chunks; \
    size_t               __lwcore_memory_pool_chunks_N = _LwCore_Memory::pool_t::_chunks_N

//#//
//#// >> Forward declaration <<
//#//

// operator new for Paging
void* operator new(size_t, lw::Session* __lwcore_monitoring__) noexcept;
void* operator new(size_t, lw::Session*, const char* _pChunk __lwcore_monitoring__) noexcept;

// operator new for Stack
void* operator new(size_t, lw::Session*, size_t __lwcore_monitoring__) noexcept;
void* operator new(size_t, lw::Session*, size_t, const char* _pChunk __lwcore_monitoring__) noexcept;

// operator new for Buffer
void* operator new[](size_t, lw::Session* __lwcore_monitoring__) noexcept;
void* operator new[](size_t, lw::Session*, const char* _pChunk __lwcore_monitoring__) noexcept;

namespace lw::memory
{
    //
    // Pointer with paradygm
    //

    /**
     *
     * @tparam T
     */
    template < typename T > class shared_ptr;
    /**
     *
     * @tparam T
     */
    template < typename T > class uniq_ptr;

    /**
     *
     * @tparam T
     */
    template < typename T > class allocator;

    class MemoryManager;
    class PoolMemory;
    class Chunk;
    class Page;
    struct Block;
} // namespace lw::memory

//#//
//#// >> Aliases <<
//#//

extern "C"
{
    extern void lw_memcpy(void* dest, const void* src, size_t n);
    extern void lw_zero(void* dest, size_t n);
}

namespace lw
{
    using MemoryManager = memory::MemoryManager;
    using MemoryChunk   = memory::Chunk;
    using MemoryPage    = memory::Page;

    /**
     * Copy memory byte per byte from source pointer to destination.
     * @param ipDest the destination pointer
     * @param ipSource the source pointer
     * @param count the number of bytes to copy
     */
    void memcpy(void* ipDest, const void* ipSource, size_t count) __asm__("lw_memcpy");
    /**
     * Fill memory with zero.
     * @param ipDest the destination pointer
     * @param count the number of bytes to fill with zero
     */
    void zero(void* ipDest, size_t count) __asm__("lw_zero");
}

namespace elw
{
    using core_ptr   = lw::memory::shared_ptr<uint8_t>;
    using buffer_ptr = lw::memory::uniq_ptr<uint8_t>;
} // namespace elw

//#//
//#// >> Definitions <<
//#//

namespace lw::memory
{
    namespace detail
    {
        template <
            bool _is_static_,
            typename _pool_t_
        >
        struct Memory
        {
            using pool_t = _pool_t_;
            using heap_t = uint8_t*;

            static constexpr size_t _heap_size = pool_t::_heap_size;

            static heap_t AllocateHeap()
            {
                if constexpr (_is_static_)
                {
                    static uint8_t _heap[_heap_size] = {};
                    return _heap;
                }
                else
                {
                    static auto _heap = static_cast<uint8_t*>(std::malloc(_heap_size));
                    return _heap;
                }
            }
        };

        template < typename pool_t > using StaticMemory = Memory<true, pool_t>;
        template < typename pool_t > using DynamicMemory = Memory<false, pool_t>;

        template < typename chunk_t, typename... others_chunk_t >
        struct addition_chunk_size
        {
            static constexpr size_t value = chunk_t::_chunk_size
                + addition_chunk_size<others_chunk_t...>::value;
        };

        template < typename chunk_t >
        struct addition_chunk_size<chunk_t>
        {
            static constexpr size_t value = chunk_t::_chunk_size;
        };

        template < typename chunk_t >
        struct is_reserved_chunk;

        template < typename chunk_t, typename... chunks_t >
        struct has_reserved_chunk
        {
            static constexpr bool value = is_reserved_chunk<chunk_t>::value
                || has_reserved_chunk<chunks_t...>::value;
        };

        template < typename chunk_t>
        struct has_reserved_chunk<chunk_t>
        {
            static constexpr bool value = is_reserved_chunk<chunk_t>::value;
        };

        constexpr bool is_powerof_two(size_t value)
        {
            return value && (value & value - 1) == 0;
        }

        template < typename... chunk_t >
        struct Pool
        {
            static_assert (has_reserved_chunk<chunk_t...>::value,
                "make sure to integrate a 'reserved' chunk for memory headers");

            static constexpr size_t _heap_size = addition_chunk_size<chunk_t...>::value;
            static constexpr size_t _chunks_N  = sizeof...(chunk_t);

            static constexpr const size_t* _chunks[_chunks_N] = {
                chunk_t::_this ...
            };
        };

        namespace chunks
        {
            enum class type : size_t
            {
                reserved = 0x00,
                paging = 0x01,
                buffer = 0x02,
                stack = 0x03
            };

            template <
                const char* _name_,
                size_t _chunk_size_,
                size_t _block_size_inliers_min_,
                size_t _block_size_inliers_max_,
                size_t _page_size_,
                size_t _page_cache_ = 4U
            >
            struct paging
            {
                static_assert (is_powerof_two(_block_size_inliers_min_));
                static_assert (is_powerof_two(_block_size_inliers_max_));
                static_assert (_page_size_%_block_size_inliers_max_ == 0);
                static_assert (_chunk_size_%_page_size_ == 0);

                static constexpr const char* _name = _name_;
                static constexpr size_t _chunk_size = _chunk_size_;
                static constexpr size_t _this[] = {
                    reinterpret_cast<size_t>(_name),
                    static_cast<size_t>( type::paging ),
                    _chunk_size_,
                    _block_size_inliers_min_,
                    _block_size_inliers_max_,
                    _page_size_,
                    _page_cache_
                };
            };

            template <
                const char* _name_,
                size_t _chunk_size_
            >
            struct stack
            {
                // check alignment
                static_assert (_chunk_size_ % sizeof(size_t) == 0);

                static constexpr const char* _name = _name_;
                static constexpr size_t _chunk_size = _chunk_size_;
                static constexpr size_t _this[] = {
                    reinterpret_cast<size_t>(_name),
                    static_cast<size_t>( type::stack ),
                    _chunk_size_
                };
            };

            template <
                const char* _name_,
                size_t _chunk_size_,
                size_t _buffer_sizes_mask_
            >
            struct buffer
            {
                // check alignment
                static_assert (_chunk_size_ % sizeof(size_t) == 0);

                static constexpr const char* _name = _name_;
                static constexpr size_t _chunk_size = _chunk_size_;
                static constexpr size_t _this[] = {
                    reinterpret_cast<size_t>(_name),
                    static_cast<size_t>( type::buffer ),
                    _chunk_size_,
                    _buffer_sizes_mask_
                };
            };

            extern inline const char __chunk_name_reserved[] = "reserved"; /*NOLINT*/

            template <size_t _chunk_size_>
            struct reserved
            {
                // check alignment
                static_assert (_chunk_size_ % sizeof(size_t) == 0);

                // check enough memory for the reserved chunk
                static_assert (1_KiB <= _chunk_size_,
                    "it is greatly adviced to provide at least 1KB "
                    "for a reserved chunk");

                static constexpr size_t _chunk_size = _chunk_size_;
                static constexpr size_t _this[] =
                {
                    reinterpret_cast<size_t>( __chunk_name_reserved ),
                    static_cast<size_t>( type::reserved ),
                    _chunk_size_
                };
            };
        }

        template < typename >
        struct is_reserved_chunk
        {
            static constexpr bool value = false;
        };

        template <size_t _chunk_size_>
        struct is_reserved_chunk<chunks::reserved<_chunk_size_>>
        {
            static constexpr bool value = true;
        };

        namespace chunk
        {
            struct __header__ /*NOLINT*/
            {
                const char* _name;
                const chunks::type _type;
            };

            struct paging  /*NOLINT*/
            {
                const char* _name;
            private:
                const chunks::type _type;
            public:
                size_t _chunkSize;
                size_t _blockSizes[2];
                size_t _pageSize;
                size_t _pageCache;
            };

            struct stack /*NOLINT*/
            {
                const char* _name;
            private:
                const chunks::type _type;
            public:
                size_t _chunkSize;
            };

            struct buffer /*NOLINT*/
            {
                const char* _name;
            private:
                const chunks::type _type;
            public:
                size_t _chunkSize;
                uint16_t _bufferSizesMask;
            };

            struct reserved /*NOLINT*/
            {
                const char* _name;
            private:
                const chunks::type _type;
            public:
                size_t _chunkSize;
            };
        }
    } // namespace detail

    //
    // DEFINITIONS OF CHUNK, PAGE
    //

    class Chunk
    {
        detail::chunk::__header__* const _pHeader;
    public:
        // Alias to enum
        using Type = detail::chunks::type;

        // Alias to a heap type
        using ptr_t = uint8_t*;
        using refptr_t = ptr_t&;

        // public const traits
        const char* name = _pHeader->_name;
        Type        type = _pHeader->_type;

        class Paging /* give an interface of allowed operation for paging */
        {
            auto* detail() const
            { return reinterpret_cast<detail::chunk::paging*> (_chunk._pHeader); }

            struct Page
            {

            };
        public:
            ptr_t allocate(size_t __lwcore_monitoring__);
            bool deallocate(ptr_t __lwcore_monitoring__);
        private:
            explicit Paging(Chunk& iParent);

            //
            // Private fields
            //

            memsize_t _maxAvailPages;
            memsize_t _maskBytePerPage;

            memsize_t*  _pPages;        // pages sizes
            uint8_t*    _pPagesMask;    // pages mask (for block availability)

            Page** _pCache;

            Chunk& _chunk;
            friend class Chunk;
        };
        /**
         * Cast current chunk as Paging's chunk
         * @return Chunk's paging entity
         */
        Paging& asPaging() const;

        class Stack /* give an interface of allowed operation for stack */
        {
            const auto* detail() const
            { return reinterpret_cast<const detail::chunk::stack*> (_chunk._pHeader); }
        public:
            /**
             *
             * @param iSize
             * @return
             */
            void * push(size_t iSize __lwcore_monitoring__);

            /**
             *
             * @param iSize
             * @return
             */
            void * pop(size_t iSize __lwcore_monitoring__);
        private:
            /**
             *
             * @param iParent
             */
            explicit Stack(Chunk& iParent);

            //
            // Private fields
            //

            ptr_t _pStack;

            Chunk& _chunk;
            friend class Chunk;
        };
        /**
         * Cast current chunk as Stack's chunk
         * @return Chunk's stack entity
         */
        Stack & asStack() const;

        class Buffer /* give an interface of allowed operation for buffer */
        {
            const auto* detail() const
            { return reinterpret_cast<const detail::chunk::buffer*> (_chunk._pHeader); }

            struct Block
            {
                const Buffer& _parent;
                memsize_t* _pHeader;
                ptr_t _pData;

                /**
                 *
                 * @return
                 */
                Block nextBlock() const;
                constexpr bool isAvailable() const;

                /**
                 * Allocate the block as a buffer with the specified size. If requested size
                 * is lower than the actual block's size, then the block will be split.
                 * @warning If the block is currently not available or the current size is lower
                 * than the requested size, the function will return null.
                 *
                 * @return the block address if it is available and size fit the requested size
                 * to allocate, otherwise, it will return @code nullptr@endcode.
                 */
                void * alloc(memsize_t&) const;

                /**
                 * Make the block available to use. Any next block will be merged with this one if
                 * they are available. If the current block is already available, the function won't
                 * do anything (except in debug mode: a note will be reported as "memory free
                 * twice")
                 */
                void free() const;

                constexpr memsize_t size() const
                { return *_pHeader & LWCORE_MEMSIZE_MAXINT; }
            };

            /**
             * @return The round size using the buffer's chunk allowed's buffer's size.
             * If the size cannot be rounded (higher than allowed or due to an invalid mask), then
             * the function will return 0.
             */
            memsize_t roundSize(memsize_t) const;
        public:

            /**
             *
             * @param iSize
             * @return
             */
            void* allocate(memsize_t& iSize __lwcore_monitoring__) const;

            /**
             *
             * @param p
             * @return
             */
            void deallocate(void* p) const;
        private:
            /**
             *
             * @param iParent
             */
            explicit Buffer(Chunk& iParent);

            //
            // Private fields
            //

            memsize_t* _pBlocks;
            memsize_t  _minBufferSize;

            Chunk& _chunk;
            friend class Chunk;
            friend struct Block;
        };
        /**
         * Cast current chunk as Buffer's chunk
         * @return Chunk's buffer entity
         */
        Buffer& asBuffer() const;

        //
        // Memory Allocation
        //

    private:
        Chunk(detail::chunk::__header__* ipDetailedHeader,
            ptr_t ipHeaderStart, ptr_t ipDataStart,
            ptr_t& iopHeaderEnd, ptr_t ipDataEnd
            );

        ptr_t _pChunkStart;
        ptr_t _pChunkEnd;

        ptr_t _pHeaderStart;
        ptr_t _pHeaderEnd;

        friend class PoolManager;
    };

    //
    // DEFINITION OF POOLMEMORY
    //

    /**
     * PoolMemory contains all chunks and manages memory allocation as an interface between
     * the memory manager and chunks.
     *
     * This class used user-defined primitive chunks definitions to allocate chunks with
     * their headers and associated allocated chunks' memories as sections.
     *
     * @details Singleton allocated by \link lw::MemoryManager \endlink
     */
    class PoolMemory
    {
    public:
        Chunk& getChunk(const char* ipChunkName);

    private:
        Chunk _reserved;
    };

    //
    // DEFINITION OF MEMORYMANAGER
    //

    /**
     * @details Singleton
     */
    class MemoryManager final
    {
    public:
        void* allocate(size_t __lwcore_monitoring__);
        void* allocateBuffer(size_t __lwcore_monitoring__);
        void* push(size_t __lwcore_monitoring__);

        void* pop(size_t __lwcore_monitoring__);
        void deallocate(void* __lwcore_monitoring__);

        PoolMemory& pool();
    private:
        MemoryManager();

#ifdef LWCORE_DEBUG
        //
        // Log
        //

        void reportWarning(const char* ipMsg, const char* ipChunk __lwcore_monitoring__);
        void reportError(const char* ipMsg, const char* ipChunk __lwcore_monitoring__);

#endif


        //
        // Friend class as package 'memory'
        //
        friend class Chunk::Buffer;
        friend class Chunk::Paging;
        friend class Chunk::Stack;

        // Full access from Session manager
        friend class Session;
    };
}
#else
#pragma message("Info: Memory management disabled")
#endif // LWCORE_USE_MEMORY_MANAGER
#endif // LW_CORE_MEMORY_H
