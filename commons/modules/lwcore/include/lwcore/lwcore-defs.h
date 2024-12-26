//------------------------------------------------------------------------------
//
// File:        lwcore-defs.h
// Description: Definitions of macros and typedefs for the lwcore library
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
#ifndef _lwcore_defs_h_
#define _lwcore_defs_h_
#include "lwcore/architecture.h"
// STL
#include <cstdint>

#define SESSION_STATE_OK      0x00
#define SESSION_STATE_ERROR   0x01
#define SESSION_STATE_BUSY    0x02
#define SESSION_STATE_TIMEOUT 0x03
#define SESSION_STATE_NOMEM   0x04

#ifdef LWCORE_USE_MEMORY_MANAGER

#ifndef LWCORE_MEMORY_SIZE_T
#pragma message ("Warning: Used default 16bits size_t for memory allocation")
#define LWCORE_MEMORY_SIZE_T 16
#endif

#if LWCORE_MEMORY_SIZE_T == 8
using memsize_t = uint8_t;

#define LWCORE_MEMSIZE_MAXUINT 0xFF
#define LWCORE_MEMSIZE_MAXINT  0x7F
#elif LWCORE_MEMORY_SIZE_T == 16
using memsize_t = uint16_t;

#define LWCORE_MEMSIZE_MAXUINT 0xFFFF
#define LWCORE_MEMSIZE_MAXINT  0x7FFF
#elif LWCORE_MEMORY_SIZE_T == 32
using memsize_t = uint32_t;

#define LWCORE_MEMSIZE_MAXUINT 0xFFFFFFFF
#define LWCORE_MEMSIZE_MAXINT  0x7FFFFFFF
#elif LWCORE_MEMORY_SIZE_T == 64
static_assert (sizeof(size_t) == 8, "Usage of 64bits size_t on a 32bits architecture!");
using memsize_t = uint64_t;

#define LWCORE_MEMSIZE_MAXUINT 0xFFFFFFFFFFFFFFFF
#define LWCORE_MEMSIZE_MAXINT  0x7FFFFFFFFFFFFFFF
#else
#error "Invalid LWCORE_MEMORY_SIZE_T: expecting 8, 16, 32 or 64bits"
#endif


#ifdef LWCORE_MEMORY_TRACE_LEAKS
#define /*NOLINT*/ __lwcore_monitoring__
#define /*NOLINT*/ __lwcore_trace__
#define /*NOLINT*/ __lwcore_ptrace__

#else
#pragma message ("Info: Memory leaks detection enabled")
#define /*NOLINT*/ __lwcore_monitoring__ , const char* __pFile__, int __lineno__
#define /*NOLINT*/ __lwcore_trace__ ,__pFile__ ,__lineno__
#define /*NOLINT*/ __lwcore_ptrace__ ,__FILE__,__LINE__

#endif // LWCORE_MEMORY_TRACE_LEAKS

#define /*NOLINT*/ __lw_new(...) __lw_new0(lw::Session::Instance(), ##__VA_ARGS__, 2, 1)

#define /*NOLINT*/ __lw_new0(session, chunk, argc) __lw_new##argc(session, chunk)
#define /*NOLINT*/ __lw_new1(session, ...) new(session __lwcore_ptrace__)
#define /*NOLINT*/ __lw_new2(session, chunk) new(session, chunk __lwcore_ptrace__)

#define /*NOLINT*/ __lw_delete(T)

#else

#define __lw_new(...)                new
#define __lw_delete(...)             delete

#endif // LWCORE_USE_MEMORY_MANAGER

#define /*NOLINT*/ __lw_warning(x) Session::Instance()->logger().warning(x __lwcore_ptrace__)
#define /*NOLINT*/ __lw_error(x) Session::Instance()->logger().error(x __lwcore_ptrace__)

#define /*NOLINT*/ __lw_warning_if(statement, x) if (statement) __lw_warning(x)
#define /*NOLINT*/ __lw_error_if(statement, x) if (statement) __lw_error(x)

#define /*NOLINT*/ __lw_assert(x, ...) __lw_assert0(x, ##__VA_ARGS__, 2, 1)
#define /*NOLINT*/ __lw_assert0(x, msg, lvl, ...) __lw_assert##lvl(x, msg)

#if defined(LWCORE_DEBUG) || not defined(LWCORE_EMBEDDED)
#define /*NOLINT*/ __lw_assert1(x, ...) if (!(x)) throw lw::RuntimeException(__FILE__, __LINE__)
#define /*NOLINT*/ __lw_assert2(x, msg) if (!(x)) throw lw::RuntimeException(msg, __FILE__, __LINE__)
#define /*NOLINT*/ __lw_ifdebug(x) { x }
#else
#define /*NOLINT*/ __lw_ifdebug(x)
#define /*NOLINT*/ __lw_assert1(x, ...) if (!(x)) lw::Session::Instance()->ErrorHandle()
#define /*NOLINT*/ __lw_assert2(x, msg) if (!(x)) lw::Session::Instance()->ErrorHandle()
#endif // LWCORE_DEBUG

#define /*NOLINT*/ __lw_assert_nonnull(x) __lw_assert(x != nullptr, #x " is null")
#define /*NOLINT*/ __lw_assert_return(x, IfFalse) if(!(x)) return IfFalse

#define LWCORE_DEFINE_SINGLETON(obj_t) \
    private: static obj_t* _pInstance; \
    public: static obj_t* Instance();

#define LWCORE_IMPLEMENT_SINGLETON(obj_t) \
    static obj_t* _pInstance = nullptr;

namespace lw
{
    
} // namespace lw

#endif // _lwcore_defs_h_