
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/NetAddressRange.h
// Library   KMS-B

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

            static DI::Object* Create();

            NetAddressRange();

            NetAddressRange(const char* aIn);

            void operator = (const Net::AddressRange& aIn);

            operator const Net::AddressRange& () const;

            const Net::AddressRange& Get() const;

            Net::AddressRange& Get();

            // ===== Value ==================================================
            virtual void Set(const char* aIn);

            // ===== Object =================================================
            virtual ~NetAddressRange();

        private:

            Net::AddressRange mInternal;

        };

    }
}
