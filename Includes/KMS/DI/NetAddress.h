
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/NetAddress.h
// Library   KMS-B

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/String_Expand.h>
#include <KMS/Net/Address.h>

namespace KMS
{
    namespace DI
    {

        class NetAddress : public String_Expand
        {

        public:

            static DI::Object* Create();

            NetAddress();

            NetAddress(const char* aIn);

            void operator = (const Net::Address& aIn);

            operator const Net::Address& () const;

            const Net::Address& Get() const;

            Net::Address& Get();

            // ===== Value ==================================================
            virtual void Set(const char* aIn);

            // ===== Object =================================================
            virtual ~NetAddress();

        private:

            Net::Address mInternal;

        };

    }
}
