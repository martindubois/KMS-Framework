
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/DI_NetAddressRange.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>
#include <KMS/DI/NetAddressRange.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        DI::Object* NetAddressRange::Create() { return new NetAddressRange; }

        NetAddressRange::NetAddressRange() {}

        NetAddressRange::NetAddressRange(const char* aIn) { Set(aIn); }

        void NetAddressRange::operator = (const Net::AddressRange& aIn) { mInternal = aIn; }

        const Net::AddressRange& NetAddressRange::Get() const { return mInternal; }

        // ===== Value ======================================================
        
        void NetAddressRange::Set(const char* aIn) { mInternal = Convert::ToNetAddressRange(aIn); };
        
        // ===== Object =====================================================

        NetAddressRange::~NetAddressRange() {}

    }
}
