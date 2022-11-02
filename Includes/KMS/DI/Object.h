
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Object.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Msg/Destination.h>

namespace KMS
{
    namespace DI
    {

        class Object
        {

        public:

            virtual ~Object();

            virtual bool Clear() = 0;

            Msg::Destination mOnChanged;

        // Internal

            virtual void Send_OnChanged(void* aData = NULL);

        protected:

            Object();

        };

    }
}
