
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/DI_NetAddress.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>
#include <KMS/DI/NetAddress.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        DI::Object* NetAddress::Create() { return new NetAddress; }

        NetAddress::NetAddress() {}

        NetAddress::NetAddress(const char* aIn) { Set(aIn); }

        void NetAddress::operator = (const Net::Address& aA) { mInternal = aA; }

        const Net::Address& NetAddress::Get() const { return mInternal; }

        Net::Address& NetAddress::Get() { return mInternal; }

        // ===== Value ======================================================
        
        void NetAddress::Set(const char* aIn)
        {
            String_Expand::Set(aIn);

            mInternal = Convert::ToNetAddress(String_Expand::Get());
        }

        // ===== Object =====================================================
        
        NetAddress::~NetAddress() {}

    }
}
