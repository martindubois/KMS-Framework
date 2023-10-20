
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/NetAddress.h
// Library   KMS-B

// External type : Net::Address (const char*)
// Internal type : Net::Address (std::string)

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/String_Expand.h>
#include <KMS/Net/Address.h>

namespace KMS
{
    namespace DI
    {

        class NetAddress final : public String_Expand
        {

        public:

            static const Net::Address DEFAULT_VALUE;

            static DI::Object* Create();

            NetAddress();

            NetAddress(const char* aIn);

            void operator = (const Net::Address& aIn);

            operator const Net::Address& () const;

            const Net::Address& Get() const;

            Net::Address& Get();

            // ===== Value ==================================================
            virtual void Set(const char* aIn);

        // Internal

            Net::Address mInternal;

        };

    }
}
