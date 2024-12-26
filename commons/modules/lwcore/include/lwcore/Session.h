//------------------------------------------------------------------------------
//
// File:        Session.h
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
#ifndef Session_H
#define Session_H
#include "lwcore/lwcore-defs.h"
// C standard lib
#include <cstdint>


namespace lw
{
    namespace memory
    {
        class MemoryManager;
    }

    /**
     * @details singleton
     */
    class Session
    {
    public:
        struct Handle
        {
            /*
             *
             */
            uint32_t (*tick)();
            /*
             *
             */
            void (*delay)(uint32_t);
            /*
             *
             */
            void (*error_handle)();
        };

        //
        // Static singleton method
        //

        static Session* Create(Handle);
        static void Destroy();
        static Session* Instance();

        static uint32_t OpState();

        //
        // Memory
        //
#ifdef LWCORE_USE_MEMORY_MANAGER
        memory::MemoryManager& memoryManager();
#endif

        //
        // Handler
        //

        void ErrorHandle() const;

    private:
        //
        // Private constructor
        //
        explicit Session(Handle iHndl);

        //
        // Private fields
        //

        // Session handler
        Handle _hndl;

        // last operation's state
        uint32_t _lastOperationState = SESSION_STATE_OK;

        static Session* _pInstance;
    };
}

namespace elw // Embedded LightWhale
{
    using Session = lw::Session;
}

#endif //Session_H
