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
#ifndef RuntimeException_H
#define RuntimeException_H

#include "lwcore/lwcore-defs.h"

namespace lw
{
    class RuntimeException final
    {
    public:
        RuntimeException();
        RuntimeException(const char* _ipFile, int _iLine);
        RuntimeException(const char* _msg, const char* _file, int _line);
    };
}

#endif // RuntimeException_H
