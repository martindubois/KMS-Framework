
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Msg_IReceiver.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Msg/IReceiver.h>

namespace KMS
{
    namespace Msg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        // Errors
        const unsigned int IReceiver::MSG_ERROR                   = 0x00000001;
        const unsigned int IReceiver::MSG_EXCEPTION               = 0x00000002;
        const unsigned int IReceiver::MSG_EXCEPTION_STD           = 0x00000004;
        const unsigned int IReceiver::MSG_EXCEPTION_UNKNOWN       = 0x00000008;

        // Warnings
        const unsigned int IReceiver::MSG_IGNORED                 = 0x00010000;

        // Actions
        const unsigned int IReceiver::MSG_SENDER_CLEAR            = 0x01000000;

        // Groups
        const unsigned int IReceiver::MSG_ALL_ERRORS              = 0x0000ffff;
        const unsigned int IReceiver::MSG_ALL_ERRORS_AND_WARNINGS = 0x00ffffff;

    }
}
