
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Object.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>

namespace KMS
{
    namespace DI
    {

        class Object
        {

        public:

            virtual ~Object();

            virtual bool Clear() = 0;

            Callback_Ptr mOnChanged;

        // Internal

            virtual void Send_OnChanged(void* aData = nullptr);

        protected:

            Object();

        };

    }
}
