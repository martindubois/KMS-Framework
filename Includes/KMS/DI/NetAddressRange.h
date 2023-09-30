
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/NetAddressRange.h
// Library   KMS-B

// External type : Net::AddressRange (const char*)
// Internal type : Net::AddressRange (std::string)

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/String_Expand.h>
#include <KMS/Net/AddressRange.h>

namespace KMS
{
    namespace DI
    {

        class NetAddressRange : public String_Expand
        {

        public:

            static const Net::AddressRange DEFAULT_VALUE;

            static DI::Object* Create();

            NetAddressRange();

            NetAddressRange(const char* aIn);

            void operator = (const Net::AddressRange& aIn);

            operator const Net::AddressRange& () const;

            const Net::AddressRange& Get() const;

            Net::AddressRange& Get();

            // ===== Value ==================================================
            virtual void Set(const char* aIn);

        // Internal

            Net::AddressRange mInternal;

        };

    }
}
