//------------------------------------------------------------------------------
//
// File:        architecture.h
// Description: Definitions of macros to manage system architecture
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
#ifndef _lwcore_architecture_h_
#define _lwcore_architecture_h_

#if defined(__amd64__)

#elif defined(__i386__)

#elif defined(__aarch32__)


#elif defined(__aarch64__)

#else
#error "Current architecture not taking in charge by lwcore library"
#endif // architecture

#endif // _lwcore_architecture_h_