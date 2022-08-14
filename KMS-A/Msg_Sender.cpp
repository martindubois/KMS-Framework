
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Msg_Sender.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Msg/IReceiver.h>

#include <KMS/Msg/Sender.h>

namespace KMS
{
    namespace Msg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Sender::Sender(void* aSender) : mSender(aSender)
        {
            assert(NULL != aSender);

            Clear();
        }

        Sender::Sender(void* aSender, IReceiver* aReceiver, unsigned int aCode) : mCode(aCode), mReceiver(aReceiver), mSender(aSender)
        {
            assert(NULL != aSender);
            assert(NULL != aReceiver);
            assert(0 < aCode);
        }

        void Sender::Clear() { mCode = 0; mReceiver = NULL; }

        bool Sender::IsSet() const { return NULL != mReceiver; }

        void Sender::Set(IReceiver* aR, unsigned int aCode)
        {
            assert(NULL != aR);
            assert(0 < aCode);

            mCode     = aCode;
            mReceiver = aR;
        }

        unsigned int Sender::Send(void* aData)
        {
            unsigned int lResult = 0;

            assert(NULL != mSender);

            if (NULL != mReceiver)
            {
                assert(0 < mCode);

                try
                {
                    lResult = mReceiver->Receive(mSender, mCode, aData);

                    if (0 != (lResult & IReceiver::MSG_SENDER_CLEAR))
                    {
                        Clear();
                        lResult &= ~ IReceiver::MSG_SENDER_CLEAR;
                    }
                }
                catch (Exception eE)
                {
                    lResult = IReceiver::MSG_EXCEPTION;
                }
                catch (std::exception eE)
                {
                    lResult = IReceiver::MSG_EXCEPTION_STD;
                }
                catch (...)
                {
                    lResult = IReceiver::MSG_EXCEPTION_UNKNOWN;
                }
            }
            else
            {
                lResult = IReceiver::MSG_IGNORED;
            }

            return lResult;
        }

    }
}
