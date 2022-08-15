
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Msg/Receiver.h

#pragma once

namespace KMS
{
    namespace Msg
    {

        class IReceiver
        {

        public:

            // Errors
            static const unsigned int MSG_ERROR;
            static const unsigned int MSG_EXCEPTION;
            static const unsigned int MSG_EXCEPTION_STD;
            static const unsigned int MSG_EXCEPTION_UNKNOWN;

            // Warnings
            static const unsigned int MSG_IGNORED;

            // Action
            static const unsigned int MSG_ACTION_CLEAR;
            static const unsigned int MSG_ACTION_QUEUE;
            static const unsigned int MSG_ACTION_STOP;

            // Groups
            static const unsigned int MSG_ERRORS;
            static const unsigned int MSG_ERRORS_AND_WARNINGS;

            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData) = 0;

        };

    }
}

#define KMS_MSG_SUCCESS(R)            (0 == ((R) & KMS::Msg::IReceiver::MSG_ERRORS_AND_WARNINGS))
#define KMS_MSG_SUCCESS_OR_WARNING(R) (0 == ((R) & KMS::Msg::IReceiver::MSG_ERRORS))
