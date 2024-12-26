/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include "lwcore/Session.h"

namespace lw
{
    Session* Session::Create(Handle iHndl)
    {
        return nullptr;
    }

    void Session::Destroy()
    {

    }

    Session *Session::Instance()
    {
        return _pInstance;
    }

    uint32_t Session::OpState()
    {
        return _pInstance->_lastOperationState;
    }

    void Session::ErrorHandle() const
    {
        _hndl.error_handle();
    }

}