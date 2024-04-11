
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Socket_Client_TLS.cpp

// TEST COVERAGE 2023-10-17 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Net/Socket_Client_TLS.h>

// Contants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_REMOTE_NAME("RemoteName = {Name}");

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Socket_Client_TLS::REMOTE_NAME_DEFAULT = "";

        Socket_Client_TLS::Socket_Client_TLS() : Socket_Client(Socket::Type::STREAM)
        {
            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&mRemoteName, false); AddEntry("RemoteName", lEntry, &MD_REMOTE_NAME);

            Construct_OSDep();
        }

        Socket_Client_TLS::~Socket_Client_TLS() { VerifyState(State::CLOSED); }

    }
}
